`timescale 1ns / 1ps
module quick3;
  reg clk, rst_n;
  reg [7:0] pix_tdata, kernel_tdata;
  reg pix_tvalid, kernel_tvalid, pix_tlast, pix_tuser, res_tready;
  wire pix_tready, kernel_tready;
  wire [31:0] res_tdata;
  wire res_tvalid, res_tlast;

  always #1 clk = ~clk;

  conv_sa #(
      .IMG_W(4), .IMG_H(4), .K(2), .DW(8), .ACC_W(32), .SA_SIZE(4)
  ) dut (.*);

  initial begin
    $display("quick3 start");
    clk = 0; rst_n = 0;
    pix_tvalid = 0; kernel_tvalid = 0; res_tready = 1;
    pix_tdata = 0; kernel_tdata = 0;
    pix_tlast = 0; pix_tuser = 0;
    #4 rst_n = 1;
    #10;
    $display("quick3 done at %0t", $time);
    $finish();
  end
endmodule
