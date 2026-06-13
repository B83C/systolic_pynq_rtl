`ifndef TB_TEST_11_SVH
`define TB_TEST_11_SVH
`include "tb/tb_common.svh"
task automatic test_11_ring_multi_a();
  $display("=== TEST 11: Ring buffer — 2 A matrices, continuous B ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_ACC_OUT, 1);
  axil_write(REG_FB_CNT, 0);

  // Load A and A2 into 8-entry ring
  axil_write(REG_A_LOOP_END, 7);
  axil_write(REG_A_LOAD, 0);
  stream_mat(A,  0);
  stream_mat(A2, 0);
  repeat (10) @(posedge clk);

  // Stream B1 twice continuously — tvalid stays high, no deassertion
  begin
    automatic logic [AXI_IN_W-1:0] rd;
    for (int i = 0; i < SIZE; i++) begin
      rd = 0;
      for (int c = 0; c < SIZE; c++) rd[c*DWI+:DWI] = 8'(B1[i][c]);
      wait (s_axis_B_tready);
      @(posedge clk);
      #1;
      s_axis_B_tdata = rd;
      s_axis_B_tvalid = 1;
      s_axis_B_tlast = 0;
    end
    // Second B1 matrix without gap
    for (int i = 0; i < SIZE; i++) begin
      rd = 0;
      for (int c = 0; c < SIZE; c++) rd[c*DWI+:DWI] = 8'(B1[i][c]);
      wait (s_axis_B_tready);
      @(posedge clk);
      #1;
      s_axis_B_tdata = rd;
      s_axis_B_tvalid = 1;
      s_axis_B_tlast = (i == SIZE - 1);
    end
    wait (s_axis_B_tready);
    @(posedge clk);
    #1;
    s_axis_B_tvalid = 0;
    s_axis_B_tlast = 0;
  end

  wait_output_done();
  $display("  captured %0d outputs", out_count);

  if (out_count < 2 * SIZE) begin
    $display("  FAIL: expected at least %0d outputs, got %0d", 2 * SIZE, out_count);
    errors++;
  end else begin
    $display("  Checking A*B1 at 0..3");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("A1B row %0d", r), r, exp_A1B[r]);
    $display("  Checking A2*B1 at 4..7");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("A2B row %0d", r), SIZE + r, exp_A2B[r]);
    if (result_tlast[2 * SIZE - 1] !== 1) begin
      $display("  MISMATCH: last output expected tlast=1");
      errors++;
    end
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
