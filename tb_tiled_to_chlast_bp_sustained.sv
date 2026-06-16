`timescale 1ns / 1ps

// Sustained backpressure test for tiled_to_chlast:
//   1. Send 32 input beats with tlast on the last
//   2. Throughout the output replay, m_ready is asserted only 1 of every 4
//      cycles (heavy sustained backpressure)
//   3. Verify final out_count=32 and tlast_count=1

module tb_tiled_to_chlast_bp_sustained;

  localparam DW = 8, CH = 32, OC = 8;

  logic clk, rst_n;
  logic [OC*DW-1:0] s_data, m_data;
  logic             s_valid, s_ready, s_last, m_valid, m_ready, m_last;

  // AXI-Lite
  logic        tc_awvalid;
  wire         tc_awready;
  logic [ 3:0] tc_awaddr;
  logic [31:0] tc_wdata;
  logic        tc_wvalid;
  wire         tc_wready;
  wire         tc_bvalid;
  logic        tc_bready;
  logic        tc_arvalid;
  wire         tc_arready;
  logic [ 3:0] tc_araddr;
  wire [31:0] tc_rdata;
  wire         tc_rvalid;
  logic        tc_rready;
  localparam TC_REG_CH      = 4'h0;
  localparam TC_REG_BYPASS = 4'h4;

  tiled_to_chlast_sv #(.DATA_WIDTH(DW), .MAX_CHANNELS(CH), .OUT_COL(OC)) dut (
      .clk, .rst_n,
      .s_axis_tdata(s_data), .s_axis_tvalid(s_valid),
      .s_axis_tready(s_ready), .s_axis_tlast(s_last),
      .m_axis_tdata(m_data), .m_axis_tvalid(m_valid),
      .m_axis_tready(m_ready), .m_axis_tlast(m_last),
      .s_axil_awvalid(tc_awvalid), .s_axil_awready(tc_awready),
      .s_axil_awaddr(tc_awaddr),
      .s_axil_wdata(tc_wdata), .s_axil_wvalid(tc_wvalid), .s_axil_wready(tc_wready),
      .s_axil_bresp(), .s_axil_bvalid(tc_bvalid), .s_axil_bready(tc_bready),
      .s_axil_arvalid(tc_arvalid), .s_axil_arready(tc_arready),
      .s_axil_araddr(tc_araddr),
      .s_axil_rdata(tc_rdata), .s_axil_rresp(),
      .s_axil_rvalid(tc_rvalid), .s_axil_rready(tc_rready)
  );

  task tc_axil_write(input [3:0] addr, input [31:0] data);
    @(posedge clk);
    tc_awvalid = 1; tc_awaddr = addr;
    tc_wdata = data; tc_wvalid = 1;
    @(posedge clk);
    while (!tc_bvalid) @(posedge clk);
    tc_awvalid = 0; tc_wvalid = 0; tc_bready = 1;
    @(posedge clk);
    tc_bready = 0;
  endtask

  int out_count, tlast_count, errors;

  // Pattern: m_ready=1 for 1 cycle, then m_ready=0 for 3 cycles (3/4 bp)
  always @(posedge clk) begin
    if (m_valid && m_ready) begin
      out_count++;
      if (m_last) tlast_count++;
    end
    // Toggle m_ready every cycle for sustained bp (after the first beat)
    if (out_count > 0 && (out_count % 1) == 0) begin
      // After every accepted beat, deny ready for 3 cycles
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
    $dumpfile("waveform.fst"); $dumpvars(0, tb_tiled_to_chlast_bp_sustained);
    clk = 0; rst_n = 0;
    s_valid = 0; s_last = 0;
    m_ready = 1; tc_awvalid = 0; tc_wvalid = 0; tc_bready = 0;
    tc_arvalid = 0; tc_rready = 0;
    out_count = 0; tlast_count = 0; errors = 0;
    repeat (5) @(posedge clk); rst_n = 1; @(posedge clk);
    tc_axil_write(TC_REG_CH, CH);

    // Send 32 input beats
    for (int ch = 0; ch < 32; ch++) begin
      @(negedge clk);
      for (int c = 0; c < OC; c++)
        s_data[c*DW+:DW] = 8'((ch * CH + c) & 8'hFF);
      s_valid = 1;
      s_last  = (ch == 31);
      while (!s_ready) @(posedge clk);
      @(negedge clk);
      s_valid = 0;
    end
    s_last = 0;

    // Wait for output to finish under sustained backpressure
    repeat (500) @(posedge clk);

    $display("t=%0t out_count=%0d tlast_count=%0d (expect 32, 1)",
             $time, out_count, tlast_count);
    if (out_count != 32) begin
      $display("  FAIL: out_count != 32 — data was eaten under sustained bp"); errors++;
    end
    if (tlast_count != 1) begin
      $display("  FAIL: tlast_count != 1 — tlast was lost"); errors++;
    end

    if (errors == 0) $display("\nALL TESTS PASSED");
    else $display("\n%0d ERRORS", errors);
    $finish;
  end

  initial #50000 begin $display("TIMEOUT"); $finish; end
  always #1 clk = ~clk;

endmodule
