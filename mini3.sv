`timescale 1ns / 1ps
module mini3;
  reg clk;

  always #1 clk = ~clk;

  initial begin
    $display("mini3 start");
    clk = 0;
    $display("a"); #2;
    $display("b"); #2;
    $display("c"); #2;
    $display("d"); #2;
    $display("e"); #2;
    $display("f"); #2;
    $display("g"); #2;
    $display("h"); #2;
    $display("i"); #2;
    $display("j"); #2;
    $display("k"); #2;
    $display("l"); #2;
    $display("m"); #2;
    $display("n"); #2;
    $display("o"); #2;
    $display("p"); #2;
    $display("q"); #2;
    $display("r"); #2;
    $display("s"); #2;
    $display("t"); #2;
    $display("u"); #2;
    $display("v"); #2;
    $display("w"); #2;
    $display("x"); #2;
    $display("y"); #2;
    $display("z"); #2;
    $display("mini3 done");
    $finish();
  end
endmodule
