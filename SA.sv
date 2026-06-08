`timescale 1ns / 1ps

module SA #(
    parameter unsigned SIZE           = 8,
    parameter unsigned MAX_LOOP       = 3 * SIZE,
    parameter unsigned DATA_WIDTH_IN  = 8,
    parameter unsigned DATA_WIDTH_OUT = 32
) (
    input clk,
    input rst_n,

    input valid,
    input [SIZE-1:0] current_row,
    input load_a,
    input load_b,
    input [7:0] loop_len_a,

    input [ DATA_WIDTH_IN-1:0] a_row[SIZE],
    input [ DATA_WIDTH_IN-1:0] b_row[SIZE],
    input [DATA_WIDTH_OUT-1:0] c_row[SIZE],

    output [DATA_WIDTH_OUT-1:0] result_row[SIZE]
);
  logic [DATA_WIDTH_IN-1:0] b_inner[SIZE][SIZE];
  logic [DATA_WIDTH_IN-1:0] a_inner_loop[SIZE][MAX_LOOP];

  logic [DATA_WIDTH_OUT-1:0] partial_sum[SIZE + 1][SIZE];

  initial begin
    b_inner = '{default: '{default: '0}};
    a_inner_loop = '{default: '{default: '0}};
  end

  genvar k;
  generate
    for (k = 0; k < SIZE; k = k + 1) begin : gen_assign_ports
      assign result_row[k] = partial_sum[SIZE][k];
    end
  endgenerate

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      b_inner <= '{default: '{default: '0}};
      a_inner_loop <= '{default: '{default: '0}};
    end else if (valid) begin
      for (integer i = 0; i < SIZE; i++) begin
        partial_sum[0][i] <= c_row[i];

        a_inner_loop[i][0] <= load_a ? a_row[i] : a_inner_loop[i][loop_len_a - 1];

        for (integer j = 0; j < MAX_LOOP - 1; j++) begin
          a_inner_loop[i][j + 1] <= a_inner_loop[i][j];
        end

        if (current_row[i] && load_b) begin
          b_inner[i] <= b_row;
        end
      end
    end
  end

  genvar i, j;

  generate
    for (i = 0; i < SIZE; i = i + 1) begin : gen_row
      for (j = 0; j < SIZE; j = j + 1) begin : gen_col
        wire [DATA_WIDTH_IN-1:0] b = b_inner[i][j];
        wire [DATA_WIDTH_IN-1:0] a = a_inner_loop[i][i];

        pe #(
            .DATA_WIDTH_IN (DATA_WIDTH_IN),
            .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
        ) pe_unit (
            .clk  (clk),
            .rst_n(rst_n),
            .tick (valid),

            .a_in(a),
            .b_in(b),
            .c_in(partial_sum[i][j]),
            .partial_sum(partial_sum[i+1][j])
        );
      end
    end
  endgenerate

endmodule
