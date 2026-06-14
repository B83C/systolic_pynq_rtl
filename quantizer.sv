`timescale 1ns / 1ps

// Quantizer:  q_out = clamp(((acc * mul_q) >>> shift) + zp_out, -128, 127)
// 1 pipeline register on DSP output breaks the combinational chain.
// Output: SIZE * 8-bit packed int8.

module quantizer #(
    parameter unsigned SIZE          = 4,
    parameter unsigned DATA_WIDTH_IN = 32,
    parameter unsigned ACCUM_WIDTH   = 32
) (
    input  logic clk,
    input  logic rst_n,

    input  logic [15:0] mul_q,
    input  logic [ 4:0] shift,
    input  logic [ 7:0] zp_out,

    // AXI-Stream slave (raw accumulators, SIZE × 32-bit)
    input  logic [SIZE*DATA_WIDTH_IN-1:0] s_axis_tdata,
    input  logic                          s_axis_tvalid,
    output logic                          s_axis_tready,
    input  logic                          s_axis_tlast,

    // AXI-Stream master (quantized int8, SIZE × 8-bit)
    output logic [SIZE*8-1:0] m_axis_tdata,
    output logic              m_axis_tvalid,
    input  logic              m_axis_tready,
    output logic              m_axis_tlast
);

    wire signed [ACCUM_WIDTH-1:0] acc[SIZE];
    generate
        for (genvar i = 0; i < SIZE; i++) begin : gen_acc_slice
            assign acc[i] = s_axis_tdata[i*DATA_WIDTH_IN+:DATA_WIDTH_IN];
        end
    endgenerate

    wire signed [ACCUM_WIDTH+15:0] q_prod[SIZE];
    wire signed [ACCUM_WIDTH+15:0] q_shifted[SIZE];

    logic signed [ACCUM_WIDTH+15:0] q_shifted_r[SIZE];
    logic p_valid, p_last;

    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_quant
            (* use_dsp = "yes" *)
            assign q_prod[qi]   = $signed(acc[qi]) * $signed({16'h0, mul_q});
            assign q_shifted[qi] = $signed(q_prod[qi]) >>> shift;
        end
    endgenerate

    wire signed [15:0] q_with_zp[SIZE];
    wire signed [ 7:0] q_out[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_clamp
            assign q_with_zp[qi] = $signed(q_shifted_r[qi][15:0]) + $signed({{8{zp_out[7]}}, zp_out});
            assign q_out[qi]     = (q_with_zp[qi] > 127)   ? 8'sd127 :
                                   (q_with_zp[qi] < -128) ? -8'sd128 :
                                                               q_with_zp[qi][7:0];
        end
    endgenerate

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            q_shifted_r <= '{default: 0};
            p_valid <= 0; p_last <= 0;
        end else begin
            q_shifted_r <= q_shifted;
            p_valid <= s_axis_tvalid;
            p_last  <= s_axis_tvalid && s_axis_tlast;
        end
    end

    assign s_axis_tready = 1'b1;

    assign m_axis_tvalid = p_valid;
    assign m_axis_tlast  = p_valid && p_last;

    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_out
            assign m_axis_tdata[qi*8+:8] = q_out[qi];
        end
    endgenerate

endmodule
