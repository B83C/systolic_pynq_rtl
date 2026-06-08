`timescale 1ns / 1ps
module mini2;
  reg clk;
  reg [31:0] cnt;

  always #1 clk = ~clk;

  always_ff @(posedge clk) cnt <= cnt + 1;

  initial begin
    $display("mini2 start");
    clk = 0; cnt = 0;
    #4;
    $display("mini2 wait for cnt...");
    while (cnt < 10) #2;
    $display("mini2 cnt=%0d", cnt);
    $display("mini2 done");
    $finish();
  end
endmodule
