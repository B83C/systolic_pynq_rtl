`timescale 1ns / 1ps

// Channel-last → tiled converter with ping-pong replay.
// Input:  channel-interleaved (8 channels per beat for one spatial pixel)
// Output: spatial-interleaved (one channel across 8 spatial positions per beat)

module chlast_to_tiled_sv #(
    parameter DATA_WIDTH   = 8,
    parameter CH_PER_BEAT  = 8,
    parameter MAX_CHANNELS = 64,
    parameter OUT_COL      = 8,
    parameter MAX_REPLAY_CNT = 16
) (
    input  logic                              clk,
    rst_n,
    input  logic [CH_PER_BEAT*DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                              s_axis_tvalid,
    output logic                              s_axis_tready,
    input  logic                              s_axis_tlast,

    output logic [OUT_COL*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                          m_axis_tvalid,
    input  logic                          m_axis_tready,
    output logic                          m_axis_tlast,



    // AXI4-Lite: config (REG_CFG_CHANNELS, REG_REPEAT_CNT)
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
    else $fatal(1, "chlast_to_tiled_sv: MAX_CHANNELS must be a power of 2");

  localparam unsigned CH_PER_BEAT_LOG2 = $clog2(CH_PER_BEAT);
  localparam CH_BLOCKS = MAX_CHANNELS / CH_PER_BEAT;
  localparam CHBLK_BITS = (CH_BLOCKS > 1) ? $clog2(CH_BLOCKS) : 1;
  localparam SP_BITS = (CH_PER_BEAT > 2) ? $clog2(CH_PER_BEAT) : 1;
  // localparam MAX_ROWS = (MAX_SPATIAL / CH_PER_BEAT) * MAX_CHANNELS;
  localparam ADDR_BITS = $clog2(CH_BLOCKS);
  // localparam CNT_BITS = (MAX_ROWS > 1) ? $clog2(MAX_ROWS + 1) : 1;
  localparam OUT_WIDTH = DATA_WIDTH * OUT_COL;
  localparam OUT_COL_BITS = $clog2(OUT_COL);
  localparam CFG_CH_W     = $clog2(MAX_CHANNELS + 1);
  localparam REPLAY_CNT_W = $clog2(MAX_REPLAY_CNT + 1);
  // BRAM element is [CH_PER_BEAT*DATA_WIDTH-1:0]; index needs this many bits
  localparam unsigned BUF_IDX_W = (CH_PER_BEAT * DATA_WIDTH > 1) ?
                                    $clog2(CH_PER_BEAT * DATA_WIDTH) : 1;
  localparam REG_CT_CH      = 4'h0;
  localparam REG_CT_RPT     = 4'h4;
  localparam REG_CT_BYPASS  = 4'h8;
  logic bypass_r;

  (* ram_style = "block" *) logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2 * OUT_COL * CH_BLOCKS];

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

  typedef enum {
    OUT_REPLAYING,
    OUT_IDLE
  } output_state_t;

  output_state_t out_state, out_state_nxt;
  logic out_buf_sel;
  logic [CFG_CH_W-1:0] out_buf_cntr;
  logic [REPLAY_CNT_W-1:0] out_replay_cnt;

  logic pending, pending_has_tlast;
  wire  last_replay = out_replay_cnt == 0;
  wire  out_last = last_replay && out_buf_cntr == cfg_channels_q - 1;
  wire  input_last;
  logic tlast_seen;
  logic output_has_tlast;

  always_comb begin
    unique case (out_state)
      OUT_REPLAYING: begin
        if (out_last && !pending && !input_last && out_pipe_adv_comb) begin
          out_state_nxt = OUT_IDLE;
        end else begin
          out_state_nxt = OUT_REPLAYING;
        end
      end
      OUT_IDLE: begin
        if (input_last) begin
          out_state_nxt = OUT_REPLAYING;
        end else begin
          out_state_nxt = OUT_IDLE;
        end
      end
    endcase
  end

   always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      out_state <= OUT_IDLE;
    end else if (bypass_r) begin
      out_state <= OUT_IDLE;
    end else begin
      out_state <= out_state_nxt;
    end
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cfg_channels  <= MAX_CHANNELS;
      cfg_channels_q <= MAX_CHANNELS;
      repeat_cnt    <= 1;
      repeat_cnt_q  <= 1;
      out_buf_cntr  <= 0;
      out_replay_cnt <= repeat_cnt - 1;
      bypass_r      <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
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

      // Shadow regs: latch cfg_channels and repeat_cnt only when state is OUT_IDLE.
      // Prevents mid-frame glitches to ch_blocks_max, out_last, out_replay_cnt.
      if (out_state == OUT_IDLE) begin
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
      end else if (out_state == OUT_REPLAYING && out_pipe_adv_comb) begin
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
      end else if (out_state_nxt == OUT_REPLAYING) begin
        out_replay_cnt <= repeat_cnt_q - 1;
      end
    end
  end

  wire [OUT_WIDTH - 1:0] output_row;

  genvar i;
  generate
    for (i = 0; i < OUT_COL; i++) begin : gen_out_conn
      // out_buf_cntr[$bits(out_buf_cntr)-1:SP_BITS] captures the channel-
      // block index.  Width is the natural slice; Vivado/Verilator may
      // complain that it's wider than CHBLK_BITS but the upper bits
      // are unused for the buffer index.
      wire [CHBLK_BITS-1:0] ch_block = out_buf_cntr[SP_BITS +: CHBLK_BITS];
      wire [SP_BITS-1:0] inner = out_buf_cntr[SP_BITS-1:0];
      // buffer is [CH_PER_BEAT*DATA_WIDTH-1:0] wide, so its index needs
      // BUF_IDX_W bits.  inner is SP_BITS bits, so we extend to the
      // right width to silence WIDTHTRUNC.
      assign output_row[i*DATA_WIDTH+:DATA_WIDTH] =
        buffer[(out_buf_sel * OUT_COL + i) * CH_BLOCKS + ch_block][BUF_IDX_W'(inner * DATA_WIDTH) +: DATA_WIDTH ];
    end
  endgenerate

  // Output pipeline stage — register tdata/tvalid/tlast to break the long
  // combinational path from the BRAM read to the AXI-Stream output.
  // In bypass mode the signals pass straight through (combinational) so
  // bypass has zero added latency.  When not bypassed, the pipeline reg
  // stalls on downstream back-pressure (m_axis_tready=0 with valid
  // pending) so no data is dropped.  FSM also stalls on same condition.
  wire [OUT_COL*DATA_WIDTH-1:0] tdata_pre  = output_row;
  wire                          tvalid_pre = out_state == OUT_REPLAYING;
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
      m_axis_tdata_r  <= tdata_pre;
      m_axis_tvalid_r <= tvalid_pre;
      m_axis_tlast_r  <= tlast_pre;
    end
  end
  assign m_axis_tdata  = bypass_r ? s_axis_tdata  : m_axis_tdata_r;
  assign m_axis_tvalid = bypass_r ? s_axis_tvalid : m_axis_tvalid_r;
  assign m_axis_tlast  = bypass_r ? s_axis_tlast  : m_axis_tlast_r;


  wire has_free = (out_state == OUT_IDLE || !pending) && !tlast_seen;
  assign s_axis_tready = bypass_r ? m_axis_tready : has_free;
  wire accept_data = !bypass_r && s_axis_tvalid && s_axis_tready;

  logic [OUT_COL_BITS - 1:0] col_cntr;
  logic [CHBLK_BITS - 1:0] row_cntr;
  wire input_sel = !out_buf_sel;
  assign input_last = accept_data
    && (row_cntr == CHBLK_BITS'(ch_blocks_max - 1))
    && (col_cntr == OUT_COL_BITS'(OUT_COL - 1));
  // (uses ch_blocks_max from cfg_channels_q)

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
      // pending-frame swap when output completes a frame
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

        if (s_axis_tlast && !tlast_seen && !input_last) begin
          tlast_seen <= 1;
        end

        if (row_cntr == CHBLK_BITS'(ch_blocks_max - 1)) begin
          row_cntr <= 0;
          if (col_cntr == OUT_COL_BITS'(OUT_COL - 1)) begin
            col_cntr <= 0;
            if ((out_state == OUT_REPLAYING && !out_last)) begin
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
