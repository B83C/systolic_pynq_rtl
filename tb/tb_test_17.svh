task automatic test_17_deadlock_stress();
  $display("=== TEST 17: Deadlock stress ===");

  // [1] LOAD_A with no data — stuck, soft_rst recovers
  $display("  [1] LOAD_A no-data → soft_rst recovery");
  reset_test();
  axil_write(5'h10, 0);  // trigger LOAD_A, but never send data
  repeat (10) @(posedge clk);
  if (dut.state != 2'd1) begin
    $display("  FAIL[1a]: state=%0d expected LOAD_A", dut.state); errors++;
  end
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != 2'd0 && dut.state != 2'd2) begin
    $display("  FAIL[1b]: soft_rst didn't recover, state=%0d", dut.state); errors++;
  end else $display("    recovered to state=%0d", dut.state);

  // [2] LOAD_C with no data — stuck, soft_rst recovers
  $display("  [2] LOAD_C no-data → soft_rst recovery");
  reset_test();
  axil_write(5'h08, 0);
  repeat (10) @(posedge clk);
  if (dut.state != 2'd3) begin
    $display("  FAIL[2a]: state=%0d expected LOAD_C", dut.state); errors++;
  end
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != 2'd0 && dut.state != 2'd2) begin
    $display("  FAIL[2b]: soft_rst didn't recover, state=%0d", dut.state); errors++;
  end else $display("    recovered to state=%0d", dut.state);

  // [3] Soft_rst during active output
  $display("  [3] soft_rst during output phase");
  reset_test();
  m_axis_tready = 1;
  axil_write(5'h14, 1); axil_write(5'h0C, 0);
  load_A();
  stream_mat(B1, 0);  // start output, leave LOAD_B
  repeat (5) @(posedge clk);
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  if (dut.state != 2'd0 && dut.state != 2'd2) begin
    $display("  FAIL[3]: state=%0d after output soft_rst", dut.state); errors++;
  end else $display("    state=%0d after soft_rst", dut.state);

  // [4] Clean compute after all stress
  $display("  [4] Clean compute after stress");
  reset_test(); errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(5'h14, 1); axil_write(5'h0C, 0);
  load_A();
  stream_mat(B1, 1);
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, exp_B1[r]);

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
