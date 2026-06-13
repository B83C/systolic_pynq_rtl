`ifndef TB_TEST_12_SVH
`define TB_TEST_12_SVH
`include "tb/tb_common.svh"
task automatic test_12_ring_accumulation();
  $display("=== TEST 12: Ring buffer — 2 A matrices, accumulation ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;

  // Load A and A2 into 8-entry ring
  axil_write(REG_A_LOOP_END, 7);
  axil_write(REG_A_LOAD, 0);
  stream_mat(A,  0);
  stream_mat(A2, 0);
  repeat (10) @(posedge clk);

  // FB_CNT=2, acc_out=1 — show both individual and accumulated
  axil_write(REG_FB_CNT, 2);
  axil_write(REG_ACC_CNT, 1);

  // 3 B matrices: B1 (uses A), B2 (uses A2), B3 (uses A again, triggers reset)
  stream_mat(B1, 0);
  stream_mat(B1, 0);
  stream_mat(B1, 1);

  wait_output_done();
  $display("  captured %0d outputs", out_count);

  if (out_count >= 2 * SIZE) begin
    // First matrix: A×B1 (individual)
    $display("  Checking A*B1 (individual) at 0..3");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("indiv row %0d", r), r, exp_A1B[r]);

    // Accumulated at FB_CNT boundary: A×B1 + A2×B1 at 4..7
    if (out_count >= 2 * SIZE) begin
      $display("  Checking accumulated A*B1 + A2*B1 at 4..7");
      for (int r = 0; r < SIZE; r++)
        check_row_str($sformatf("acc row %0d", r), SIZE + r, exp_ring_acc[r]);
    end

    if (result_tlast[out_count-1] !== 1) begin
      $display("  MISMATCH: last output expected tlast=1");
      errors++;
    end
  end else begin
    $display("  FAIL: expected at least %0d outputs, got %0d", 2 * SIZE, out_count);
    errors++;
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
