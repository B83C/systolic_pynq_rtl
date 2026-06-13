`ifndef TB_TEST_13_SVH
`define TB_TEST_13_SVH
`include "tb/tb_common.svh"
task automatic test_13_c_matrix();
  $display("=== TEST 13: C matrix — A×B1 + C ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_ACC_OUT, 1);
  axil_write(REG_FB_CNT, 0);
  axil_write(6'h24, SIZE - 1);   // C_LOOP_END
  axil_write(6'h20, 0);          // C_LOOP_START
  load_A();

  // Trigger C_LOAD — state now IDLE (load_A used tlast=1 on A)
  axil_write(REG_C_LOAD, 0);
  repeat (10) @(posedge clk);

  // Stream C data
  for (int i = 0; i < SIZE; i++) begin
    wait (s_axis_B_tready);
    @(posedge clk);
    #1;
    s_axis_B_tdata = 100 + i * 100;
    s_axis_B_tvalid = 1;
    s_axis_B_tlast = 0;
  end
  wait (s_axis_B_tready);
  @(posedge clk);
  #1;
  s_axis_B_tvalid = 0;

  stream_mat(B1, 1);
  wait_output_done();

  $display("  captured %0d outputs", out_count);

  if (out_count >= SIZE) begin
    $display("  Checking A×B1 + C");
    for (int r = 0; r < SIZE; r++) begin
      int exp_c[4];
      for (int c = 0; c < SIZE; c++)
        exp_c[c] = exp_B1[r][c] + 100 + r * 100;
      check_row_str($sformatf("row %0d", r), r, exp_c);
    end
  end else begin
    $display("  FAIL: expected at least %0d outputs, got %0d", SIZE, out_count);
    errors++;
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
