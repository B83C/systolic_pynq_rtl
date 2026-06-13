`ifndef TB_TEST_05_SVH
`define TB_TEST_05_SVH
`include "tb/tb_common.svh"
task test_05_eye();
  $display("=== TEST 5: A×Eye (= A) ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_CNT, 1);
  load_A();
  stream_mat(B_eye);

  while (out_count < SIZE) @(posedge clk);
  @(posedge clk);
  for (int r = 0; r < SIZE; r++)
    check_row_str($sformatf("row %0d", r), r, A[r]);

  begin
    automatic logic [31:0] rd;
    axil_read(REG_STATUS, rd);
    if (rd[0] !== 0) begin
      $display("  FAIL: unexpected b_underflow=1 after normal matrix");
      errors++;
    end
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
