`timescale 1ns / 1ps (* keep = "true" *)
module sa_wrapper_axi_ctrl #(
    parameter unsigned A_DEPTH        = 4,
    parameter unsigned C_DEPTH        = 4,
    parameter unsigned ACCUM_WIDTH    = 32,
    parameter unsigned SIZE           = 4,
    parameter unsigned DATA_WIDTH_IN  = 8,
    parameter unsigned DATA_WIDTH_OUT = 8
) (
    input wire clk,
    input wire rst_n,

    // AXI4-Stream: input B  (also used for A and C ring loading)
    input  wire [SIZE*DATA_WIDTH_IN-1:0] s_axis_B_tdata,
    input  wire                          s_axis_B_tvalid,
    output wire                          s_axis_B_tready,
    input  wire                          s_axis_B_tlast,

    // AXI4-Stream: output
    output wire [SIZE*DATA_WIDTH_OUT-1:0] m_axis_tdata,
    output wire                           m_axis_tvalid,
    input  wire                           m_axis_tready,
    output wire                           m_axis_tlast,

    // AXI4-Lite: control / status
    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [ 5:0] s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output wire        s_axil_bvalid,
    input  wire        s_axil_bready,

    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [ 5:0] s_axil_araddr,
    output wire [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output wire        s_axil_rvalid,
    input  wire        s_axil_rready,

    output wire a_bypass,
    output wire axis_bypass,
    output wire idle,

    output wire [15:0] o_mul_q,
    output wire [ 4:0] o_shift,
    output wire [ 7:0] o_zp_out,
    output wire [ 7:0] o_zp_in
);

  sa_wrapper_axi_ctrl_sv #(
      .SIZE(SIZE),
      .A_DEPTH(A_DEPTH),
      .C_DEPTH(C_DEPTH),
      .ACCUM_WIDTH(ACCUM_WIDTH),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) impl (
      .clk            (clk),
      .rst_n          (rst_n),
      .s_axis_B_tdata (s_axis_B_tdata),
      .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready),
      .s_axis_B_tlast (s_axis_B_tlast),
      .m_axis_tdata   (m_axis_tdata),
      .m_axis_tvalid  (m_axis_tvalid),
      .m_axis_tready  (m_axis_tready),
      .m_axis_tlast   (m_axis_tlast),
      .s_axil_awvalid (s_axil_awvalid),
      .s_axil_awready (s_axil_awready),
      .s_axil_awaddr  (s_axil_awaddr),
      .s_axil_wdata   (s_axil_wdata),
      .s_axil_wvalid  (s_axil_wvalid),
      .s_axil_wready  (s_axil_wready),
      .s_axil_bresp   (s_axil_bresp),
      .s_axil_bvalid  (s_axil_bvalid),
      .s_axil_bready  (s_axil_bready),
      .s_axil_arvalid (s_axil_arvalid),
      .s_axil_arready (s_axil_arready),
      .s_axil_araddr  (s_axil_araddr),
      .s_axil_rdata   (s_axil_rdata),
      .s_axil_rresp   (s_axil_rresp),
      .s_axil_rvalid  (s_axil_rvalid),
      .s_axil_rready  (s_axil_rready),
      .a_bypass       (a_bypass),
      .axis_bypass    (axis_bypass),
      .idle           (idle),
      .o_mul_q        (o_mul_q),
      .o_shift        (o_shift),
      .o_zp_out       (o_zp_out),
      .o_zp_in        (o_zp_in)
  );

endmodule
