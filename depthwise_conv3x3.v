`timescale 1ns / 1ps
(* keep = "true" *)
module depthwise_conv3x3 #(
    parameter DATA_WIDTH = 8,
    parameter CHANNELS   = 8,
    parameter IMG_W      = 32,
    parameter IMG_H      = 32
) (
    input  wire clk,
    input  wire rst_n,

    input  wire [CHANNELS*DATA_WIDTH-1:0] s_axis_tdata,
    input  wire                            s_axis_tvalid,
    output wire                            s_axis_tready,
    input  wire                            s_axis_tlast,

    output wire [CHANNELS*DATA_WIDTH-1:0] m_axis_tdata,
    output wire                            m_axis_tvalid,
    input  wire                            m_axis_tready,
    output wire                            m_axis_tlast,

    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [ 7:0] s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output wire        s_axil_bvalid,
    input  wire        s_axil_bready,

    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [ 7:0] s_axil_araddr,
    output wire [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output wire        s_axil_rvalid,
    input  wire        s_axil_rready
);

  depthwise_conv3x3_sv #(
      .DATA_WIDTH (DATA_WIDTH),
      .CHANNELS   (CHANNELS),
      .IMG_W      (IMG_W),
      .IMG_H      (IMG_H)
  ) impl (
      .clk               (clk),
      .rst_n             (rst_n),
      .s_axis_tdata      (s_axis_tdata),
      .s_axis_tvalid     (s_axis_tvalid),
      .s_axis_tready     (s_axis_tready),
      .s_axis_tlast      (s_axis_tlast),
      .m_axis_tdata      (m_axis_tdata),
      .m_axis_tvalid     (m_axis_tvalid),
      .m_axis_tready     (m_axis_tready),
      .m_axis_tlast      (m_axis_tlast),
      .s_axil_awvalid    (s_axil_awvalid),
      .s_axil_awready    (s_axil_awready),
      .s_axil_awaddr     (s_axil_awaddr),
      .s_axil_wdata      (s_axil_wdata),
      .s_axil_wvalid     (s_axil_wvalid),
      .s_axil_wready     (s_axil_wready),
      .s_axil_bresp      (s_axil_bresp),
      .s_axil_bvalid     (s_axil_bvalid),
      .s_axil_bready     (s_axil_bready),
      .s_axil_arvalid    (s_axil_arvalid),
      .s_axil_arready    (s_axil_arready),
      .s_axil_araddr     (s_axil_araddr),
      .s_axil_rdata      (s_axil_rdata),
      .s_axil_rresp      (s_axil_rresp),
      .s_axil_rvalid     (s_axil_rvalid),
      .s_axil_rready     (s_axil_rready)
  );

endmodule
