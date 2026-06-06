`timescale 1ns / 1ps

module sa_module #(
    parameter unsigned SIZE = 8,
    parameter unsigned DATA_WIDTH_IN = 8,
    parameter unsigned DATA_WIDTH_OUT = 32,
    parameter unsigned C_S_AXIS_DATA_WIDTH = 64,
    localparam unsigned C_M_AXIS_DATA_WIDTH = C_S_AXIS_DATA_WIDTH * (DATA_WIDTH_OUT / DATA_WIDTH_IN)
) (
    input wire aclk,
    input wire aresetn,

    input wire [C_S_AXIS_DATA_WIDTH-1 : 0] s_axis_A_tdata,
    input wire s_axis_A_tvalid,
    output wire s_axis_A_tready,
    input wire s_axis_A_tlast,

    input wire [C_S_AXIS_DATA_WIDTH-1 : 0] s_axis_B_tdata,
    input wire s_axis_B_tvalid,
    output wire s_axis_B_tready,
    input wire s_axis_B_tlast,

    output [C_M_AXIS_DATA_WIDTH-1 : 0] m_axis_tdata,
    output m_axis_tvalid,
    input wire m_axis_tready,
    output wire m_axis_tlast
);

  SA_wrapper #(
      .SIZE(SIZE),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .C_S_AXIS_DATA_WIDTH(C_S_AXIS_DATA_WIDTH)
  ) sa_wrapper (
      .*
  );

endmodule
