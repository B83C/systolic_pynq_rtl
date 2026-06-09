`timescale 1ns / 1ps

(* use_dsp = "yes" *) module pe #(
    parameter unsigned DATA_WIDTH_IN = 16
) (
    input                        clk,
    input                        tick,
    input                        rst_n,
    input  [DATA_WIDTH_IN-1: 0]  a_in,
    input  [DATA_WIDTH_IN-1: 0]  b_in,
    input  [15:0]                c_in_lo,
    input  [15:0]                c_in_hi,
    output reg [15:0]            psum_lo,
    output reg [15:0]            psum_hi
);

  wire [31:0] product = a_in * b_in;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      psum_lo <= 16'd0;
      psum_hi <= 16'd0;
    end else if (tick) begin
      psum_lo <= c_in_lo + product[15:0];
      psum_hi <= c_in_hi + product[31:16];
    end
  end

endmodule
