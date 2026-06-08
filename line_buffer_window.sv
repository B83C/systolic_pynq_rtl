`timescale 1ns / 1ps

module line_buffer_window #(
    parameter unsigned IMG_W = 10,
    parameter unsigned IMG_H = 10,
    parameter unsigned K = 3,
    parameter unsigned DW = 8
) (
    input clk,
    input rst_n,

    input [DW-1:0] pix_in,
    input pix_valid,
    input pix_last,
    input pix_sof,
    output logic pix_ready,

    input window_ready,

    output logic [DW-1:0] window [K*K],
    output logic window_valid,
    output logic window_last
);

  localparam ROW_W = $clog2(K);
  localparam COL_W = $clog2(IMG_W);

  logic [DW-1:0] line_buf[K][IMG_W];
  logic [ROW_W-1:0] wr_row;
  logic [COL_W-1:0] wr_col;
  logic [$clog2(IMG_H):0] row_rx;
  logic in_frame;

  genvar gk, gl;
  generate
    for (gk = 0; gk < K; gk++) begin : gen_win_row
      for (gl = 0; gl < K; gl++) begin : gen_win_col
        assign window[gk * K + gl] =
            line_buf[(wr_row + 1 + gk) % K][wr_col - K + 1 + gl];
      end
    end
  endgenerate

  assign window_valid = (row_rx >= K - 1) && (wr_col >= K - 1) && in_frame;
  assign window_last = window_valid && pix_last && (row_rx == IMG_H - 1);
  assign pix_ready = !window_valid || window_ready;

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      wr_row <= 0;
      wr_col <= 0;
      row_rx <= 0;
      in_frame <= 0;
    end else begin
      if (pix_sof) begin
        wr_row <= 0;
        wr_col <= 0;
        row_rx <= 0;
        in_frame <= 1;
      end

      if (pix_valid && pix_ready) begin
        line_buf[wr_row][wr_col] <= pix_in;

        if (pix_last) begin
          wr_row <= (wr_row + 1 >= K) ? 0 : wr_row + 1;
          wr_col <= 0;
          row_rx <= row_rx + 1;
          if (row_rx == IMG_H - 1) in_frame <= 0;
        end else begin
          wr_col <= wr_col + 1;
        end
      end
    end
  end

endmodule
