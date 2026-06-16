`timescale 1ns / 1ps

// Tiled → channel-last converter with gather+flush (ping-pong).
//
// Instead of writing 1 byte at a time to the BRAM (8 partial-word WEs
// per input beat, each with its own read-modify-write chain), we:
//   1. Accumulate 1 byte per beat in a small LUT-RAM gather buffer.
//   2. After 8 beats the gather is full; flush all 8 words to the main
//      BRAM as full 64-bit writes (1 WE per word, no fanout).
//   3. Ping-pong between two gather sets so flushing doesn't stall input.
//
// The critical write-side path
//   cfg_channels_q → in_last → in_cnt → ch_blk → BRAM byte WE
// becomes
//   accept_data → gather WE   (1 bit, fast LUT-RAM byte write)
//   gather_full → BRAM flush  (1 bit, registered flush FSM)
//   flush_cnt   → BRAM addr   (registered 3-bit counter, 0 fanout)
//
// Input:  tiled — beat = one channel across OUT_COL spatial positions
// Output: channel-last — beat = CH_PER_BEAT channels at one spatial position

module tiled_to_chlast_gather_sv #(
    parameter DATA_WIDTH   = 8,
    parameter CH_PER_BEAT  = 8,
    parameter MAX_CHANNELS = 64,
    parameter OUT_COL      = 8
) (
    input  logic                          clk,
    rst_n,
    input  logic [OUT_COL*DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                          s_axis_tvalid,
    output logic                          s_axis_tready,
    input  logic                          s_axis_tlast,

    output logic [CH_PER_BEAT*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                              m_axis_tvalid,
    input  logic                              m_axis_tready,
    output logic                              m_axis_tlast,

    // AXI4-Lite: config
    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [ 3:0] s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output reg         s_axil_bvalid,
    input  wire        s_axil_bready,

    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [ 3:0] s_axil_araddr,
    output reg  [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output reg         s_axil_rvalid,
    input  wire        s_axil_rready
);

  initial
    assert ((MAX_CHANNELS & (MAX_CHANNELS - 1)) == 0)
    else $fatal(1, "tiled_to_chlast_gather_sv: MAX_CHANNELS must be a power of 2");

  localparam unsigned CH_PER_BEAT_LOG2 = $clog2(CH_PER_BEAT);
  localparam CH_BLOCKS = MAX_CHANNELS / CH_PER_BEAT;
  localparam CHBLK_BITS = (CH_BLOCKS > 1) ? $clog2(CH_BLOCKS) : 1;
  localparam SP_BITS = (CH_PER_BEAT > 2) ? $clog2(CH_PER_BEAT) : 1;
  localparam OUT_COL_BITS = $clog2(OUT_COL);
  localparam CFG_CH_W = $clog2(MAX_CHANNELS + 1);
  localparam REG_TC_CH = 4'h0;
  localparam REG_TC_BYPASS = 4'h4;
  logic bypass_r;

  // Main BRAM — 2 banks × OUT_COL × CH_BLOCKS entries, flattened to 2D.
  // Full-word writes only (no byte-level enables).
`ifndef FORCE_LUTRAM
  (* ram_style = "block" *) logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2 * OUT_COL * CH_BLOCKS];
`else
  (* ram_style = "distributed" *) logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2 * OUT_COL * CH_BLOCKS];
`endif

  // Row-gather LUT-RAM — ping-pong between 2 sets.
  // Byte-wide elements so `inner` is an address index, not a variable
  // bit-select — Vivado can infer distributed RAM.
  (* ram_style = "distributed" *) logic [DATA_WIDTH-1:0] gather[2][OUT_COL][CH_PER_BEAT];

  // cfg_channels is the live AXI reg; cfg_channels_q is the shadow used by
  // the FSM.  cfg_ch_block_mask_q is a one-hot bit at position
  // (ch_blocks_max - 1), derived from cfg_channels_q once on AXI write.
  logic [CFG_CH_W-1:0]  cfg_channels;
  logic [CFG_CH_W-1:0]  cfg_channels_q;
  logic [CH_BLOCKS-1:0] cfg_ch_block_mask_q;

  wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;
  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  typedef enum {
    OUT_REPLAYING,
    OUT_IDLE
  } state_t;
  state_t state, state_nxt;

  logic                    out_buf_sel;
  logic [    CFG_CH_W-1:0] in_cnt;
  logic [OUT_COL_BITS-1:0] out_col_cnt;
  logic [  CHBLK_BITS-1:0] out_ch_cnt;

  wire [SP_BITS-1:0] inner = in_cnt[SP_BITS-1:0];
  wire [CHBLK_BITS-1:0] ch_blk = in_cnt[SP_BITS +: CHBLK_BITS];

  wire in_last = in_cnt == cfg_channels - 1;
  wire out_ch_last = cfg_ch_block_mask_q[out_ch_cnt];
  wire out_last = (out_col_cnt == OUT_COL_BITS'(OUT_COL - 1)) && out_ch_last;

  logic pending, pending_has_tlast, output_has_tlast;
  logic pending_flush;
  wire input_last;
  logic tlast_seen;

  wire input_sel = !out_buf_sel;
  wire accept_data = !bypass_r && s_axis_tvalid && s_axis_tready;

  // ── Gather FSM ──────────────────────────────────────────────────────
  typedef enum {FILL, FLUSH} gstate_t;
  gstate_t gstate;

  logic              gather_set;         // which set is being written
  logic              flush_sel;          // input_sel captured at flush start
  logic [CHBLK_BITS-1:0] gather_ch_blk; // ch_block being gathered
  logic [  SP_BITS-1:0] flush_cnt;      // 0..CH_PER_BEAT-1

  wire gather_full = accept_data && (inner == SP_BITS'(CH_PER_BEAT - 1));
  wire gather_flush_done = flush_cnt == SP_BITS'(CH_PER_BEAT - 1);

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      gstate        <= FILL;
      gather_set    <= 0;
      flush_sel     <= 0;
      gather_ch_blk <= 0;
      flush_cnt     <= 0;
    end else begin
      unique case (gstate)
        FILL: begin
          if (gather_full) begin
            // 8th beat captured — gather full.  Start flush, swap set.
            gstate        <= FLUSH;
            flush_sel     <= input_sel;
            gather_ch_blk <= ch_blk;
            gather_set    <= gather_set + 1;  // next set
            flush_cnt     <= 0;
          end
        end
        FLUSH: begin
          if (gather_flush_done) begin
            gstate <= FILL;
          end else begin
            flush_cnt <= flush_cnt + 1;
          end
        end
      endcase
    end
  end

  // Gather write: 1 byte per col per input beat
  always @(posedge clk) begin
    if (accept_data) begin
      for (int c = 0; c < OUT_COL; c++) begin
        gather[gather_set][c][inner]
          <= tlast_seen ? {DATA_WIDTH{1'b0}} : s_axis_tdata[c*DATA_WIDTH+:DATA_WIDTH];
      end
    end
  end

  // Flush: write gather[flush_set] → buffer[flush_sel] over CH_PER_BEAT cycles.
  // Each cycle writes one packed word (full 64-bit write to BRAM, 1 WE).
  // Unrolled loop packs CH_PER_BEAT byte-wide elements into the buffer word.
  always @(posedge clk) begin
    if (gstate == FLUSH) begin
      for (int b = 0; b < CH_PER_BEAT; b++) begin
        buffer[(flush_sel * OUT_COL + flush_cnt) * CH_BLOCKS + gather_ch_blk][DATA_WIDTH*b +: DATA_WIDTH]
          <= gather[gather_set - 1][flush_cnt][b];
      end
    end
  end

  // ── Input capture counter ───────────────────────────────────────────
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) in_cnt <= 0;
    else if (accept_data) begin
      in_cnt <= in_last ? 0 : in_cnt + 1;
    end
  end

  assign input_last = accept_data && in_last;

  // ── tlast / pending / bank-swap logic ───────────────────────────────
  always @(posedge clk, negedge rst_n) begin
    if (pending && out_last && gather_flush_done) begin
      // Replay finished.  If no pending flush, swap now; otherwise the
      // flush handler will do the swap and keep pending_has_tlast.
      if (!pending_flush) begin
        output_has_tlast  <= pending_has_tlast;
        out_buf_sel       <= !out_buf_sel;
      end
      pending           <= 0;
      pending_has_tlast <= 0;
    end

    if (pending_flush && gather_flush_done) begin
      pending_flush     <= 0;
      tlast_seen        <= 0;
      output_has_tlast  <= pending_has_tlast;
      out_buf_sel       <= !out_buf_sel;
    end

    if (!rst_n) begin
      pending           <= 0;
      pending_flush     <= 0;
      tlast_seen        <= 0;
      pending_has_tlast <= 0;
      output_has_tlast  <= 0;
    end else if (!bypass_r && (accept_data || tlast_seen)) begin
      // (write to gather is handled above; this block only tracks
      //  tlast_seen and handles bank-swap on frame boundary.)
      if (s_axis_tlast && !tlast_seen && !input_last) tlast_seen <= 1;

      if (input_last) begin
        if (state == OUT_REPLAYING && !out_last) begin
          pending           <= 1;
          pending_has_tlast <= s_axis_tlast;
        end else if (gather_full) begin
          // Gather just filled on this edge; flush just started.
          // Defer the bank-swap until the flush finishes so the
          // replay doesn't read a partially-written ch_block.
          pending_flush     <= 1;
          pending_has_tlast <= s_axis_tlast;
        end else begin
          tlast_seen       <= 0;
          output_has_tlast <= tlast_seen || s_axis_tlast;
          out_buf_sel      <= !out_buf_sel;
        end
      end
    end
  end

  // ── Output replay FSM ───────────────────────────────────────────────
  logic replay_armed;  // latched when in_last fires but flush in progress

  always_comb begin
    unique case (state)
      OUT_IDLE: state_nxt = ((s_axis_tvalid && s_axis_tready && in_last && !gather_full) ||
                              (replay_armed && !pending_flush))
                               ? OUT_REPLAYING : OUT_IDLE;
      OUT_REPLAYING:
      state_nxt = (out_last && !pending && !input_last && out_pipe_adv_comb) ? OUT_IDLE : OUT_REPLAYING;
    endcase
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      state <= OUT_IDLE;
      replay_armed <= 0;
    end else begin
      state <= state_nxt;
      if (state == OUT_IDLE && state_nxt == OUT_REPLAYING)
        replay_armed <= 0;
      else if (input_last && gather_full && state == OUT_IDLE)
        replay_armed <= 1;
    end
  end

  // ── AXI-Lite config ────────────────────────────────────────────────
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cfg_channels         <= MAX_CHANNELS;
      cfg_channels_q       <= MAX_CHANNELS;
      cfg_ch_block_mask_q  <= {CH_BLOCKS{1'b1}};
      out_col_cnt          <= 0;
      out_ch_cnt           <= 0;
      bypass_r             <= 0;
      s_axil_bvalid        <= 0;
      s_axil_rvalid  <= 0;
      s_axil_rdata   <= 0;
    end else begin
      if (axil_wr_en) begin
        case (s_axil_awaddr)
          REG_TC_CH:     cfg_channels <= (s_axil_wdata[CFG_CH_W-1:0] == 0 || s_axil_wdata[CFG_CH_W-1:0] > MAX_CHANNELS)
                                          ? MAX_CHANNELS : s_axil_wdata[CFG_CH_W-1:0];
          REG_TC_BYPASS: bypass_r <= s_axil_wdata[0];
          default:       ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (state == OUT_IDLE) begin
        cfg_channels_q      <= cfg_channels;
        cfg_ch_block_mask_q <= CH_BLOCKS'(1) << ((cfg_channels >> CH_PER_BEAT_LOG2) - 1);
      end

      if (axil_rd_en) begin
        case (s_axil_araddr)
          REG_TC_CH:     s_axil_rdata <= {{32 - CFG_CH_W{1'b0}}, cfg_channels};
          REG_TC_BYPASS: s_axil_rdata <= {31'h0, bypass_r};
          default:       s_axil_rdata <= 0;
        endcase
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end

      if (state == OUT_REPLAYING && out_pipe_adv_comb) begin
        if (out_ch_last) begin
          out_ch_cnt  <= 0;
          out_col_cnt <= out_last ? 0 : out_col_cnt + 1;
        end else begin
          out_ch_cnt <= out_ch_cnt + 1;
        end
      end
    end
  end

  // ── Output pipeline stage ───────────────────────────────────────────
  wire [CH_PER_BEAT*DATA_WIDTH-1:0] tdata_pre = buffer[(out_buf_sel * OUT_COL + out_col_cnt) * CH_BLOCKS + out_ch_cnt];
  wire tvalid_pre = rst_n && state == OUT_REPLAYING;
  wire tlast_pre = out_last && output_has_tlast;
  wire out_pipe_adv_comb = !bypass_r && (!m_axis_tvalid_r || m_axis_tready);
  logic [CH_PER_BEAT*DATA_WIDTH-1:0] m_axis_tdata_r;
  logic m_axis_tvalid_r;
  logic m_axis_tlast_r;
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata_r  <= 0;
      m_axis_tvalid_r <= 0;
      m_axis_tlast_r  <= 0;
    end else if (out_pipe_adv_comb) begin
      m_axis_tdata_r  <= tdata_pre;
      m_axis_tvalid_r <= tvalid_pre;
      m_axis_tlast_r  <= tlast_pre;
    end
  end
  assign m_axis_tdata  = bypass_r ? s_axis_tdata  : m_axis_tdata_r;
  assign m_axis_tvalid = bypass_r ? s_axis_tvalid : m_axis_tvalid_r;
  assign m_axis_tlast  = bypass_r ? s_axis_tlast  : m_axis_tlast_r;

  assign s_axis_tready = bypass_r ? m_axis_tready :
    rst_n && (state == OUT_IDLE || !pending) && !tlast_seen;

endmodule
