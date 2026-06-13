`ifndef TB_TEST_03_SVH
`define TB_TEST_03_SVH
`include "tb/tb_common.svh"
task test_03_accumulation_accout0();
  $display("=== TEST 3: FB_CNT=2, acc_out=0 ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 2);
  axil_write(REG_ACC_OUT, 0);
  load_A();

  stream_mat(B1, 0);
  stream_mat(B2, 0);
  stream_mat(B_eye, 1);

  wait_output_done();
  $display("  captured %0d outputs (acc_out=0: only group-boundary)", out_count);

  for (int i = 0; i < out_count; i++) begin
    $write("  out[%0d] =", i);
    for (int c = 0; c < SIZE; c++) $write(" %0d", result[i][c]);
    $write(" tlast=%0d\n", result_tlast[i]);
  end

  if (out_count >= SIZE) begin
    $display("  Checking final accumulated result");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("acc row %0d", r), out_count - SIZE + r, exp_3way[r]);
    if (errors == 0) $display("  final accumulated correct\n");
  end
endtask
`endif
