`ifndef TB_TEST_20_SVH
`define TB_TEST_20_SVH

// Compute expected quantized output given raw and quant params
function int quant_exp(input int raw, input int mq, input int sh, input int zp);
    automatic int prod = raw * mq;
    automatic int shifted = prod >>> sh;
    automatic int rez = shifted + zp;
    return (rez > 127) ? 127 : ((rez < -128) ? -128 : rez);
endfunction

// Helper: check quantizer output against expected
task check_quant(string label, int nrows);
    for (int r = 0; r < nrows; r++) begin
        for (int c = 0; c < SIZE; c++) begin
            if (q_result[r][c] !== quant_exp(result[r][c], o_mul_q, o_shift, o_zp_out)) begin
                $display("  %s [%0d][%0d]: got %0d, exp %0d (raw=%0d, q=%0d>>%0d+%0d)",
                         label, r, c, q_result[r][c],
                         quant_exp(result[r][c], o_mul_q, o_shift, o_zp_out),
                         result[r][c], o_mul_q, o_shift, o_zp_out);
                errors++;
            end
        end
    end
endtask

task test_20_quantizer_scale();
    $display("=== TEST 20: Quantizer — scale ×2 ===");
    reset_test();
    errors = 0; out_count = 0; q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 2);
    axil_write(REG_SHIFT, 0);
    axil_write(REG_ZP_OUT, 0);
    load_A();
    stream_mat(B1, 1);
    wait_output_done();
    repeat(10) @(posedge clk);

    $display("  raw: %0d  quant: %0d", out_count, q_out_count);
    if (q_out_count < SIZE) begin
        $display("  FAIL: not enough quantized outputs"); errors++;
    end else check_quant("scale", SIZE);
    if (errors == 0) $display("  PASS\n");
endtask

task test_21_quantizer_shift();
    $display("=== TEST 21: Quantizer — shift >>1 (div 2) ===");
    reset_test();
    errors = 0; out_count = 0; q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 1);
    axil_write(REG_SHIFT, 1);
    axil_write(REG_ZP_OUT, 0);
    load_A();
    stream_mat(B1, 1);
    wait_output_done();
    repeat(10) @(posedge clk);

    $display("  raw: %0d  quant: %0d", out_count, q_out_count);
    if (q_out_count < SIZE) begin
        $display("  FAIL: not enough quantized outputs"); errors++;
    end else check_quant("shift", SIZE);
    if (errors == 0) $display("  PASS\n");
endtask

task test_22_quantizer_zp();
    $display("=== TEST 22: Quantizer — zp_out=10 ===");
    reset_test();
    errors = 0; out_count = 0; q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 1);
    axil_write(REG_SHIFT, 0);
    axil_write(REG_ZP_OUT, 10);
    load_A();
    stream_mat(B1, 1);
    wait_output_done();
    repeat(10) @(posedge clk);

    $display("  raw: %0d  quant: %0d", out_count, q_out_count);
    if (q_out_count < SIZE) begin
        $display("  FAIL: not enough quantized outputs"); errors++;
    end else check_quant("zp_out", SIZE);
    if (errors == 0) $display("  PASS\n");
endtask

task test_23_quantizer_negative();
    $display("=== TEST 23: Quantizer — negative clamp ===");
    reset_test();
    errors = 0; out_count = 0; q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 1);
    axil_write(REG_SHIFT, 0);
    axil_write(REG_ZP_OUT, 0);
    load_A();
    stream_mat(Bneg, 1);
    wait_output_done();
    repeat(10) @(posedge clk);

    $display("  raw: %0d  quant: %0d", out_count, q_out_count);
    for (int r = 0; r < SIZE; r++)
        $display("  row %0d: quant=%0d %0d %0d %0d  raw=%0d %0d %0d %0d",
                 r, q_result[r][0], q_result[r][1], q_result[r][2], q_result[r][3],
                 result[r][0], result[r][1], result[r][2], result[r][3]);
    if (q_out_count < SIZE) begin
        $display("  FAIL: not enough quantized outputs"); errors++;
    end else check_quant("negative", SIZE);
    if (errors == 0) $display("  PASS\n");
endtask

task test_24_quantizer_mixed();
    $display("=== TEST 24: Quantizer — mul=3 >>2 +5 ===");
    reset_test();
    errors = 0; out_count = 0; q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 3);
    axil_write(REG_SHIFT, 2);
    axil_write(REG_ZP_OUT, 5);
    load_A();
    stream_mat(B1, 1);
    wait_output_done();
    repeat(10) @(posedge clk);

    $display("  raw: %0d  quant: %0d", out_count, q_out_count);
    if (q_out_count < SIZE) begin
        $display("  FAIL: not enough quantized outputs"); errors++;
    end else check_quant("mixed", SIZE);
    if (errors == 0) $display("  PASS\n");
endtask

`endif
