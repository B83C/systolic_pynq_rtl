`timescale 1ns / 1ps

// Channel-last → tiled converter with on-the-fly gather (no GATHERING gap).
//
// The output tile reads one channel across OUT_COL spatial positions per beat.
// Instead of 8-way multiport reads from the BRAM buffer, a small gather
// LUT-RAM is populated byte-by-byte during the input phase — one byte per
// channel per spatial position, written directly into gather[bank][channel] at
// the correct byte offset.  The output replay reads a single gather address
// per beat: no GATHERING state, no 64-cycle gap between frames, no partial
// BRAM reads.
//
// Input:  channel-interleaved (8 channels per beat for one spatial pixel)
// Output: spatial-interleaved (one channel across 8 spatial positions per beat)

module chlast_to_tiled_gather_sv #(
    parameter DATA_WIDTH   = 8,
    parameter CH_PER_BEAT  = 8,
    parameter MAX_CHANNELS = 64,
    parameter OUT_COL      = 8,
    parameter MAX_REPLAY_CNT = 16
) (
    input  logic                              clk, rst_n,
    input  logic [CH_PER_BEAT*DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                              s_axis_tvalid,
    output logic                              s_axis_tready,
    input  logic                              s_axis_tlast,

    output logic [OUT_COL*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                          m_axis_tvalid,
    input  logic                          m_axis_tready,
    output logic                          m_axis_tlast,

    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [3:0]  s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output reg         s_axil_bvalid,
    input  wire        s_axil_bready,
    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [3:0]  s_axil_araddr,
    output reg  [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output reg         s_axil_rvalid,
    input  wire        s_axil_rready
);

  initial assert((MAX_CHANNELS & (MAX_CHANNELS - 1)) == 0)
    else $fatal(1, "chlast_to_tiled_gather_sv: MAX_CHANNELS must be a power of 2");

  localparam unsigned CH_PER_BEAT_LOG2 = $clog2(CH_PER_BEAT);
  localparam CH_BLOCKS = MAX_CHANNELS / CH_PER_BEAT;
  localparam CHBLK_BITS = (CH_BLOCKS > 1) ? $clog2(CH_BLOCKS) : 1;
  localparam SP_BITS = (CH_PER_BEAT > 2) ? $clog2(CH_PER_BEAT) : 1;
  localparam OUT_WIDTH = DATA_WIDTH * OUT_COL;
  localparam OUT_COL_BITS = $clog2(OUT_COL);
  localparam CFG_CH_W     = $clog2(MAX_CHANNELS + 1);
  localparam REPLAY_CNT_W = $clog2(MAX_REPLAY_CNT + 1);
  localparam unsigned BUF_IDX_W = (CH_PER_BEAT * DATA_WIDTH > 1) ?
                                    $clog2(CH_PER_BEAT * DATA_WIDTH) : 1;
  localparam GATHER_IDX_W = (OUT_COL * DATA_WIDTH > 1) ?
                             $clog2(OUT_COL * DATA_WIDTH) : 1;
  localparam REG_CT_CH      = 4'h0;
  localparam REG_CT_RPT     = 4'h4;
  localparam REG_CT_BYPASS  = 4'h8;
  logic bypass_r;

  // ── BRAM buffer (full-word writes from input, unused for output) ──
  (* ram_style = "block" *) logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2 * OUT_COL * CH_BLOCKS];

  // ── Gather LUT-RAM, ping-pong banks ────────────────────────────────
  // Gathered on-the-fly during input: one byte per (ch,spatial) per beat.
  // Output reads a full word (all OUT_COL spatial positions) per channel.
  (* ram_style = "distributed" *) logic [OUT_COL*DATA_WIDTH-1:0] gather[2][MAX_CHANNELS];

  logic [CFG_CH_W-1:0]     cfg_channels;
  logic [CFG_CH_W-1:0]     cfg_channels_q;
  logic [REPLAY_CNT_W-1:0] repeat_cnt;
  logic [REPLAY_CNT_W-1:0] repeat_cnt_q;
  wire  [CFG_CH_W-1:0]     ch_blocks_max = (cfg_channels_q + CH_PER_BEAT - 1) >> CH_PER_BEAT_LOG2;

  wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;
  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  // ── FSM: IDLE ↔ REPLAYING (no GATHER state) ──────────────────────
  typedef enum { IDLE, REPLAYING } state_t;
  state_t state, state_nxt;

  logic out_buf_sel;                    // ping-pong bank select for gather
  logic [CFG_CH_W-1:0] out_buf_cntr;    // output channel counter
  logic [REPLAY_CNT_W-1:0] out_replay_cnt;

  wire  last_replay = out_replay_cnt == 0;
  wire  out_last    = last_replay && out_buf_cntr == cfg_channels_q - 1;
  wire  input_last;

  logic pending, pending_has_tlast;
  logic tlast_seen;
  logic output_has_tlast;

  always_comb begin
    unique case (state)
      IDLE:       state_nxt = input_last ? REPLAYING : IDLE;
      REPLAYING:  begin
        if (out_last && !input_last && out_pipe_adv_comb) begin
          if (pending)  state_nxt = REPLAYING;  // keep playing (pending handler swapped bank)
          else          state_nxt = IDLE;
        end else begin
                        state_nxt = REPLAYING;
        end
      end
      default:    state_nxt = IDLE;
    endcase
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n)       state <= IDLE;
    else if (bypass_r) state <= IDLE;
    else               state <= state_nxt;
  end

  // ── AXI-Lite config + replay counter ──────────────────────────────
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cfg_channels   <= MAX_CHANNELS;
      cfg_channels_q <= MAX_CHANNELS;
      repeat_cnt     <= 1;
      repeat_cnt_q   <= 1;
      out_buf_cntr   <= 0;
      out_replay_cnt <= repeat_cnt - 1;
      bypass_r       <= 0;
      s_axil_bvalid  <= 0;
      s_axil_rvalid  <= 0;
      s_axil_rdata   <= 0;
    end else begin
      if (axil_wr_en) begin
        case (s_axil_awaddr)
          REG_CT_CH:     cfg_channels <= (s_axil_wdata[CFG_CH_W-1:0] == 0 || s_axil_wdata[CFG_CH_W-1:0] > MAX_CHANNELS)
                                          ? MAX_CHANNELS : s_axil_wdata[CFG_CH_W-1:0];
          REG_CT_RPT:    repeat_cnt   <= (s_axil_wdata[REPLAY_CNT_W-1:0] == 0)
                                          ? 1 : (s_axil_wdata[REPLAY_CNT_W-1:0] > MAX_REPLAY_CNT
                                                 ? MAX_REPLAY_CNT : s_axil_wdata[REPLAY_CNT_W-1:0]);
          REG_CT_BYPASS: bypass_r    <= s_axil_wdata[0];
          default: ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (state == IDLE) begin
        cfg_channels_q <= cfg_channels;
        repeat_cnt_q   <= repeat_cnt;
      end

      if (axil_rd_en) begin
        case (s_axil_araddr)
          REG_CT_CH:     s_axil_rdata <= {{32-CFG_CH_W{1'b0}},     cfg_channels};
          REG_CT_RPT:    s_axil_rdata <= {{32-REPLAY_CNT_W{1'b0}}, repeat_cnt};
          REG_CT_BYPASS: s_axil_rdata <= {31'h0, bypass_r};
          default:       s_axil_rdata <= 0;
        endcase
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end

      if (bypass_r) begin
        out_buf_cntr   <= 0;
        out_replay_cnt <= repeat_cnt_q - 1;
      end else if (state == REPLAYING && out_pipe_adv_comb) begin
        if (out_buf_cntr == cfg_channels_q - 1) begin
          out_buf_cntr <= 0;
          if (out_replay_cnt == 0) begin
            out_replay_cnt <= repeat_cnt_q - 1;
          end else begin
            out_replay_cnt <= out_replay_cnt - 1;
          end
        end else begin
          out_buf_cntr <= out_buf_cntr + 1;
        end
      end else if (state == IDLE && state_nxt == REPLAYING) begin
        out_buf_cntr   <= 0;
        out_replay_cnt <= repeat_cnt_q - 1;
      end
    end
  end

  // ── Output: read from gather[out_buf_sel][channel] ────────────────
  wire [OUT_WIDTH-1:0] output_row;
  generate
    for (genvar i = 0; i < OUT_COL; i++) begin : gen_out_conn
      wire [GATHER_IDX_W-1:0] oaddr = i * DATA_WIDTH;
      assign output_row[i*DATA_WIDTH+:DATA_WIDTH] = gather[out_buf_sel][out_buf_cntr][oaddr +: DATA_WIDTH];
    end
  endgenerate

  wire [OUT_COL*DATA_WIDTH-1:0] tdata_pre  = output_row;
  wire                          tvalid_pre = state == REPLAYING;
  wire                          tlast_pre  = out_last && output_has_tlast;
  wire                          out_pipe_adv_comb = !bypass_r && (!m_axis_tvalid_r || m_axis_tready);
  logic [OUT_COL*DATA_WIDTH-1:0] m_axis_tdata_r;
  logic                          m_axis_tvalid_r;
  logic                          m_axis_tlast_r;
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata_r  <= 0;
      m_axis_tvalid_r <= 0;
      m_axis_tlast_r  <= 0;
    end else if (out_pipe_adv_comb) begin
      m_axis_tdata_r  <= tvalid_pre ? tdata_pre : 0;
      m_axis_tvalid_r <= tvalid_pre;
      m_axis_tlast_r  <= tlast_pre;
    end
  end
  assign m_axis_tdata  = bypass_r ? s_axis_tdata  : m_axis_tdata_r;
  assign m_axis_tvalid = bypass_r ? s_axis_tvalid : m_axis_tvalid_r;
  assign m_axis_tlast  = bypass_r ? s_axis_tlast  : m_axis_tlast_r;

  // ── Input capture ───────────────────────────────────────────────────
  wire has_free = (state == IDLE || !pending) && !tlast_seen;
  assign s_axis_tready = bypass_r ? m_axis_tready : has_free;
  wire accept_data = !bypass_r && s_axis_tvalid && s_axis_tready;

  logic [OUT_COL_BITS-1:0] col_cntr;
  logic [CHBLK_BITS-1:0]   row_cntr;
  wire input_sel = !out_buf_sel;
  assign input_last = accept_data
    && (row_cntr == CHBLK_BITS'(ch_blocks_max - 1))
    && (col_cntr == OUT_COL_BITS'(OUT_COL - 1));

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      col_cntr         <= 0;
      row_cntr         <= 0;
      pending          <= 0;
      pending_has_tlast <= 0;
      tlast_seen       <= 0;
      output_has_tlast <= 0;
      out_buf_sel      <= 0;
    end else begin
      if (pending && out_last) begin
        pending          <= 0;
        pending_has_tlast <= 0;
        output_has_tlast <= pending_has_tlast;
        out_buf_sel      <= !out_buf_sel;
      end

      if (!bypass_r && (accept_data || tlast_seen)) begin
        if (tlast_seen) begin
          buffer[(input_sel * OUT_COL + col_cntr) * CH_BLOCKS + row_cntr] <= 0;
        end else begin
          buffer[(input_sel * OUT_COL + col_cntr) * CH_BLOCKS + row_cntr] <= s_axis_tdata;
        end

        // On-the-fly gather: write each channel byte into gather[bank][ch] at spatial offset
        if (!tlast_seen) begin
          for (int c = 0; c < CH_PER_BEAT; c++) begin
            automatic int ch_idx = row_cntr * CH_PER_BEAT + c;
            if (ch_idx < MAX_CHANNELS)
              gather[input_sel][ch_idx][GATHER_IDX_W'(col_cntr * DATA_WIDTH) +: DATA_WIDTH]
                <= s_axis_tdata[c * DATA_WIDTH +: DATA_WIDTH];
          end
        end

        if (s_axis_tlast && !tlast_seen && !input_last) begin
          tlast_seen <= 1;
        end

        if (row_cntr == CHBLK_BITS'(ch_blocks_max - 1)) begin
          row_cntr <= 0;
          if (col_cntr == OUT_COL_BITS'(OUT_COL - 1)) begin
            col_cntr <= 0;
            if ((state == REPLAYING && !out_last)) begin
              pending          <= 1;
              pending_has_tlast <= s_axis_tlast;
            end else begin
              tlast_seen       <= 0;
              output_has_tlast <= s_axis_tlast;
              out_buf_sel      <= !out_buf_sel;
            end
          end else begin
            col_cntr <= col_cntr + 1'b1;
          end
        end else begin
          row_cntr <= row_cntr + 1'b1;
        end
      end
    end
  end

endmodule
