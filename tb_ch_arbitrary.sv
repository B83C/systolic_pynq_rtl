`timescale 1ns / 1ps

module tb_ch_arbitrary;

  localparam DW = 8, CH_PER_BEAT = 8, MAX_CH = 64, OC = 8;

  logic clk, rst_n;
  logic [CH_PER_BEAT*DW-1:0] s_data;
  logic             s_valid, s_ready, s_last;
  logic [OC*DW-1:0] m_data;
  logic             m_valid, m_ready, m_last;

  logic        ct_awvalid;
  wire         ct_awready;
  logic [ 3:0] ct_awaddr;
  logic [31:0] ct_wdata;
  logic        ct_wvalid;
  wire         ct_wready;
  wire         ct_bvalid;
  logic        ct_bready;
  logic        ct_arvalid;
  wire         ct_arready;
  logic [ 3:0] ct_araddr;
  wire [31:0] ct_rdata;
  wire         ct_rvalid;
  logic        ct_rready;
  localparam CT_REG_CH      = 4'h0;
  localparam CT_REG_RPT     = 4'h4;
  localparam CT_REG_BYPASS  = 4'h8;

  chlast_to_tiled_sv #(
      .DATA_WIDTH(DW),
      .CH_PER_BEAT(CH_PER_BEAT),
      .MAX_CHANNELS(MAX_CH),
      .OUT_COL(OC),
      .MAX_REPLAY_CNT(16)
  ) dut (
      .clk, .rst_n,
      .s_axis_tdata(s_data), .s_axis_tvalid(s_valid),
      .s_axis_tready(s_ready), .s_axis_tlast(s_last),
      .m_axis_tdata(m_data), .m_axis_tvalid(m_valid),
      .m_axis_tready(m_ready), .m_axis_tlast(m_last),
      .s_axil_awvalid(ct_awvalid), .s_axil_awready(ct_awready),
      .s_axil_awaddr(ct_awaddr),
      .s_axil_wdata(ct_wdata), .s_axil_wvalid(ct_wvalid), .s_axil_wready(ct_wready),
      .s_axil_bresp(), .s_axil_bvalid(ct_bvalid), .s_axil_bready(ct_bready),
      .s_axil_arvalid(ct_arvalid), .s_axil_arready(ct_arready),
      .s_axil_araddr(ct_araddr),
      .s_axil_rdata(ct_rdata), .s_axil_rresp(),
      .s_axil_rvalid(ct_rvalid), .s_axil_rready(ct_rready)
  );

  int out_cnt, tlast_cnt;

  always @(posedge clk) begin
    if (m_valid && m_ready) begin
      out_cnt <= out_cnt + 1;
      if (m_last) tlast_cnt <= tlast_cnt + 1;
    end
  end

  task ct_write(input [3:0] addr, input [31:0] data);
    @(posedge clk);
    ct_awvalid = 1; ct_awaddr = addr;
    ct_wdata = data; ct_wvalid = 1;
    @(posedge clk);
    while (!ct_bvalid) @(posedge clk);
    ct_awvalid = 0; ct_wvalid = 0; ct_bready = 1;
    @(posedge clk);
    ct_bready = 0;
  endtask

  task run_test(input int cfg_ch, input int n_beats, ref int nok);
    $display("  cfg_channels=%0d (CH_PER_BEAT=%0d) -> send %0d beat(s)", cfg_ch, CH_PER_BEAT, n_beats);

    ct_write(CT_REG_CH, cfg_ch);
    ct_write(CT_REG_RPT, 1);
    ct_write(CT_REG_BYPASS, 0);

    out_cnt = 0; tlast_cnt = 0;

    m_ready = 1;
    for (int b = 0; b < n_beats; b++) begin
      @(negedge clk);
      for (int c = 0; c < CH_PER_BEAT; c++)
        s_data[c*DW+:DW] = 8'((b * CH_PER_BEAT + c) & 8'hFF);
      s_valid = 1;
      s_last  = (b == n_beats - 1);
      while (!s_ready) @(posedge clk);
      @(negedge clk);
      s_valid = 0;
    end
    s_last = 0;

    repeat (1000) @(posedge clk);
    m_ready = 0;

    $display("    out_count=%0d tlast=%0d  (expect %0d, 1)", out_cnt, tlast_cnt, cfg_ch);
    if (out_cnt != cfg_ch) begin
      $display("    FAIL: expected %0d beats, got %0d", cfg_ch, out_cnt); nok++;
    end
    if (tlast_cnt != 1) begin
      $display("    FAIL: expected 1 tlast, got %0d", tlast_cnt); nok++;
    end
  endtask

  int errors;
  initial begin
    $dumpfile("waveform.fst"); $dumpvars(0, tb_ch_arbitrary);
    clk = 0; rst_n = 0;
    s_valid = 0; s_last = 0; m_ready = 0;
    ct_awvalid = 0; ct_wvalid = 0; ct_bready = 0;
    ct_arvalid = 0; ct_rready = 0;
    errors = 0;
    repeat (5) @(posedge clk); rst_n = 1; @(posedge clk);

    $display("=== chlast_to_tiled: arbitrary channel sizes ===");

    run_test(8,   (8  / CH_PER_BEAT) * OC, errors);
    run_test(32,  (32 / CH_PER_BEAT) * OC, errors);
    run_test(3,   ((3 + CH_PER_BEAT - 1) / CH_PER_BEAT) * OC, errors);
    run_test(7,   ((7 + CH_PER_BEAT - 1) / CH_PER_BEAT) * OC, errors);
    run_test(9,   ((9 + CH_PER_BEAT - 1) / CH_PER_BEAT) * OC, errors);
    run_test(1,   ((1 + CH_PER_BEAT - 1) / CH_PER_BEAT) * OC, errors);

    if (errors == 0) $display("\nPASS: all tests passed");
    else $display("\nFAIL: %0d errors", errors);
    $finish;
  end

  initial #100000 begin $display("TIMEOUT"); $finish; end
  always #1 clk = ~clk;

endmodule
