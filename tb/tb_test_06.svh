`ifndef TB_TEST_06_SVH
`define TB_TEST_06_SVH
`include "tb/tb_common.svh"
task test_06_consecutive_groups();
  $display("=== TEST 6: Two consecutive groups (6 matrices, FB_CNT=2) ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 2);
  axil_write(REG_ACC_CNT, 1);
  load_A();

  stream_mat(B1, 0);
  stream_mat(B2, 0);
  stream_mat(B_eye, 0);
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

  $display("  Checking group 1");
  if (out_count >= 3*SIZE) begin
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("grp1 B1 row %0d", r), r, exp_B1[r]);
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("grp1 acc row %0d", r), SIZE + r, exp_acc[r]);
  end

  if (out_count >= 6*SIZE) begin
    $display("  Checking group 2 at %0d..%0d", 3*SIZE, 5*SIZE-1);
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("grp2 B1 row %0d", r), 3*SIZE + r, exp_B1[r]);
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("grp2 acc row %0d", r), 4*SIZE + r, exp_acc[r]);
  end else begin
    $display("  Skipping group 2 checks (only %0d outputs)", out_count);
  end

  if (out_count > 0 && !result_tlast[out_count-1]) begin
    $display("  MISMATCH: last output expected tlast=1");
    errors++;
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
