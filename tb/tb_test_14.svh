task automatic test_14_negative();
  $display("=== TEST 14: Signed multiply — negative B ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(5'h18, 0);
  axil_write(5'h0C, 0);
  axil_write(5'h14, 1);
  load_A();

  $display("  Expected row 0: %0d %0d %0d %0d",
           exp_neg[0][0], exp_neg[0][1], exp_neg[0][2], exp_neg[0][3]);

  stream_mat(Bneg, 1);
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);

  $display("  captured %0d outputs", out_count);
  for (int r = 0; r < SIZE; r++) begin
    $write("  row %0d: ", r);
    for (int c = 0; c < SIZE; c++) $write(" %0d", result[r][c]);
    $write("\n");
    check_row_str($sformatf("row %0d", r), r, exp_neg[r]);
  end

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
