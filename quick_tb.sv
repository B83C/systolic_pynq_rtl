`timescale 1ns / 1ps
module quick_tb;
  reg clk;
  always #1 clk = ~clk;

  initial begin
    $display("hello from quick_tb");
    clk = 0;
    #10;
    $display("after 10ns");
    #10;
    $display("after 20ns");
    $finish();
  end
endmodule
