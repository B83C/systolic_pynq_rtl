`timescale 1ns / 1ps

(* use_dsp = "yes" *) module pe #(
    parameter unsigned DATA_WIDTH_IN  = 8,
    parameter unsigned DATA_WIDTH_OUT = 32
) (
    input                       clk,
    input                       tick,
    input                       rst_n,
    input  [ DATA_WIDTH_IN-1:0] a_in,
    input  [ DATA_WIDTH_IN-1:0] b_in,
    input  [DATA_WIDTH_OUT-1:0] c_in,
    output [DATA_WIDTH_OUT-1:0] partial_sum
);

  wire signed [DATA_WIDTH_OUT-1:0] mac = $signed(a_in) * $signed(b_in) + $signed(c_in);

  reg signed  [DATA_WIDTH_OUT-1:0] sum;
  initial sum = 0;

  assign partial_sum = sum;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      sum <= 'd0;
    end else begin
      if (tick) begin
        sum <= mac;
      end
    end
  end

endmodule
