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

    input  wire [$clog2(MAX_MUL_Q+1)-1:0]    mul_q,
    input  wire [$clog2(MAX_SHIFT+1)-1:0]    shift,
    input  wire [$clog2(MAX_ZP_OUT-MIN_ZP_OUT+1)-1:0] zp_out,

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
        .mul_q          (mul_q),
        .shift          (shift),
        .zp_out         (zp_out),
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
