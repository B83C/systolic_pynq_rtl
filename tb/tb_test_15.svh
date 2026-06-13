`ifndef TB_TEST_15_SVH
`define TB_TEST_15_SVH
`include "tb/tb_common.svh"
task automatic test_15_state_fuzz();
  $display("=== TEST 15: State machine fuzz ===");

  // Force state back to IDLE: stream tlast on LOAD_B
  if (dut.state == 2'd2) begin
    s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0;
    repeat (3) @(posedge clk);
  end
  // Drain LOAD_A: stream A with tlast
  if (dut.state == 2'd1) begin
    stream_mat(A, 1);
    repeat (3) @(posedge clk);
  end
  soft_rst_via_axil();
  repeat (5) @(posedge clk);

  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_ACC_OUT, 1);
  axil_write(REG_FB_CNT, 0);

  // [1] A_LOAD while tvalid=1 (DMA still sending B)
  $display("  [1] A_LOAD while tvalid=1 → blocked, released");
  load_A();
  s_axis_B_tvalid = 1;  @(posedge clk);
  axil_write(REG_A_LOAD, 0);
  repeat (3) @(posedge clk);
  if (dut.state != 2'd2) begin
    $display("  FAIL[1a]: state=%0d expected LOAD_B", dut.state); errors++;
  end else $display("    state stayed LOAD_B (correctly blocked)");
  s_axis_B_tvalid = 0;
  repeat (5) @(posedge clk);
  if (dut.state != 2'd1 && dut.state != 2'd0) begin
    $display("  FAIL[1b]: state stuck at %0d", dut.state); errors++;
  end else $display("    state transitioned to %0d", dut.state);

  // Reset to IDLE for next scenario
  if (dut.state == 2'd1) stream_mat(A, 1);
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  soft_rst_via_axil();  repeat (5) @(posedge clk);

  // [2] C_LOAD while tvalid=1
  $display("  [2] C_LOAD while tvalid=1 → blocked, released");
  load_A();
  s_axis_B_tvalid = 1;  @(posedge clk);
  axil_write(REG_C_LOAD, 0);
  repeat (3) @(posedge clk);
  if (dut.state != 2'd2) begin
    $display("  FAIL[2a]: state=%0d", dut.state); errors++;
  end else $display("    state stayed LOAD_B (correctly blocked)");
  s_axis_B_tvalid = 0;
  repeat (5) @(posedge clk);
  if (dut.state != 2'd3 && dut.state != 2'd0) begin
    $display("  FAIL[2b]: state=%0d", dut.state); errors++;
  end else $display("    state transitioned to %0d", dut.state);

  // Reset
  if (dut.state == 2'd1) stream_mat(A, 1);
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  // Drain LOAD_C if stuck
  if (dut.state == 2'd3) begin
    for (int i = 0; i < SIZE; i++) begin @(posedge clk); s_axis_B_tdata = 0; s_axis_B_tvalid = 1; s_axis_B_tlast = 0; end
    @(posedge clk); s_axis_B_tvalid = 0;
  end
  soft_rst_via_axil();  repeat (5) @(posedge clk);

  // Reset to IDLE
  soft_rst_via_axil();  repeat (5) @(posedge clk);
  // Force IDLE: tlast if in LOAD_B
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  repeat (5) @(posedge clk);

  // [3] Verify IDLE priority: C_LOAD wins over A_LOAD
  $display("  [3] IDLE priority: C_LOAD");
  soft_rst_via_axil();  repeat (5) @(posedge clk);
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  repeat (3) @(posedge clk);
  axil_write(REG_C_LOAD, 0);  // C_LOAD only
  repeat (3) @(posedge clk);
  if (dut.state != 2'd3) begin
    $display("  FAIL[3a]: state=%0d expected LOAD_C", dut.state); errors++;
  end else $display("    LOAD_C entered (C has priority)");
  // Drain LOAD_C
  for (int i = 0; i < SIZE; i++) begin @(posedge clk); s_axis_B_tdata = 0; s_axis_B_tvalid = 1; s_axis_B_tlast = 0; end
  @(posedge clk); s_axis_B_tvalid = 0;
  repeat (3) @(posedge clk);

  // [3b] A_LOAD from IDLE
  $display("  [3b] IDLE → A_LOAD");
  soft_rst_via_axil();  repeat (5) @(posedge clk);
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  repeat (3) @(posedge clk);
  axil_write(REG_A_LOAD, 0);
  repeat (3) @(posedge clk);
  if (dut.state != 2'd1) begin
    $display("  FAIL[3b]: state=%0d expected LOAD_A", dut.state); errors++;
  end else $display("    LOAD_A entered (A works)");
  stream_mat(A, 1);  // drain LOAD_A
  repeat (3) @(posedge clk);

  // [4] Full clean compute after all fuzz
  $display("  [4] Clean compute after fuzz");
  out_count = 0;
  load_A();
  stream_mat(B1, 1);
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, exp_B1[r]);
  $display("    outputs: %0d", out_count);

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
`endif
