`timescale 1ns / 1ps

module conv_sa_tb;

  localparam IMG_W = 4;
  localparam IMG_H = 4;
  localparam K     = 2;
  localparam DW    = 8;
  localparam ACC_W = 32;
  localparam SA_SZ = K * K;

  reg clk, rst_n;

  reg [DW-1:0]   pix_tdata;
  reg            pix_tvalid;
  wire           pix_tready;
  reg            pix_tlast;
  reg            pix_tuser;

  reg [DW-1:0]   kernel_tdata;
  reg            kernel_tvalid;
  wire           kernel_tready;
  reg            kernel_tlast;

  reg [3:0] kernel_idx;
  reg [4:0] pix_idx;
  reg [3:0] result_cnt;

  wire [ACC_W-1:0] res_tdata;
  wire             res_tvalid;
  reg              res_tready;
  wire             res_tlast;

  conv_sa #(
      .IMG_W   (IMG_W),
      .IMG_H   (IMG_H),
      .K       (K),
      .DW      (DW),
      .ACC_W   (ACC_W),
      .SA_SIZE (SA_SZ)
  ) dut (
      .clk                 (clk),
      .rst_n               (rst_n),
      .s_axis_pix_tdata    (pix_tdata),
      .s_axis_pix_tvalid   (pix_tvalid),
      .s_axis_pix_tready   (pix_tready),
      .s_axis_pix_tlast    (pix_tlast),
      .s_axis_pix_tuser    (pix_tuser),
      .s_axis_kernel_tdata (kernel_tdata),
      .s_axis_kernel_tvalid(kernel_tvalid),
      .s_axis_kernel_tready(kernel_tready),
      .m_axis_res_tdata    (res_tdata),
      .m_axis_res_tvalid   (res_tvalid),
      .m_axis_res_tready   (res_tready),
      .m_axis_res_tlast    (res_tlast)
  );

  always #1 clk = ~clk;

  // Kernel driver: always block with NBAs
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      kernel_idx <= 0;
      kernel_tvalid <= 0;
    end else begin
      if (kernel_idx < 4) begin
        kernel_tvalid <= 1;
        kernel_tlast <= (kernel_idx == 3);
        case (kernel_idx)
          0: kernel_tdata <= 1;
          1: kernel_tdata <= 0;
          2: kernel_tdata <= 0;
          3: kernel_tdata <= 1;
        endcase
        kernel_idx <= kernel_idx + 1;
      end else begin
        kernel_tvalid <= 0;
      end
    end
  end

  // Pixel values (4x4, row-major)
  function [DW-1:0] pix_val(input [4:0] i);
    case (i)
      0:  return 1;
      1:  return 2;
      2:  return 3;
      3:  return 4;
      4:  return 5;
      5:  return 6;
      6:  return 7;
      7:  return 8;
      8:  return 9;
      9:  return 8;
      10: return 7;
      11: return 6;
      12: return 5;
      13: return 4;
      14: return 3;
      15: return 2;
      default: return 0;
    endcase
  endfunction

  // Pixel driver: holds values until tready
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      pix_idx <= 0;
      pix_tvalid <= 0;
      pix_tuser <= 0;
    end else begin
      if (!pix_tvalid || (pix_tvalid && pix_tready)) begin
        if (pix_idx < 16) begin
          pix_tdata <= pix_val(pix_idx);
          pix_tvalid <= 1;
          pix_tlast <= (pix_idx % 4 == 3);
          pix_tuser <= (pix_idx == 0);
          pix_idx <= pix_idx + 1;
        end else begin
          pix_tvalid <= 0;
          pix_tuser <= 0;
        end
      end
    end
  end

  initial begin
    $display("=== conv_sa test ===");

    clk    = 0;
    rst_n  = 0;
    kernel_idx = 0;
    pix_idx = 0;
    result_cnt = 0;
    res_tready   = 1;

    #4 rst_n = 1;
    $display("reset done");

    // Wait for results
    repeat (200) @(posedge clk) begin
      if (res_tvalid) begin
        $display("GOT result[%0d]: %0d", result_cnt, res_tdata);
        result_cnt = result_cnt + 1;
        if (result_cnt == 9) begin
          $display("PASS");
          $finish();
        end
      end
    end
    $display("TIMEOUT - no result seen");
    $finish();
  end

endmodule
