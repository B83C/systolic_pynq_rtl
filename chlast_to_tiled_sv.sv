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

    input logic                            bypass_i,
    input logic                            cfg_channels_wen,
    input logic [$clog2(MAX_CHANNELS+1)-1:0]   cfg_channels_wdata,
    input logic                            repeat_cnt_wen,
    input logic [$clog2(MAX_REPLAY_CNT+1)-1:0] repeat_cnt_wdata
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

  logic [CH_PER_BEAT*DATA_WIDTH-1:0] buffer[2][OUT_COL][CH_BLOCKS];

  logic [CFG_CH_W-1:0]     cfg_channels;
  logic [REPLAY_CNT_W-1:0] repeat_cnt;
  wire  [CFG_CH_W-1:0]     ch_blocks_max = cfg_channels >> CH_PER_BEAT_LOG2;

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
  wire  out_last = last_replay && out_buf_cntr == cfg_channels - 1;
  wire  input_last;
  logic tlast_seen;
  logic output_has_tlast;

  always_comb begin
    unique case (out_state)
      OUT_REPLAYING: begin
        if (out_last && !pending && !input_last && m_axis_tready) begin
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
    end else if (bypass_i) begin
      out_state <= OUT_IDLE;
    end else begin
      out_state <= out_state_nxt;
    end
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cfg_channels  <= MAX_CHANNELS;
      repeat_cnt    <= 1;
      out_buf_cntr  <= 0;
      out_replay_cnt <= repeat_cnt - 1;
    end else begin
      if (cfg_channels_wen) cfg_channels <= cfg_channels_wdata;
      if (repeat_cnt_wen)   repeat_cnt   <= repeat_cnt_wdata;
      if (bypass_i) begin
        out_buf_cntr   <= 0;
        out_replay_cnt <= repeat_cnt - 1;
      end else if (out_state == OUT_REPLAYING && m_axis_tready) begin
        if (out_buf_cntr == cfg_channels - 1) begin
          out_buf_cntr <= 0;
          if (out_replay_cnt == 0) begin
            out_replay_cnt <= repeat_cnt - 1;
          end else begin
            out_replay_cnt <= out_replay_cnt - 1;
          end
        end else begin
          out_buf_cntr <= out_buf_cntr + 1;
        end
      end else if (out_state_nxt == OUT_REPLAYING) begin
        out_replay_cnt <= repeat_cnt - 1;
      end
    end
  end

  wire [OUT_WIDTH - 1:0] output_row;

  genvar i;
  generate
    for (i = 0; i < OUT_COL; i++) begin : gen_out_conn
      wire [$bits(
out_buf_cntr
) - SP_BITS -1:0] ch_block = out_buf_cntr[$bits(
          out_buf_cntr
      )-1:SP_BITS];
      wire [SP_BITS-1:0] inner = out_buf_cntr[SP_BITS-1:0];
      assign output_row[i*DATA_WIDTH+:DATA_WIDTH] =
        buffer[out_buf_sel][i][ch_block][inner * DATA_WIDTH +: DATA_WIDTH ];
    end
  endgenerate

  assign m_axis_tdata  = bypass_i ? s_axis_tdata : output_row;
  assign m_axis_tvalid = bypass_i ? s_axis_tvalid : out_state == OUT_REPLAYING;
  assign m_axis_tlast  = bypass_i ? s_axis_tlast : out_last && output_has_tlast;


  wire has_free = (out_state == OUT_IDLE || !pending) && !tlast_seen;
  assign s_axis_tready = bypass_i ? m_axis_tready : has_free;
  wire accept_data = !bypass_i && s_axis_tvalid && s_axis_tready;

  logic [OUT_COL_BITS - 1:0] col_cntr;
  logic [CHBLK_BITS - 1:0] row_cntr;
  wire input_sel = !out_buf_sel;
  assign input_last = accept_data && (row_cntr == ch_blocks_max - 1) && (col_cntr == OUT_COL - 1);

  always @(posedge clk, negedge rst_n) begin
    if (pending && out_last) begin
      pending <= 0;
      pending_has_tlast <= 0;
      output_has_tlast <= pending_has_tlast;
      out_buf_sel <= !out_buf_sel;
    end
    if (!rst_n) begin
      col_cntr <= 0;
      row_cntr <= 0;
      pending <= 0;
      tlast_seen <= 0;
      pending_has_tlast <= 0;
    end else if (!bypass_i && (accept_data || tlast_seen)) begin
      if (tlast_seen) begin
        buffer[input_sel][col_cntr][row_cntr] <= 0;
      end else begin
        buffer[input_sel][col_cntr][row_cntr] <= s_axis_tdata;
      end

      if (s_axis_tlast && !tlast_seen && !input_last) begin
        tlast_seen <= 1;
      end

      if (row_cntr == ch_blocks_max - 1) begin
        row_cntr <= 0;
        if (col_cntr == OUT_COL - 1) begin
          col_cntr <= 0;
          if ((out_state == OUT_REPLAYING && !out_last)) begin
            pending <= 1;
            pending_has_tlast <= s_axis_tlast;
          end else begin
            tlast_seen <= 0;
            output_has_tlast <= s_axis_tlast;
            out_buf_sel <= !out_buf_sel;
          end
        end else begin
          col_cntr <= col_cntr + 1;
        end
      end else begin
        row_cntr <= row_cntr + 1;
      end
    end
  end
endmodule
