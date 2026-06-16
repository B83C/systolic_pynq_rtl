`timescale 1ns / 1ps

// Comprehensive testbench for tiled_to_chlast:
//   TEST 1: output backpressure (m_axis_tready stalls)
//   TEST 2: input backpressure  (s_axis_tready stalls)
//   TEST 3: bypass passthrough
//   TEST 4: sequential back-to-back frames

module tb_tiled_to_chlast_full;

  localparam DW = 8, CH = 32, OC = 8;

  logic clk, rst_n;
  logic [OC*DW-1:0] s_data, m_data;
  logic             s_valid, s_ready, s_last, m_valid, m_ready, m_last;
  logic             bypass;
  // bypass signal now driven by TC_REG_BYPASS via AXI-Lite; the local
  // logic here is a convenience handle for tests.
  wire _unused = &{1'b0, bypass, 1'b0};
  // AXI-Lite for tiled_to_chlast
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

  // Readback via AXI-Lite
  task tc_axil_read(input [3:0] addr, output [31:0] data);
    @(posedge clk);
    tc_arvalid = 1; tc_araddr = addr;
    @(posedge clk);
    while (!tc_rvalid) @(posedge clk);
    data = tc_rdata;
    tc_arvalid = 0; tc_rready = 1;
    @(posedge clk);
    tc_rready = 0;
  endtask

  logic [31:0] captured_data;

  // Drive bypass via AXI-Lite (reg TC_REG_BYPASS) instead of a port
  assign bypass = dut.bypass_r;

  always #1 clk = ~clk;

  int out_count, tlast_count, errors;

  always @(posedge clk)
    if (m_valid && m_ready) begin
      out_count++;
      if (m_last) tlast_count++;
    end

  task send_frame(input int base, input int n_ch);
    for (int ch = 0; ch < n_ch; ch++) begin
      @(negedge clk);
      for (int c = 0; c < OC; c++)
        s_data[c*DW+:DW] = 8'(base + ch * CH + c);
      s_valid = 1;
      s_last  = (ch == n_ch - 1);
      while (!s_ready) @(posedge clk);
      @(negedge clk);
      s_valid = 0;
    end
    s_last = 0;
  endtask

  task send_frame_bp_in(input int base, input int n_ch);
    for (int ch = 0; ch < n_ch; ch++) begin
      @(negedge clk);
      for (int c = 0; c < OC; c++)
        s_data[c*DW+:DW] = 8'(base + ch * CH + c);
      s_valid = 1;
      s_last  = (ch == n_ch - 1);
      if (ch % 5 == 0) repeat (3) @(posedge clk);
      while (!s_ready) @(posedge clk);
      @(negedge clk);
      s_valid = 0;
    end
    s_last = 0;
  endtask

  initial begin
    $dumpfile("waveform.fst"); $dumpvars(0, tb_tiled_to_chlast_full);
    clk = 0; rst_n = 0;
    s_valid = 0; s_last = 0;
    m_ready = 1; tc_awvalid = 0; tc_wvalid = 0; tc_bready = 0;
    tc_arvalid = 0; tc_rready = 0;
    out_count = 0; tlast_count = 0; errors = 0;
    repeat (5) @(posedge clk); rst_n = 1; @(posedge clk);
    // Push initial config
    tc_axil_write(4'h0, CH);

    // TEST 1: output backpressure
    $display("=== TEST 1: output backpressure ===");
    out_count = 0; tlast_count = 0;
    m_ready = 1;
    send_frame(0, CH);

    // Wait until m_valid is high; the always block will count a beat this
    // same posedge. Deassert m_ready at the next negedge to capture stall
    // after exactly 1 beat is observed.
    while (m_valid == 0) @(posedge clk);
    @(negedge clk);
    m_ready = 0;
    repeat (20) @(posedge clk);
    if (out_count != 1) begin
      $display("  FAIL: out_count=%0d after stall (expect 1)", out_count);
      errors++;
    end else $display("  out_count=%0d (stalled correctly)", out_count);

    @(negedge clk);
    m_ready = 1;
    repeat (200) @(posedge clk);
    $display("  output beats: %0d  tlast: %0d  (expect %0d, 1)",
             out_count, tlast_count, CH);
    if (out_count != CH || tlast_count != 1) begin
      $display("  FAIL"); errors++;
    end
    out_count = 0; tlast_count = 0;

    // TEST 2: input backpressure
    $display("=== TEST 2: input backpressure ===");
    m_ready = 1;
    send_frame_bp_in(0, CH);
    repeat (200) @(posedge clk);
    $display("  output beats: %0d  tlast: %0d  (expect %0d, 1)",
             out_count, tlast_count, CH);
    if (out_count != CH || tlast_count != 1) begin
      $display("  FAIL"); errors++;
    end
    out_count = 0; tlast_count = 0;

    // TEST 3: bypass
    $display("=== TEST 3: bypass passthrough ===");
    // Verify bypass readback (should be 0 before write)
    tc_axil_read(TC_REG_BYPASS, captured_data);
    $display("  bypass_r before write: %0d (expect 0)", captured_data);
    tc_axil_write(TC_REG_BYPASS, 1);
    tc_axil_read(TC_REG_BYPASS, captured_data);
    $display("  bypass_r after write 1: %0d (expect 1)", captured_data);
    m_ready = 1;

    @(negedge clk);
    s_data = 64'hDEADBEEFCAFEBABE;
    s_valid = 1; s_last = 1;
    @(posedge clk);
    if (m_valid !== 1'b1) begin
      $display("  FAIL: m_valid low in bypass"); errors++;
    end else if (m_data !== 64'hDEADBEEFCAFEBABE) begin
      $display("  FAIL: m_data=%h expected DEADBEEFCAFEBABE", m_data); errors++;
    end else $display("  bypass OK (data=%h, last=%b)", m_data, m_last);
    s_valid = 0; s_last = 0;
    tc_axil_write(TC_REG_BYPASS, 0);
    repeat (20) @(posedge clk);

    // TEST 4: sequential back-to-back
    $display("=== TEST 4: sequential back-to-back (3 frames) ===");
    out_count = 0; tlast_count = 0;
    m_ready = 1;

    for (int f = 0; f < 3; f++)
      for (int ch = 0; ch < CH; ch++) begin
        @(negedge clk);
        for (int c = 0; c < OC; c++)
          s_data[c*DW+:DW] = 8'(f*1000 + ch * CH + c);
        s_valid = 1;
        s_last  = (ch == CH - 1);
        while (!s_ready) @(posedge clk);
      end
    @(negedge clk);
    s_valid = 0; s_last = 0;

    repeat (300) @(posedge clk);
    $display("  output beats: %0d  tlast: %0d  (expect %0d, %0d)",
             out_count, tlast_count, 3*CH, 3);
    if (out_count != 3*CH || tlast_count != 3) begin
      $display("  FAIL"); errors++;
    end
    out_count = 0; tlast_count = 0;

    if (errors == 0) $display("\nALL TESTS PASSED");
    else $display("\n%0d ERRORS", errors);
    $finish;
  end

  initial #500000 begin $display("TIMEOUT"); $finish; end

endmodule
