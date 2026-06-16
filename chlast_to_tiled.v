`timescale 1ns / 1ps (* keep = "true" *)

module chlast_to_tiled #(
    parameter DATA_WIDTH   = 8,
    parameter CH_PER_BEAT  = 8,
    parameter MAX_CHANNELS = 64,
    parameter OUT_COL      = 8,
    parameter MAX_REPLAY_CNT = 16
) (
    input  wire                              clk,
    rst_n,
    input  wire [CH_PER_BEAT*DATA_WIDTH-1:0] s_axis_tdata,
    input  wire                              s_axis_tvalid,
    output wire                              s_axis_tready,
    input  wire                              s_axis_tlast,

    output wire [OUT_COL*DATA_WIDTH-1:0] m_axis_tdata,
    output wire                          m_axis_tvalid,
    input  wire                          m_axis_tready,
    output wire                          m_axis_tlast,

    input wire                            bypass_i,
    input wire [$clog2(MAX_CHANNELS+1)-1:0]   cfg_channels_i,
    input wire [$clog2(MAX_REPLAY_CNT+1)-1:0] repeat_cnt_i
);

  chlast_to_tiled_sv #(
      .DATA_WIDTH   (DATA_WIDTH),
      .CH_PER_BEAT  (CH_PER_BEAT),
      .MAX_CHANNELS (MAX_CHANNELS),
      .OUT_COL      (OUT_COL),
      .MAX_REPLAY_CNT(MAX_REPLAY_CNT)
  ) impl (
      .clk(clk),
      .s_axis_tdata(s_axis_tdata),
      .s_axis_tvalid(s_axis_tvalid),
      .s_axis_tready(s_axis_tready),
      .s_axis_tlast(s_axis_tlast),
      .m_axis_tdata(m_axis_tdata),
      .m_axis_tvalid(m_axis_tvalid),
      .m_axis_tready(m_axis_tready),
      .m_axis_tlast(m_axis_tlast),

      .bypass_i(bypass_i),
      .cfg_channels_i(cfg_channels_i),
      .repeat_cnt_i(repeat_cnt_i)
  );

endmodule
