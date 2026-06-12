task automatic test_15_reload_a_from_loadb();
  $display("=== TEST 15: Trigger A_LOAD while in LOAD_B state ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;

  // First compute: A×B1, leave DUT in LOAD_B
  load_A();
  stream_mat(B1, 0);  // tlast=0 → stay in LOAD_B
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  $display("  first compute: %0d outputs", out_count);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("first row %0d", r), r, exp_B1[r]);

  // Now state is LOAD_B. Trigger A_LOAD → should go to LOAD_A, load new A
  $display("  calling load_A() from LOAD_B...");
  load_A();

  // Stream B1 again with tlast
  stream_mat(B1, 1);
  while (out_count < 2 * SIZE) @(posedge clk);
  @(posedge clk);
  $display("  second compute at indices %0d..%0d", SIZE, 2*SIZE-1);
  for (int r = 0; r < SIZE && (SIZE + r) < out_count; r++)
    check_row_str($sformatf("second row %0d", r), SIZE + r, exp_B1[r]);

  if (out_count > 0 && !result_tlast[out_count-1]) begin
    $display("  MISMATCH: last output expected tlast=1");
    errors++;
  end

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
