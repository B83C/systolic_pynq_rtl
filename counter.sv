`timescale 1ns / 1ps

module counter #(
    parameter unsigned DYN = 0,
    parameter unsigned COUNTDOWN = 0,
    parameter unsigned MAX = 8,
    localparam unsigned MaxW = MAX > 1 ? $clog2(MAX) : 1
) (
    input wire clk,
    input wire en,
    input wire rst_n,

    input reg [MaxW - 1:0] dyn_max = 0,

    output wire ending,
    output reg [MaxW - 1:0] count
);
  generate
    if (COUNTDOWN) begin
      assign ending = count == 0;
    end else begin
      if (DYN) begin : gen_dynamic_compare
        assign ending = count == dyn_max;
      end else begin : gen_static_compare
        assign ending = count == MaxW'(MAX - 1);
      end
    end
  endgenerate

  generate
    always @(posedge clk, negedge rst_n) begin
      if (~rst_n) begin
        if (COUNTDOWN) begin
          count <= dyn_max;
        end else begin
          count <= 0;
        end
      end else if (en) begin
        if (COUNTDOWN) begin
          if (ending) begin
            count <= dyn_max;
          end else begin
            count <= count - 1;
          end
        end else begin
          if (ending) begin
            count <= 0;
          end else begin
            count <= count + 1;
          end
        end
      end
    end

  endgenerate

endmodule
