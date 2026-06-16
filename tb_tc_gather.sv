`timescale 1ns / 1ps
// Smoke-test for tiled_to_chlast_gather_sv
module tb_tc_gather;
  localparam DW = 8, CPB = 8, CH = 32, OC = 8, MAX_CH = 64;

  logic clk = 0; always #5 clk = ~clk;
  logic rst_n;
  logic [OC*DW-1:0] s_data;
  logic s_valid, s_ready, s_last;
  logic [CPB*DW-1:0] m_data;
  logic m_valid, m_ready, m_last;
  int out_count = 0, tlast_count = 0, errors = 0;

  logic c_awvalid, c_awready, c_wvalid, c_wready, c_bvalid, c_bready;
  logic c_arvalid, c_arready, c_rvalid, c_rready;
  logic [3:0] c_awaddr, c_araddr;
  logic [31:0] c_wdata, c_rdata;

  tiled_to_chlast_gather_sv #(
    .DATA_WIDTH(DW), .CH_PER_BEAT(CPB), .MAX_CHANNELS(MAX_CH), .OUT_COL(OC)
  ) dut (
    .clk, .rst_n,
    .s_axis_tdata(s_data), .s_axis_tvalid(s_valid),
    .s_axis_tready(s_ready), .s_axis_tlast(s_last),
    .m_axis_tdata(m_data), .m_axis_tvalid(m_valid),
    .m_axis_tready(m_ready), .m_axis_tlast(m_last),
    .s_axil_awvalid(c_awvalid), .s_axil_awready(c_awready),
    .s_axil_awaddr(c_awaddr), .s_axil_wdata(c_wdata),
    .s_axil_wvalid(c_wvalid), .s_axil_wready(c_wready),
    .s_axil_bresp(), .s_axil_bvalid(c_bvalid), .s_axil_bready(c_bready),
    .s_axil_arvalid(c_arvalid), .s_axil_arready(c_arready),
    .s_axil_araddr(c_araddr), .s_axil_rdata(c_rdata),
    .s_axil_rresp(), .s_axil_rvalid(c_rvalid), .s_axil_rready(c_rready)
  );

  always @(posedge clk)
    if (m_valid && m_ready) begin
      out_count <= out_count + 1;
      if (m_last) tlast_count <= tlast_count + 1;
    end

  initial begin
    $dumpfile("waveform.fst"); $dumpvars(0, tb_tc_gather);
    rst_n = 0; s_valid = 0; s_last = 0; m_ready = 1;
    c_awvalid = 0; c_wvalid = 0; c_bready = 0;
    c_arvalid = 0; c_rready = 0;
    repeat (5) @(posedge clk); rst_n = 1;

    // Write cfg_channels=32 via AXI-Lite
    @(posedge clk);
    c_awvalid = 1; c_awaddr = 0; c_wdata = 32; c_wvalid = 1;
    @(posedge clk); c_awvalid = 0; c_wvalid = 0;
    @(posedge clk); c_bready = 1;
    @(posedge clk); c_bready = 0;
    @(posedge clk);

    for (int i = 0; i < 32; i++) begin
      @(negedge clk);
      for (int c = 0; c < OC; c++) s_data[c*DW+:DW] = 8'(i + c);
      s_valid = 1; s_last = (i == 31);
      while (!s_ready) @(posedge clk);
      @(negedge clk); s_valid = 0;
    end
    s_last = 0;

    repeat (200) @(posedge clk);
    $display("out_count=%0d tlast=%0d (expect 32, 1)", out_count, tlast_count);
    if (out_count != 32 || tlast_count != 1) begin $display("FAIL"); errors++; end
    else $display("ALL PASSED");
    $finish;
  end
endmodule
