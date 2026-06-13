`ifndef TB_TEST_10_SVH
`define TB_TEST_10_SVH
`include "tb/tb_common.svh"
task automatic test_10_random_backpressure();
  $display("=== TEST 10: Random output back-pressure (5 matrices) ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_OUT, 1);
  load_A();

  stream_mat(B1, 0);  repeat (8) @(posedge clk);
  m_axis_tready = 0;   repeat (5) @(posedge clk);
  m_axis_tready = 1;   repeat (16) @(posedge clk);

  stream_mat(B1, 0);  repeat (8) @(posedge clk);
  m_axis_tready = 0;   repeat (3) @(posedge clk);
  m_axis_tready = 1;   repeat (16) @(posedge clk);

  stream_mat(B1, 0);  repeat (8) @(posedge clk);
  m_axis_tready = 0;   repeat (7) @(posedge clk);
  m_axis_tready = 1;   repeat (16) @(posedge clk);

  stream_mat(B1, 0);  repeat (8) @(posedge clk);
  m_axis_tready = 0;   repeat (4) @(posedge clk);
  m_axis_tready = 1;   repeat (16) @(posedge clk);

  stream_mat(B1, 1);
  m_axis_tready = 1;
  repeat (400) @(posedge clk);

  $display("  captured %0d outputs", out_count);

  if (out_count < 5 * SIZE) begin
    $display("  FAIL: expected at least %0d outputs, got %0d", 5 * SIZE, out_count);
    errors++;
  end

  for (int i = 0; i < out_count; i++) begin
    bit found;
    found = 0;
    for (int r = 0; r < SIZE; r++) begin
      automatic int match = 1;
      for (int c = 0; c < SIZE; c++)
        if (result[i][c] !== exp_B1[r][c]) match = 0;
      if (match) found = 1;
    end
    if (!found) begin
      $write("  FAIL: output[%0d] =", i);
      for (int c = 0; c < SIZE; c++) $write(" %0d", result[i][c]);
      $write(" corrupted\n");
      errors++;
    end
  end

  if (result_tlast[out_count-1] !== 1) begin
    $display("  MISMATCH: last output expected tlast=1");
    errors++;
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
