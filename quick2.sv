`timescale 1ns / 1ps
module quick2;
  reg clk;
  reg [7:0] cnt = 0;
  always #1 clk = ~clk;
  always_ff @(posedge clk) cnt <= cnt + 1;
  initial begin
    $display("start");
    clk = 0;
    #20;
    $display("cnt=%0d", cnt);
    #20;
    $display("cnt=%0d", cnt);
    $finish();
  end
endmodule
