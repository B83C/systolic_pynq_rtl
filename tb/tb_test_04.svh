task test_04_accumulation_accout1();
  $display("=== TEST 4: FB_CNT=2, acc_out=1 (show all) ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(5'h18, 0);
  axil_write(5'h0C, 2);
  axil_write(5'h14, 1);
  load_A();

  stream_mat(B1, 0);
  stream_mat(B2, 0);
  stream_mat(B_eye, 1);

  wait_output_done();
  $display("  captured %0d outputs", out_count);

  for (int i = 0; i < out_count; i++) begin
    $write("  out[%0d] =", i);
    for (int c = 0; c < SIZE; c++) $write(" %0d", result[i][c]);
    $write(" tlast=%0d\n", result_tlast[i]);
  end

  $display("  Checking B1 (individual)");
  if (out_count >= SIZE) for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("B1 row %0d", r), r, exp_B1[r]);

  if (out_count >= 2*SIZE) begin
    $display("  Checking B1+B2 (accumulated) at %0d..%0d", SIZE, 2*SIZE-1);
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("acc row %0d", r), SIZE + r, exp_acc[r]);
  end

  if (errors == 0) $display("  PASS (individual + accumulated visible)\n");
endtask
