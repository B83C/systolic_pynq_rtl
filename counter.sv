`timescale 1ns / 1ps


module counter #(
    parameter  unsigned MAX  = 8,
    localparam unsigned MaxW = MAX > 1 ? $clog2(MAX) : 1
) (
    input wire clk,
    input wire en,
    input wire rstn,

    output wire ending,
    output reg [MaxW - 1:0] count
);

`ifndef SYNTHESIS
  initial count = 0;
`endif

  assign ending = count == MaxW'(MAX - 1);

  always @(posedge clk, negedge rstn) begin
    if (~rstn) begin
      count <= 0;
    end else if (en) begin
      if (ending) begin
        count <= 0;
      end else begin
        count <= count + 1;
      end
    end
  end

endmodule
