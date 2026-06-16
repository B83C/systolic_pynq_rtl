`timescale 1ns / 1ps

module quantizer_wrapper #(
    parameter unsigned SIZE           = 4,
    parameter unsigned DATA_WIDTH_IN  = 32,
    parameter unsigned DATA_WIDTH_OUT = 8,
    parameter unsigned ACCUM_WIDTH    = 32,
    parameter unsigned MAX_MUL_Q       = 65535,
    parameter unsigned MAX_SHIFT       = 31,
    parameter integer  MAX_ZP_OUT      = 127,
    parameter integer  MIN_ZP_OUT      = -128
) (
    input  wire         clk,
    input  wire         rst_n,

    // AXI4-Lite: config / status
    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [3:0]  s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output wire        s_axil_bvalid,
    input  wire        s_axil_bready,

    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [3:0]  s_axil_araddr,
    output wire [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output wire        s_axil_rvalid,
    input  wire        s_axil_rready,

    input  wire [SIZE*DATA_WIDTH_IN -1:0] s_axis_tdata,
    input  wire                           s_axis_tvalid,
    output wire                           s_axis_tready,
    input  wire                           s_axis_tlast,

    output wire [SIZE*DATA_WIDTH_OUT-1:0] m_axis_tdata,
    output wire                           m_axis_tvalid,
    input  wire                           m_axis_tready,
    output wire                           m_axis_tlast
);

    quantizer #(
        .SIZE         (SIZE),
        .DATA_WIDTH_IN(DATA_WIDTH_IN),
        .DATA_WIDTH_OUT(DATA_WIDTH_OUT),
        .ACCUM_WIDTH  (ACCUM_WIDTH),
        .MAX_MUL_Q(MAX_MUL_Q),
        .MAX_SHIFT(MAX_SHIFT),
        .MAX_ZP_OUT(MAX_ZP_OUT),
        .MIN_ZP_OUT(MIN_ZP_OUT)
    ) u_quant (
        .clk            (clk),
        .rst_n          (rst_n),
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
        .s_axis_tdata   (s_axis_tdata),
        .s_axis_tvalid  (s_axis_tvalid),
        .s_axis_tready  (s_axis_tready),
        .s_axis_tlast   (s_axis_tlast),
        .m_axis_tdata   (m_axis_tdata),
        .m_axis_tvalid  (m_axis_tvalid),
        .m_axis_tready  (m_axis_tready),
        .m_axis_tlast   (m_axis_tlast)
    );

endmodule
