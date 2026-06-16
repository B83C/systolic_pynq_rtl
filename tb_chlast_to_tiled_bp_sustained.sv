`timescale 1ns / 1ps

// Sustained backpressure test for chlast_to_tiled:
//   1. Send 64 input beats
//   2. Throughout output replay, m_ready=1 every 4 cycles
//   3. Verify final out_count=32 and tlast_count=1

module tb_chlast_to_tiled_bp_sustained;

  localparam DW = 8, CH = 32, OC = 8, CH_PER_BEAT = 4;
  localparam REP_CNT = 1;
  localparam N_IN = (CH / CH_PER_BEAT) * OC;

  logic clk, rst_n;
  logic [CH_PER_BEAT*DW-1:0] s_data;
  logic             s_valid, s_ready, s_last;
  logic [OC*DW-1:0] m_data;
  logic             m_valid, m_ready, m_last;
  logic             bypass;

  // AXI-Lite
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
      .MAX_CHANNELS(CH),
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

  task ct_axil_write(input [3:0] addr, input [31:0] data);
    @(posedge clk);
    ct_awvalid = 1; ct_awaddr = addr;
    ct_wdata = data; ct_wvalid = 1;
    @(posedge clk);
    while (!ct_bvalid) @(posedge clk);
    ct_awvalid = 0; ct_wvalid = 0; ct_bready = 1;
    @(posedge clk);
    ct_bready = 0;
  endtask

  int out_count, tlast_count, errors;

  // Pattern: m_ready=1 for 1 cycle, then m_ready=0 for 3 cycles
  always @(posedge clk) begin
    if (m_valid && m_ready) begin
      out_count++;
      if (m_last) tlast_count++;
    end
    if (out_count > 0 && (out_count % 1) == 0) begin
      static int deny_cnt = 0;
      if (deny_cnt > 0) begin
        m_ready = 0;
        deny_cnt--;
      end else begin
        m_ready = 1;
        deny_cnt = 3;
      end
    end
  end

  initial begin
    $dumpfile("waveform.fst"); $dumpvars(0, tb_chlast_to_tiled_bp_sustained);
    clk = 0; rst_n = 0;
    s_valid = 0; s_last = 0;
    m_ready = 1; ct_awvalid = 0; ct_wvalid = 0; ct_bready = 0;
    ct_arvalid = 0; ct_rready = 0;
    bypass = 0;
    out_count = 0; tlast_count = 0; errors = 0;
    repeat (5) @(posedge clk); rst_n = 1; @(posedge clk);
    ct_axil_write(CT_REG_CH, CH);
    ct_axil_write(CT_REG_RPT, REP_CNT);
    ct_axil_write(CT_REG_BYPASS, 0);

    for (int ch = 0; ch < N_IN; ch++) begin
      @(negedge clk);
      for (int c = 0; c < CH_PER_BEAT; c++)
        s_data[c*DW+:DW] = 8'((ch * CH_PER_BEAT + c) & 8'hFF);
      s_valid = 1;
      s_last  = (ch == N_IN - 1);
      while (!s_ready) @(posedge clk);
      @(negedge clk);
      s_valid = 0;
    end
    s_last = 0;

    repeat (500) @(posedge clk);

    $display("t=%0t out_count=%0d tlast_count=%0d (expect %0d, %0d)",
             $time, out_count, tlast_count, CH*REP_CNT, REP_CNT);
    if (out_count != CH*REP_CNT) begin
      $display("  FAIL: out_count != %0d — data was eaten under sustained bp", CH*REP_CNT); errors++;
    end
    if (tlast_count != REP_CNT) begin
      $display("  FAIL: tlast_count != %0d — tlast was lost", REP_CNT); errors++;
    end

    if (errors == 0) $display("\nALL TESTS PASSED");
    else $display("\n%0d ERRORS", errors);
    $finish;
  end

  initial #50000 begin $display("TIMEOUT"); $finish; end
  always #1 clk = ~clk;

endmodule
