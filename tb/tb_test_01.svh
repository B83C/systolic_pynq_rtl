`ifndef TB_TEST_01_SVH
`define TB_TEST_01_SVH
`include "tb/tb_common.svh"
task test_01_single_multiply();
  $display("=== TEST 1: Single multiply, no accumulation ===");
  errors = 0; out_count = 0;
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_OUT, 1);
  load_A();
  stream_mat(B1);

  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  $display("  captured %0d outputs", out_count);

  for (int r = 0; r < SIZE; r++) begin
    $write("  row %0d: ", r);
    for (int c = 0; c < SIZE; c++) $write(" %0d", result[r][c]);
    $write(" (exp:");
    for (int c = 0; c < SIZE; c++) $write(" %0d", exp_AB1[r][c]);
    $write(")\n");
  end
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, exp_AB1[r]);

  if (errors == 0) $display("  PASS\n");
  else $display("  FAIL: %0d errors\n", errors);
endtask
`endif
