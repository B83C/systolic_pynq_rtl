`ifndef TB_TEST_07_SVH
`define TB_TEST_07_SVH
`include "tb/tb_common.svh"
task test_07_undersized();
  $display("=== TEST 7: Under-sized matrix should produce no output ===");
  reset_test();
  errors = 0; out_count = 0;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_CNT, 1);
  load_A();

  stream_mat_n(B1, 3);  // only 3 rows, tlast on row 2

  repeat (100) @(posedge clk);
  $display("  captured %0d outputs (expected 0)", out_count);

  if (out_count != 0) begin
    $display("  FAIL: unexpected output from under-sized matrix");
    errors++;
  end else begin
    $display("  output check PASS");
  end

  begin
    automatic logic [31:0] rd;
    axil_read(REG_STATUS, rd);
    $display("  STATUS after underflow: 0x%08x (expect bit 0 = 1)", rd);
    if (rd[0] !== 1) begin
      $display("  FAIL: expected b_underflow=1");
      errors++;
    end
    axil_read(REG_STATUS, rd);
    $display("  STATUS after clear: 0x%08x (expect bit 0 = 0)", rd);
    if (rd[0] !== 0) begin
      $display("  FAIL: expected b_underflow=0 after clear-on-read");
      errors++;
    end
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
