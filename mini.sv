`timescale 1ns / 1ps
module mini;
  reg clk, rst_n;
  reg [7:0] pt, kt;
  reg pv, kv, pl, pu, rr;
  wire pr, kr, rv, rl;
  wire [31:0] rd;

  always #1 clk = ~clk;

  conv_sa #(.IMG_W(4),.IMG_H(4),.K(2),.DW(8),.ACC_W(32),.SA_SIZE(4)) dut(
    .clk(clk), .rst_n(rst_n),
    .s_axis_pix_tdata(pt), .s_axis_pix_tvalid(pv), .s_axis_pix_tready(pr),
    .s_axis_pix_tlast(pl), .s_axis_pix_tuser(pu),
    .s_axis_kernel_tdata(kt), .s_axis_kernel_tvalid(kv), .s_axis_kernel_tready(kr),
    .m_axis_res_tdata(rd), .m_axis_res_tvalid(rv), .m_axis_res_tready(rr), .m_axis_res_tlast(rl)
  );

  initial begin
    $display("mini start");
    clk=0; rst_n=0; pv=0; kv=0; rr=1; pt=0; kt=0; pl=0; pu=0;
    #4 rst_n=1;
    #10;
    $display("mini done");
    $finish();
  end
endmodule
