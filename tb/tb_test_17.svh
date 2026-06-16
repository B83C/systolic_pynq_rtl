`ifndef TB_TEST_17_SVH
`define TB_TEST_17_SVH
`include "tb/tb_common.svh"
task automatic test_17_deadlock_stress();
  // state_t enum: LOAD_A=0, LOAD_B=1, LOAD_C=2.
  int S_LOAD_A  = 32'd0;
  int S_LOAD_B  = 32'd1;
  int S_LOAD_C  = 32'd2;

  $display("=== TEST 17: Deadlock stress ===");

  // [1] LOAD_A with no data — stuck, soft_rst recovers
  $display("  [1] LOAD_A no-data → soft_rst recovery");
  reset_test();
  axil_write(REG_A_LOAD, 0);  // trigger LOAD_A, but never send data
  repeat (10) @(posedge clk);
  if (dut.state != S_LOAD_A) begin
    $display("  FAIL[1a]: state=%0d expected LOAD_A", dut.state); errors++;
  end
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != S_LOAD_A && dut.state != S_LOAD_B) begin
    $display("  FAIL[1b]: soft_rst didn't recover, state=%0d", dut.state); errors++;
  end else $display("    recovered to state=%0d", dut.state);

  // [2] LOAD_C with no data — stuck, soft_rst recovers
  $display("  [2] LOAD_C no-data → soft_rst recovery");
  reset_test();
  axil_write(REG_C_LOAD, 0);
  repeat (10) @(posedge clk);
  if (dut.state != S_LOAD_C) begin
    $display("  FAIL[2a]: state=%0d expected LOAD_C", dut.state); errors++;
  end
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != S_LOAD_A && dut.state != S_LOAD_B) begin
    $display("  FAIL[2b]: soft_rst didn't recover, state=%0d", dut.state); errors++;
  end else $display("    recovered to state=%0d", dut.state);

  // [3] Soft_rst during active output
  $display("  [3] soft_rst during output phase");
  reset_test();
  m_axis_tready = 1;
  axil_write(REG_ACC_CNT, 1); axil_write(REG_FB_CNT, 0);
  load_A();
  stream_mat(B1, 0);  // start output, leave LOAD_B
  repeat (5) @(posedge clk);
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != S_LOAD_A && dut.state != S_LOAD_B) begin
    $display("  FAIL[3]: state=%0d after output soft_rst", dut.state); errors++;
  end else $display("    state=%0d after soft_rst", dut.state);

  // [4] Clean compute after all stress
  $display("  [4] Clean compute after stress");
  reset_test(); errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_ACC_CNT, 1); axil_write(REG_FB_CNT, 0);
  load_A();
  stream_mat(B1, 1);
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, exp_B1[r]);

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
`endif
