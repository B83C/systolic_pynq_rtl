`ifndef TB_TEST_09_SVH
`define TB_TEST_09_SVH
`include "tb/tb_common.svh"
task test_09_continuous_streaming();
  $display("=== TEST 9: Continuous streaming (3 matrices, tvalid never deasserted) ===");
  reset_test();
  errors = 0; out_count = 0;
  m_axis_tready = 1;
  axil_write(REG_A_LOOP_START, 0);
  axil_write(REG_FB_CNT, 0);
  axil_write(REG_ACC_CNT, 1);
  load_A();

  // Stream B1, B2, B_eye continuously — tvalid stays high, tlast only on final packet
  begin
    automatic logic [AXI_IN_W-1:0] rd;
    // B1: intermediate matrix, no tlast
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
    // B2: intermediate matrix, no tlast
    for (int i = 0; i < SIZE; i++) begin
      rd = 0;
      for (int c = 0; c < SIZE; c++) rd[c*DWI+:DWI] = 8'(B2[i][c]);
      wait (s_axis_B_tready);
      @(posedge clk);
      #1;
      s_axis_B_tdata = rd;
      s_axis_B_tvalid = 1;
      s_axis_B_tlast = 0;
    end
    // B_eye: final matrix, tlast on last row
    for (int i = 0; i < SIZE; i++) begin
      rd = 0;
      for (int c = 0; c < SIZE; c++) rd[c*DWI+:DWI] = 8'(B_eye[i][c]);
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

  if (out_count < 3 * SIZE) begin
    $display("  FAIL: expected at least %0d outputs, got %0d", 3 * SIZE, out_count);
    errors++;
  end else begin
    $display("  Checking matrix 1 (B1)");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("mat1 row %0d", r), r, exp_B1[r]);

    $display("  Checking matrix 2 (B2)");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("mat2 row %0d", r), SIZE + r, exp_B2[r]);

    $display("  Checking matrix 3 (B_eye = A)");
    for (int r = 0; r < SIZE; r++)
      check_row_str($sformatf("mat3 row %0d", r), 2 * SIZE + r, A[r]);

    if (result_tlast[3 * SIZE - 1] !== 1) begin
      $display("  MISMATCH: last output expected tlast=1");
      errors++;
    end
  end

  if (errors == 0) $display("  PASS\n");
endtask
`endif
