task automatic test_16_softrst_in_loadc();
  $display("=== TEST 16: soft_reset in LOAD_C → A_LOAD ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(5'h14, 1);
  axil_write(5'h0C, 0);

  // Enter LOAD_C
  $display("  [1] Enter LOAD_C, stream 2 of 4 C values...");
  load_A();
  axil_write(6'h24, SIZE - 1);
  axil_write(5'h08, 0);
  repeat (5) @(posedge clk);
  // Stream only 2 C values (incomplete)
  for (int i = 0; i < 2; i++) begin
    @(posedge clk); s_axis_B_tdata = 100 + i*100; s_axis_B_tvalid = 1; s_axis_B_tlast = 0;
  end
  @(posedge clk); s_axis_B_tvalid = 0;
  repeat (3) @(posedge clk);
  $display("    state=%0d (LOAD_C=%0d)", dut.state, 2'd3);

  // Soft reset — should force state to IDLE
  $display("  [2] soft_reset → should go to IDLE");
  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  $display("    state=%0d (expect IDLE or LOAD_B)", dut.state);
  if (dut.state != 2'd0 && dut.state != 2'd2) begin
    $display("  FAIL[2]: state stuck at %0d", dut.state); errors++;
  end

  // Now load A and compute — without reloading C, expect c_ring=0 from hard reset
  // (soft_rst doesn't clear ring data — real driver reloads C explicitly if needed)
  $display("  [3] Load A, reload C as zeros, then compute");
  load_A();
  // Optional: reload C with zeros
  axil_write(5'h08, 0);
  repeat (5) @(posedge clk);
  for (int i = 0; i < SIZE; i++) begin
    @(posedge clk); s_axis_B_tdata = 0; s_axis_B_tvalid = 1; s_axis_B_tlast = 0;
  end
  @(posedge clk); s_axis_B_tvalid = 0;
  repeat (5) @(posedge clk);
  stream_mat(B1, 1);
  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, exp_B1[r]);
  $display("    outputs: %0d", out_count);

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
