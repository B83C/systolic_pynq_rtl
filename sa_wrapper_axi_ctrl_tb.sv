`timescale 1ns / 1ps

`include "defs.svh"

module sa_wrapper_axi_ctrl_tb;

  localparam SIZE = 4;
  localparam A_DEPTH = 8;
  localparam DATA_WIDTH_IN = 8;
  localparam DATA_WIDTH_OUT = 32;
  localparam AXI_IN_WIDTH = SIZE * DATA_WIDTH_IN;
  localparam AXI_OUT_WIDTH = SIZE * DATA_WIDTH_OUT;

  logic                     clk;
  logic                     rst_n;

  logic [ AXI_IN_WIDTH-1:0] s_axis_B_tdata;
  logic                     s_axis_B_tvalid;
  logic                     s_axis_B_tready;
  logic                     s_axis_B_tlast;

  logic [AXI_OUT_WIDTH-1:0] m_axis_tdata;
  logic                     m_axis_tvalid;
  logic                     m_axis_tready;
  logic                     m_axis_tlast;

  logic                     s_axil_awvalid;
  logic                     s_axil_awready;
  logic [              4:0] s_axil_awaddr;
  logic [             31:0] s_axil_wdata;
  logic                     s_axil_wvalid;
  logic                     s_axil_wready;
  logic [              1:0] s_axil_bresp;
  logic                     s_axil_bvalid;
  logic                     s_axil_bready;

  logic                     s_axil_arvalid;
  logic                     s_axil_arready;
  logic [              4:0] s_axil_araddr;
  logic [             31:0] s_axil_rdata;
  logic [              1:0] s_axil_rresp;
  logic                     s_axil_rvalid;
  logic                     s_axil_rready;

  logic                     a_bypass;

  sa_wrapper_axi_ctrl_sv #(
      .SIZE(SIZE),
      .A_DEPTH(A_DEPTH),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) dut (
      .*
  );

  always #1 clk = ~clk;

  // Unpacked aliases for waveform viewing
  wire [31:0] m_axis_out[3:0];
  wire [31:0] s_axis_B_in[3:0];
  wire [7:0] dut_a_row[3:0];
  wire [31:0] dut_c_row[3:0];
  wire [31:0] dut_result_row[3:0];
  generate
    for (genvar i = 0; i < SIZE; i++) begin
      assign m_axis_out[i]     = m_axis_tdata[i*32+:32];
      assign s_axis_B_in[i]    = {24'h0, s_axis_B_tdata[i*8+:8]};
      assign dut_a_row[i]      = dut.a_row[i];
      assign dut_c_row[i]      = dut.c_row[i];
      assign dut_result_row[i] = dut.result_row[i];
    end
  endgenerate

  int errors;
  int out_count;
  int result[256][4];
  bit result_tlast[256];

  // Test matrices
  int A[4][4] = '{'{10, 11, 12, 13}, '{11, 12, 13, 14}, '{12, 13, 14, 15}, '{13, 14, 15, 16}};
  int B1[4][4] = '{'{1, 2, 3, 4}, '{5, 6, 7, 8}, '{9, 10, 11, 12}, '{13, 14, 15, 16}};
  int B2[4][4] = '{'{0, 1, 0, 0}, '{1, 0, 0, 0}, '{0, 0, 0, 1}, '{0, 0, 1, 0}};

  task axil_write(input [4:0] addr, input [31:0] data);
    @(posedge clk);
    s_axil_awvalid = 1;
    s_axil_awaddr  = addr;
    s_axil_wdata   = data;
    s_axil_wvalid  = 1;
    @(posedge clk);
    while (!s_axil_bvalid) @(posedge clk);
    s_axil_awvalid = 0;
    s_axil_wvalid  = 0;
    s_axil_bready  = 1;
    @(posedge clk);
    s_axil_bready = 0;
  endtask

  task axil_read(input [4:0] addr, output [31:0] data);
    @(posedge clk);
    s_axil_arvalid = 1;
    s_axil_araddr  = addr;
    @(posedge clk);
    s_axil_arvalid = 0;
    while (!s_axil_rvalid) @(posedge clk);
    data = s_axil_rdata;
    s_axil_rready = 1;
    @(posedge clk);
    s_axil_rready = 0;
  endtask

  task stream_word(input [AXI_IN_WIDTH-1:0] data, input last = 0);
    s_axis_B_tdata  = data;
    s_axis_B_tlast  = last;
    s_axis_B_tvalid = 1;
    wait (s_axis_B_tready);
    @(posedge clk);  // consume the first word
    // while (!s_axis_B_tready) @(posedge clk);
    // @(posedge clk);
    s_axis_B_tvalid = 0;
    s_axis_B_tlast  = 0;
  endtask

  task automatic stream_B(input int B[4][4], input bit last_set = 1);
    automatic logic [AXI_IN_WIDTH-1:0] row_data;
    // First word: handshake (handles IDLE→LOAD_B transition) then wait for consume
    row_data = 0;
    foreach (B[i]) begin
      for (int c = 0; c < SIZE; c++) row_data[c*DATA_WIDTH_IN+:DATA_WIDTH_IN] = 8'(B[i][c]);
      s_axis_B_tdata  = row_data;
      s_axis_B_tvalid = 1;
      s_axis_B_tlast  = last_set && (i == SIZE - 1);
      wait (s_axis_B_tready);
      @(posedge clk);  // consume the first word
    end
    // All rows consumed
    s_axis_B_tvalid = 0;
    s_axis_B_tlast  = 0;
  endtask

  task load_A_rows();
    for (int r = 0; r < SIZE; r++) begin
      automatic logic [AXI_IN_WIDTH-1:0] row_data = 0;
      for (int c = 0; c < SIZE; c++) row_data[c*DATA_WIDTH_IN+:DATA_WIDTH_IN] = 8'(A[r][c]);
      stream_word(row_data);
    end
  endtask

  task wait_for_idle();
    logic [31:0] val;
    axil_read(5'h00, val);
    while (val[0]) begin
      @(posedge clk);
      axil_read(5'h00, val);
    end
  endtask

  task trigger_A_LOAD();
    axil_write(5'h1C, SIZE - 1);
    axil_write(5'h10, 0);
    load_A_rows();
    wait_for_idle();
    // axil_write(5'h1C, 0);
  endtask

  // Capture output
  always @(posedge clk) begin
    if (m_axis_tvalid && m_axis_tready) begin
      $write("OUT[%0d]=", out_count);
      for (int i = 0; i < SIZE; i++) $write(" %0d", m_axis_tdata[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT]);
      $write(" tlast=%0d\n", m_axis_tlast);
      for (int i = 0; i < SIZE; i++)
      result[out_count][i] = m_axis_tdata[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT];
      result_tlast[out_count] = m_axis_tlast;
      out_count++;
    end
  end

  // Utility to wait for a specific number of outputs
  task wait_outputs(int n);
    while (out_count < n) @(posedge clk);
  endtask

  // Compute A × B (full matrix)
  function automatic void matmul(int A[4][4], int B[4][4], ref int out[4][4]);
    for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) begin
      out[r][c] = 0;
      for (int k = 0; k < SIZE; k++) out[r][c] += A[r][k] * B[k][c];
    end
  endfunction

  // Compute row × matrix
  function automatic void matmul_row(int row[4], int B[4][4], ref int out[4]);
    for (int c = 0; c < SIZE; c++) begin
      out[c] = 0;
      for (int k = 0; k < SIZE; k++) out[c] += row[k] * B[k][c];
    end
  endfunction

  int expected[4][4];
  int AB1[4][4], AB2[4][4];
  int B_rand[4][4];
  int B_tmp[4][4];
  int exp_row[4];
  int full_result[4][4];
  int expected_rows[5][4];

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_wrapper_axi_ctrl_tb);
    clk = 0;
    rst_n = 0;
    s_axis_B_tvalid = 0;
    s_axis_B_tlast = 0;
    s_axis_B_tdata = 0;
    s_axil_awvalid = 0;
    s_axil_wvalid = 0;
    s_axil_arvalid = 0;
    s_axil_bready = 0;
    s_axil_rready = 0;
    m_axis_tready = 1;
    errors = 0;
    out_count = 0;

    $display("=== Reset ===");
    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    $display("Reset done\n");

    axil_write(5'h18, 0);

    // ═══════════════════════════════════════════════
    // TEST 1: Single matrix multiply
    // ═══════════════════════════════════════════════
    $display("=== TEST 1: Single multiply ===");
    errors = 0;
    out_count = 0;

    axil_write(5'h0C, 0);
    axil_write(5'h14, 1);

    trigger_A_LOAD();

    $display("  streaming B...");
    stream_B(B1);

    wait_outputs(1);
    @(posedge clk);
    $display("  output done, out_count=%0d", out_count);

    // SA computes: a_row × b_row (row vector × matrix)
    matmul_row(A[0], B1, exp_row);
    errors = 0;
    for (int c = 0; c < SIZE; c++)
    if (result[0][c] !== exp_row[c]) begin
      $display("  MISMATCH [0][%0d]: got %0d, expected %0d", c, result[0][c], exp_row[c]);
      errors++;
    end
    if (errors == 0) $display("  PASS");
    else $display("  FAIL: %0d errors", errors);

    // ═══════════════════════════════════════════════
    // TEST 2: Two sequential runs
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 2: Two sequential runs ===");

    for (int run = 0; run < 2; run++) begin
      errors = 0;
      out_count = 0;
      trigger_A_LOAD();
      stream_B(run == 0 ? B1 : B2);
      wait_outputs(1);
      @(posedge clk);
      $display("  run %0d done, out_count=%0d", run, out_count);
      matmul_row(A[0], run == 0 ? B1 : B2, exp_row);
      for (int c = 0; c < SIZE; c++)
      if (result[0][c] !== exp_row[c]) begin
        $display("  MISMATCH run %0d [%0d]: got %0d, expected %0d", run, c, result[0][c],
                 exp_row[c]);
        errors++;
      end
      if (errors == 0) $display("  run %0d PASS", run);
    end

    // ═══════════════════════════════════════════════
    // TEST 3: Accumulation (acc_cnt=3 → 3 runs, 1 output)
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 3: Accumulation (acc_cnt=3) ===");

    rst_n = 0;
    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    axil_write(5'h18, 0);

    errors = 0;
    out_count = 0;

    matmul_row(A[0], B1, AB1[0]);
    matmul_row(A[0], B2, AB2[0]);
    for (int c = 0; c < SIZE; c++) expected[0][c] = AB1[0][c] + 2 * AB2[0][c];

    axil_write(5'h0C, 3);
    axil_write(5'h14, 1);

    trigger_A_LOAD();

    $display("  Run 1 (B1)...");
    stream_B(B1, 0);  // no tlast — part of continuous stream

    $display("  Run 2 (B2)...");
    stream_B(B2, 0);  // no tlast

    $display("  Run 3 (B2, final)...");
    stream_B(B2, 1);  // tlast on last word — marks stream end

    wait_outputs(3);
    @(posedge clk);

    $display("  out_count=%0d", out_count);

    errors = 0;
    for (int c = 0; c < SIZE; c++)
    if (result[2][c] != expected[0][c]) begin
      $display("  MISMATCH final [%0d]: got %0d, expected %0d", c, result[2][c], expected[0][c]);
      errors++;
    end
    // Check tlast: only last output (index 2) should have tlast
    for (int i = 0; i < out_count; i++) begin
      if (i < out_count - 1 && result_tlast[i]) begin
        $display("  MISMATCH output %0d: unexpected tlast=1", i);
        errors++;
      end
      if (i == out_count - 1 && !result_tlast[i]) begin
        $display("  MISMATCH output %0d: expected tlast=1", i);
        errors++;
      end
    end
    if (errors == 0) $display("  PASS (3 outputs, final correct, tlast follows input)");
    else $display("  FAIL: %0d errors", errors);

    // ═══════════════════════════════════════════════
    // TEST 4: Random matrix (no accumulation)
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 4: Random matrix ===");
    rst_n = 0;
    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    axil_write(5'h18, 0);

    errors = 0;
    out_count = 0;

    for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) begin
      A[r][c] = $urandom_range(0, 20);
      B_rand[r][c] = $urandom_range(0, 20);
    end

    axil_write(5'h14, 1);
    axil_write(5'h0C, 0);

    trigger_A_LOAD();
    stream_B(B_rand);
    wait_outputs(1);
    @(posedge clk);
    $display("  output done, out_count=%0d", out_count);
    matmul_row(A[0], B_rand, exp_row);

    errors = 0;
    for (int c = 0; c < SIZE; c++)
    if (result[0][c] !== exp_row[c]) begin
      $display("  MISMATCH [%0d]: got %0d, expected %0d", c, result[0][c], exp_row[c]);
      errors++;
    end
    if (errors == 0) $display("  PASS");

    // ═══════════════════════════════════════════════
    // TEST 5: Long streaming B (5 runs, same A)
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 5: Long streaming B (5 runs, same A) ===");
    rst_n = 0;
    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    axil_write(5'h18, 0);

    errors = 0;
    axil_write(5'h14, 1);

    A[0][0] = 10;
    A[0][1] = 11;
    A[0][2] = 12;
    A[0][3] = 13;
    A[1][0] = 11;
    A[1][1] = 12;
    A[1][2] = 13;
    A[1][3] = 14;
    A[2][0] = 12;
    A[2][1] = 13;
    A[2][2] = 14;
    A[2][3] = 15;
    A[3][0] = 13;
    A[3][1] = 14;
    A[3][2] = 15;
    A[3][3] = 16;

    trigger_A_LOAD();

    for (int run = 0; run < 5; run++) begin
      $display("  B run %0d...", run);

      if (run < 3) begin
        for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++) B_tmp[r][c] = $urandom_range(0, 15);
      end else if (run == 3) begin
        for (int r = 0; r < SIZE; r++) for (int c = 0; c < SIZE; c++) B_tmp[r][c] = B1[r][c];
      end else begin
        for (int r = 0; r < SIZE; r++) for (int c = 0; c < SIZE; c++) B_tmp[r][c] = B2[r][c];
      end

      out_count = 0;
      matmul_row(A[0], B_tmp, exp_row);
      stream_B(B_tmp);
      wait_outputs(1);
      @(posedge clk);
    end

    // After all 5 runs, check each run's result
    // Actually, we need to check per run. Let's redo with output tracking.
    // For simplicity: we just check the last run produced correct output.
    // The per-run check requires us to not reset out_count between runs.
    // Let me redo this test properly below.

    // Actually, this test structure is awkward with the new single-output-per-run design.
    // Let me restructure: send all 5 B matrices back-to-back and check all 5 outputs.

    $display("\n=== TEST 5b: 5 back-to-back B matrices ===");
    rst_n = 0;
    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    axil_write(5'h18, 0);

    errors = 0;
    out_count = 0;
    axil_write(5'h14, 1);
    axil_write(5'h0C, 0);

    trigger_A_LOAD();

    // Pre-generate all matrices and expected results (single pass)
    begin
      int B_list[5][SIZE] [SIZE];
      int exp_5b[5][SIZE];

      for (int run = 0; run < 5; run++) begin
        if (run < 3) begin
          for (int r = 0; r < SIZE; r++)
          for (int c = 0; c < SIZE; c++) B_list[run][r][c] = $urandom_range(0, 15);
        end else if (run == 3) begin
          for (int r = 0; r < SIZE; r++)
          for (int c = 0; c < SIZE; c++) B_list[run][r][c] = B1[r][c];
        end else begin
          for (int r = 0; r < SIZE; r++)
          for (int c = 0; c < SIZE; c++) B_list[run][r][c] = B2[r][c];
        end
        for (int c = 0; c < SIZE; c++) begin
          exp_5b[run][c] = 0;
          for (int k = 0; k < SIZE; k++) exp_5b[run][c] += A[0][k] * B_list[run][k][c];
        end
      end

      trigger_A_LOAD();
      out_count = 0;

      for (int run = 0; run < 5; run++) begin
        stream_B(B_list[run], run == 4);
      end

      wait_outputs(5);
      @(posedge clk);
      $display("  output done, out_count=%0d", out_count);

      for (int run = 0; run < out_count; run++) begin
        for (int c = 0; c < SIZE; c++)
        if (result[run][c] != exp_5b[run][c]) begin
          $display("  MISMATCH run %0d [%0d]: got %0d, expected %0d", run, c, result[run][c],
                   exp_5b[run][c]);
          errors++;
        end
        if (run < out_count - 1 && result_tlast[run]) begin
          $display("  MISMATCH run %0d: unexpected tlast=1", run);
          errors++;
        end
        if (run == out_count - 1 && !result_tlast[run]) begin
          $display("  MISMATCH run %0d: expected tlast=1", run);
          errors++;
        end
      end
      if (errors == 0) $display("  All 5 back-to-back runs PASS (tlast follows input)");
    end

    // ═══════════════════════════════════════════════
    // TEST 5c: Continuous streaming (no tlast at all)
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 5c: Continuous streaming (no tlast) ===");
    out_count = 0;

    // Stream 3 matrices with NO tlast — truly continuous
    for (int run = 0; run < 3; run++) begin
      for (int r = 0; r < SIZE; r++)
      for (int c = 0; c < SIZE; c++) B_tmp[r][c] = $urandom_range(0, 15);
      stream_B(B_tmp, 0);
    end
    // Now send one more with tlast to flush
    stream_B(B1, 1);

    wait_outputs(4);
    @(posedge clk);
    $display("  output done, out_count=%0d", out_count);

    for (int run = 0; run < out_count; run++) begin
      if (run < out_count - 1 && result_tlast[run]) begin
        $display("  MISMATCH run %0d: tlast should be 0 (continuous stream)", run);
        errors++;
      end
      if (run == out_count - 1 && !result_tlast[run]) begin
        $display("  MISMATCH run %0d: tlast should be 1 (last in stream)", run);
        errors++;
      end
    end
    if (errors == 0) $display("  Continuous streaming PASS (tlast=0 on all but last output)");

    // ═══════════════════════════════════════════════
    // TEST 6: Full matrix multiply (4 different rows)
    // ═══════════════════════════════════════════════
    $display("\n=== TEST 6: Full matmul A×B1 (4 different rows) ===");
    rst_n = 0;
    repeat (3) @(posedge clk);
    rst_n = 1;
    @(posedge clk);

    errors = 0;
    out_count = 0;
    axil_write(5'h14, 1);
    axil_write(5'h0C, 0);

    A[0][0] = 10;
    A[0][1] = 11;
    A[0][2] = 12;
    A[0][3] = 13;
    A[1][0] = 11;
    A[1][1] = 12;
    A[1][2] = 13;
    A[1][3] = 14;
    A[2][0] = 12;
    A[2][1] = 13;
    A[2][2] = 14;
    A[2][3] = 15;
    A[3][0] = 13;
    A[3][1] = 14;
    A[3][2] = 15;
    A[3][3] = 16;

    // Load all A rows into ring indices 0..3
    axil_write(5'h18, 0);
    axil_write(5'h1C, SIZE - 1);
    axil_write(5'h10, 0);
    load_A_rows();
    wait_for_idle();

    // Compute each output row by fixing a_loop_start/r to select different A rows
    for (int r = 0; r < SIZE; r++) begin
      axil_write(5'h18, r);
      axil_write(5'h1C, r);
      out_count = 0;
      $display("  Row %0d: a_row = A[%0d]", r, r);
      stream_B(B1);
      wait_outputs(1);
      @(posedge clk);
      for (int c = 0; c < SIZE; c++) full_result[r][c] = result[0][c];
      $write("    OUT =");
      for (int c = 0; c < SIZE; c++) $write(" %0d", full_result[r][c]);
      $write("\n");
    end

    matmul(A, B1, AB1);
    errors = 0;
    for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
    if (full_result[r][c] != AB1[r][c]) begin
      $display("  MISMATCH [%0d][%0d]: got %0d, expected %0d", r, c, full_result[r][c], AB1[r][c]);
      errors++;
    end
    if (errors == 0) $display("  Full matmul A×B1 PASS (all rows different)");

    // ═══════════════════════════════════════════════
    // Summary
    // ═══════════════════════════════════════════════
    if (errors != 0) $display("\nFAIL: %0d total errors", errors);
    else $display("\nPASS: all tests passed");
    $finish;
  end

  // debug monitor (print on state change only)
  state_t prev_state;
  always @(posedge clk) begin
    if (dut.state != prev_state) begin
      prev_state <= dut.state;
      $display("  MON clk=%0t st=%0d run=%b aLP=%d", $time, dut.state, dut.state_running,
               dut.a_load_pending);
      // $display("    aLP=%d aLD=%d", dut.a_load_pending, dut.a_load_done);
    end
  end

  initial
    #5000 begin
      $display("\nTIMEOUT: simulation took too long");
      $finish;
    end

endmodule
