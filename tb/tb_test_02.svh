`ifndef TB_TEST_02_SVH
`define TB_TEST_02_SVH
`include "tb/tb_common.svh"
task test_02_backtoback();
  $display("=== TEST 2: Back-to-back (B1, B2, B1) with tlast ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_OUT, 1);
  load_A();

  stream_mat(B1, 0);
  stream_mat(B2, 0);
  stream_mat(B1, 1);

  while (out_count < 3 * SIZE) @(posedge clk);
  @(posedge clk);
  $display("  captured %0d outputs", out_count);

  for (int i = 0; i < 3; i++) begin
    $display("  matrix %0d:", i);
    for (int r = 0; r < SIZE; r++) begin
      $write("    row %0d: ", r);
      for (int c = 0; c < SIZE; c++) $write(" %0d", result[i*SIZE + r][c]);
      $write(" (exp:");
      for (int c = 0; c < SIZE; c++) $write(" %0d", i == 1 ? exp_B2[r][c] : exp_AB1[r][c]);
      $write(")\n");
      check_row_str($sformatf("mat %0d row %0d", i, r), i*SIZE + r,
                    i == 1 ? exp_B2[r] : exp_AB1[r]);
    end
  end
  if (out_count > 0 && !result_tlast[out_count-1]) begin
    $display("  MISMATCH: last output expected tlast=1");
    errors++;
  end
  if (errors == 0) $display("  PASS\n");
endtask
`endif
