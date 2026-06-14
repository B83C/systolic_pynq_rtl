`timescale 1ns / 1ps

module SA #(
    parameter unsigned SIZE           = 8,
    parameter unsigned DATA_WIDTH_IN  = 8,
    parameter unsigned DATA_WIDTH_OUT = 32,
    parameter unsigned ACCUM_WIDTH    = 32
) (
    input clk,
    input rst_n,

    input valid,

    input [DATA_WIDTH_IN-1:0] a_row[SIZE],
    input [DATA_WIDTH_IN-1:0] b_row[SIZE][SIZE],
    input [  ACCUM_WIDTH-1:0] c_row[SIZE],

    output [ACCUM_WIDTH-1:0] result_row[SIZE]
);

  wire [ACCUM_WIDTH-1:0] partial_sum[SIZE + 1][SIZE];

  assign partial_sum[0] = c_row;
  assign result_row = partial_sum[SIZE];

  genvar i, j;
  generate
    for (i = 0; i < SIZE; i = i + 1) begin : gen_row
      for (j = 0; j < SIZE; j = j + 1) begin : gen_col
        pe #(
            .DATA_WIDTH_IN(DATA_WIDTH_IN),
            .DATA_WIDTH_OUT(DATA_WIDTH_OUT),
            .ACCUM_WIDTH(ACCUM_WIDTH)
        ) pe_unit (
            .clk  (clk),
            .rst_n(rst_n),
            .tick (valid),

            .a_in(a_row[i]),
            .b_in(b_row[i][j]),
            .c_in(partial_sum[i][j]),
            .partial_sum(partial_sum[i+1][j])
        );
      end
    end
  endgenerate
endmodule
