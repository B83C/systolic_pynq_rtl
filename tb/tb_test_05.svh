task test_05_eye();
  $display("=== TEST 5: A×Eye (= A) ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(5'h18, 0);
  axil_write(5'h0C, 0);
  axil_write(5'h14, 1);
  load_A();
  stream_mat(B_eye);

  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, A[r]);

  begin
    automatic logic [31:0] rd;
    axil_read(5'h04, rd);
    if (rd[0] !== 0) begin
      $display("  FAIL: unexpected b_underflow=1 after normal matrix");
      errors++;
    end
  end

  if (errors == 0) $display("  PASS\n");
endtask
