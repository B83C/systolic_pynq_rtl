`timescale 1ns / 1ps

// Tiled → channel-last converter (column-first output).
// Input:  tiled — beat = one channel across OUT_COL spatial positions
// Output: channel-last — beat = CH_PER_BEAT channels at one spatial position

module tiled_to_chlast_sv #(
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

    // AXI4-Lite: config (only REG_CFG_CHANNELS)
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
    else $fatal(1, "tiled_to_chlast_sv: MAX_CHANNELS must be a power of 2");

  localparam unsigned CH_PER_BEAT_LOG2 = $clog2(CH_PER_BEAT);
  localparam CH_BLOCKS = MAX_CHANNELS / CH_PER_BEAT;
  localparam CHBLK_BITS = (CH_BLOCKS > 1) ? $clog2(CH_BLOCKS) : 1;
  localparam SP_BITS = (CH_PER_BEAT > 2) ? $clog2(CH_PER_BEAT) : 1;
  localparam OUT_COL_BITS = $clog2(OUT_COL);
  localparam CFG_CH_W = $clog2(MAX_CHANNELS + 1);
  localparam REG_TC_CH = 4'h0;
  localparam REG_TC_BYPASS = 4'h4;
  logic bypass_r;

  logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2][OUT_COL][CH_BLOCKS];

  // cfg_channels is the live AXI reg; cfg_channels_q is the shadow used by
  // the FSM.  cfg_ch_block_mask_q is a one-hot bit at position
  // (ch_blocks_max - 1), derived from cfg_channels_q once on AXI write.
  // The FSM uses cfg_ch_block_mask_q (single-LUT mask lookup) for
  // out_ch_last, avoiding a runtime subtractor+comparator that limited
  // timing closure.
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

  wire  [     SP_BITS-1:0] inner = in_cnt[SP_BITS-1:0];
  wire  [  CHBLK_BITS-1:0] ch_blk = in_cnt[$bits(in_cnt)-1:SP_BITS];
  wire  [OUT_COL_BITS-1:0] out_col = out_col_cnt;
  wire  [  CHBLK_BITS-1:0] out_ch = out_ch_cnt;

  wire                     in_last = in_cnt == cfg_channels - 1;
  wire                          out_ch_last = cfg_ch_block_mask_q[out_ch_cnt];
  wire                     out_last = (out_col_cnt == OUT_COL - 1) && out_ch_last;

  logic pending, pending_has_tlast, output_has_tlast;
  wire  input_last;
  logic tlast_seen;

  wire  input_sel = !out_buf_sel;

  // `out_pipe_adv_comb` is true when the output pipeline reg can advance
  // (no back-pressure holding it).  The FSM uses this as the "beat
  // accepted" signal.  Declared further down (after the pipeline reg).

  always_comb begin
    unique case (state)
      OUT_IDLE: state_nxt = (s_axis_tvalid && s_axis_tready && in_last) ? OUT_REPLAYING : OUT_IDLE;
      OUT_REPLAYING:
      state_nxt = (out_last && !pending && !input_last && out_pipe_adv_comb) ? OUT_IDLE : OUT_REPLAYING;
    endcase
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) state <= OUT_IDLE;
    else state <= state_nxt;
  end

  // output counters + AXI-Lite config
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
          REG_TC_CH:     cfg_channels <= s_axil_wdata[CFG_CH_W-1:0];
          REG_TC_BYPASS: bypass_r <= s_axil_wdata[0];
          default:       ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      // Shadow reg: latch cfg_channels and compute the one-hot mask
      // only when state is OUT_IDLE.  The mask is the runtime value,
      // but computing it once per AXI write (not every cycle) keeps it
      // off the FSM's critical timing path.  out_ch_last uses the mask
      // as a single LUT: cfg_ch_block_mask_q[out_ch_cnt].
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

  // input capture counter
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) in_cnt <= 0;
    else if (s_axis_tvalid && s_axis_tready) begin
      in_cnt <= in_last ? 0 : in_cnt + 1;
    end
  end

  // buffer swap (now handled in input capture block below)
  /* removed - handled in input capture block */

  wire accept_data = !bypass_r && s_axis_tvalid && s_axis_tready;
  assign input_last = accept_data && in_last;

  // input capture + tlast tracking
  always @(posedge clk, negedge rst_n) begin
    if (pending && out_last) begin
      pending           <= 0;
      pending_has_tlast <= 0;
      output_has_tlast  <= pending_has_tlast;
      out_buf_sel       <= !out_buf_sel;
    end
    if (!rst_n) begin
      pending           <= 0;
      tlast_seen        <= 0;
      pending_has_tlast <= 0;
      output_has_tlast  <= 0;
    end else if (!bypass_r && (accept_data || tlast_seen)) begin
      if (tlast_seen) begin
        for (int c = 0; c < OUT_COL; c++) begin
          buffer[input_sel][c][ch_blk][inner] <= 0;
        end
      end else begin
        for (int c = 0; c < OUT_COL; c++) begin
          buffer[input_sel][c][ch_blk][inner * DATA_WIDTH +: DATA_WIDTH]
            <= s_axis_tdata[c*DATA_WIDTH+:DATA_WIDTH];
        end
      end

      if (s_axis_tlast && !tlast_seen && !input_last) tlast_seen <= 1;

      if (input_last) begin
        if (state == OUT_REPLAYING && !out_last) begin
          pending           <= 1;
          pending_has_tlast <= s_axis_tlast;
        end else begin
          tlast_seen       <= 0;
          output_has_tlast <= tlast_seen || s_axis_tlast;
          out_buf_sel      <= !out_buf_sel;
        end
      end
    end
  end

  // Output pipeline stage — register tdata/tvalid/tlast to break the long
  // combinational path from the BRAM read to the AXI-Stream output.
  // In bypass mode the signals pass straight through (combinational) so
  // bypass has zero added latency.  When not bypassed, the pipeline reg
  // stalls on downstream back-pressure (m_axis_tready=0 with valid
  // pending) so no data is dropped.  The FSM also stalls on the same
  // condition (`out_pipe_adv_comb`).
  wire [CH_PER_BEAT*DATA_WIDTH-1:0] tdata_pre = buffer[out_buf_sel][out_col][out_ch];
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
  assign m_axis_tdata = bypass_r ? s_axis_tdata : m_axis_tdata_r;
  assign m_axis_tvalid = bypass_r ? s_axis_tvalid : m_axis_tvalid_r;
  assign m_axis_tlast = bypass_r ? s_axis_tlast : m_axis_tlast_r;

  assign s_axis_tready = bypass_r ? m_axis_tready :
    rst_n && (state == OUT_IDLE || !pending) && !tlast_seen;

endmodule
