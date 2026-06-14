`timescale 1ns / 1ps

module quantizer_wrapper (
    input  wire         clk,
    input  wire         rst_n,

    input  wire [15:0]  mul_q,
    input  wire [ 4:0]  shift,
    input  wire [ 7:0]  zp_out,

    input  wire [127:0] s_axis_tdata,
    input  wire         s_axis_tvalid,
    output wire         s_axis_tready,
    input  wire         s_axis_tlast,

    output wire [ 31:0] m_axis_tdata,
    output wire         m_axis_tvalid,
    input  wire         m_axis_tready,
    output wire         m_axis_tlast
);

    quantizer #(
        .SIZE         (4),
        .DATA_WIDTH_IN(32),
        .ACCUM_WIDTH  (32)
    ) u_quant (
        .clk,
        .rst_n,
        .mul_q,
        .shift,
        .zp_out,
        .s_axis_tdata,
        .s_axis_tvalid,
        .s_axis_tready,
        .s_axis_tlast,
        .m_axis_tdata,
        .m_axis_tvalid,
        .m_axis_tready,
        .m_axis_tlast
    );

endmodule
