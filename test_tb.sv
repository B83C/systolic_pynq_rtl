`timescale 1ns / 1ps
module test_tb;
  logic clk, rst_n;
  logic [31:0] s_axis_A_tdata;
  logic s_axis_A_tvalid, s_axis_A_tready, s_axis_A_tlast;
  logic [31:0] s_axis_B_tdata;
  logic s_axis_B_tvalid, s_axis_B_tready, s_axis_B_tlast;
  logic [127:0] m_axis_tdata;
  logic m_axis_tvalid, m_axis_tready, m_axis_tlast;

  logic s_axil_awvalid, s_axil_awready;
  logic [3:0] s_axil_awaddr;
  logic [31:0] s_axil_wdata;
  logic s_axil_wvalid, s_axil_wready;
  logic [1:0] s_axil_bresp;
  logic s_axil_bvalid, s_axil_bready;
  logic s_axil_arvalid, s_axil_arready;
  logic [3:0] s_axil_araddr;
  logic [31:0] s_axil_rdata;
  logic [1:0] s_axil_rresp;
  logic s_axil_rvalid, s_axil_rready;

  sa_wrapper_axi_ctrl_sv #(.SIZE(4)) dut (
      .clk(clk), .rst_n(rst_n),
      .s_axis_A_tdata(s_axis_A_tdata), .s_axis_A_tvalid(s_axis_A_tvalid),
      .s_axis_A_tready(s_axis_A_tready), .s_axis_A_tlast(s_axis_A_tlast),
      .s_axis_B_tdata(s_axis_B_tdata), .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready), .s_axis_B_tlast(s_axis_B_tlast),
      .m_axis_tdata(m_axis_tdata), .m_axis_tvalid(m_axis_tvalid),
      .m_axis_tready(m_axis_tready), .m_axis_tlast(m_axis_tlast),
      .s_axil_awvalid(s_axil_awvalid), .s_axil_awready(s_axil_awready),
      .s_axil_awaddr(s_axil_awaddr), .s_axil_wdata(s_axil_wdata),
      .s_axil_wvalid(s_axil_wvalid), .s_axil_wready(s_axil_wready),
      .s_axil_bresp(s_axil_bresp), .s_axil_bvalid(s_axil_bvalid),
      .s_axil_bready(s_axil_bready),
      .s_axil_arvalid(s_axil_arvalid), .s_axil_arready(s_axil_arready),
      .s_axil_araddr(s_axil_araddr), .s_axil_rdata(s_axil_rdata),
      .s_axil_rresp(s_axil_rresp), .s_axil_rvalid(s_axil_rvalid),
      .s_axil_rready(s_axil_rready)
  );

  always #1 clk = ~clk;

  initial begin
    clk = 0; rst_n = 0;
    s_axis_A_tvalid = 0; s_axis_B_tvalid = 0;
    s_axis_A_tlast = 0; s_axis_B_tlast = 0;
    s_axis_A_tdata = 0; s_axis_B_tdata = 0;
    s_axil_awvalid = 0; s_axil_wvalid = 0;
    s_axil_arvalid = 0; s_axil_bready = 0; s_axil_rready = 0;
    m_axis_tready = 1;

    $display("BEFORE RESET");
    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    $display("AFTER RESET, testing AXI-Lite write...");

    // AXI-Lite write
    @(posedge clk);
    s_axil_awvalid = 1;
    s_axil_awaddr = 0;
    s_axil_wdata = 32'h1;
    s_axil_wvalid = 1;
    @(posedge clk);
    while (!s_axil_bvalid) @(posedge clk);
    $display("AXI-Lite write complete, bvalid seen");
    s_axil_awvalid = 0;
    s_axil_wvalid = 0;
    s_axil_bready = 1;
    @(posedge clk);
    s_axil_bready = 0;

    $display("DONE, finishing");
    @(posedge clk);
    @(posedge clk);
    $finish;
  end
endmodule
