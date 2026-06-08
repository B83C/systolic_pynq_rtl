`timescale 1ns / 1ps
module quick;
  reg clk = 0;
  always #1 clk = ~clk;
  initial begin
    $write("hello\n");
    #4;
    $write("after 4ns\n");
    #10;
    $write("after 14ns\n");
    $finish();
  end
endmodule
