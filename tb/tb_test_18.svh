`ifndef TB_TEST_18_SVH
`define TB_TEST_18_SVH
`include "tb/tb_common.svh"
task test_18_single_element_ring();
  $display("=== TEST 18: Single-element ring (start==end) ===");
  errors = 0; out_count = 0;

  soft_rst_via_axil();
  repeat (5) @(posedge clk);
  // Force IDLE if needed
  if (dut.state == 2'd2) begin s_axis_B_tlast = 1; @(posedge clk); s_axis_B_tlast = 0; end
  repeat (3) @(posedge clk);

  // Set a_loop_start = a_loop_end = 0 → single-element ring
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_A_LOOP_END, 0);

  // Trigger A_LOAD
  axil_write(REG_A_LOAD, 0);
  repeat (3) @(posedge clk);
  if (dut.state != 2'd1) begin
    $display("  FAIL[1]: state=%0d expected LOAD_A", dut.state); errors++;
  end else $display("    LOAD_A entered");

  // Stream one row with tlast
  begin
    logic [AXI_IN_W-1:0] row_data;
    row_data = 0;
    for (int c = 0; c < SIZE; c++)
      row_data[c*DWI+:DWI] = 8'(A[0][c]);
    wait (s_axis_B_tready);
    @(posedge clk); #1;
    s_axis_B_tdata = row_data;
    s_axis_B_tvalid = 1;
    s_axis_B_tlast = 1;
    @(posedge clk); #1;
    s_axis_B_tvalid = 0;
    s_axis_B_tlast = 0;
  end
  repeat (3) @(posedge clk);

  // Verify state exited LOAD_A
  if (dut.state == 2'd1) begin
    $display("  FAIL[2]: state stuck in LOAD_A after single row"); errors++;
  end else $display("    state exited LOAD_A (state=%0d)", dut.state);

  // Now do a clean full compute to verify normal ring still works after restore
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_A_LOOP_END, SIZE - 1);
  out_count = 0;
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
