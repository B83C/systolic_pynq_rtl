`ifndef TB_TEST_19_SVH
`define TB_TEST_19_SVH

// Test quantizer: set mul_q=1, shift=0, zp_out=0 → raw SA values clamped to int8
task test_19_quantizer_identity();
    $display("=== TEST 19: Quantizer — identity (mul_q=1, shift=0, zp_out=0) ===");
    reset_test();
    errors = 0;
    out_count = 0;
    q_out_count = 0;

    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_MUL_Q, 1);
    axil_write(REG_SHIFT, 0);
    axil_write(REG_ZP_OUT, 0);
    load_A();

    stream_mat(B1, 1);

    wait_output_done();
    repeat(10) @(posedge clk); // wait for quantizer pipeline drain

    $display("  raw outputs: %0d,  quantized outputs: %0d", out_count, q_out_count);

    if (q_out_count < SIZE) begin
        $display("  FAIL: expected at least %0d quantized outputs", SIZE); errors++;
    end else begin
        // With mul_q=1, shift=0, zp_out=0: q_out = clamp(acc, -128, 127)
        // B1 values (342, 388, ...) are >127 → should be clamped to 127
        for (int r = 0; r < SIZE; r++) begin
            $write("  q row %0d:", r);
            for (int c = 0; c < SIZE; c++) $write(" %0d", q_result[r][c]);
            $display(" (raw row: %0d %0d %0d %0d)", result[r][0], result[r][1], result[r][2], result[r][3]);

            // raw > 127 → expect 127 (saturated)
            // raw < -128 → expect -128
            for (int c = 0; c < SIZE; c++) begin
                automatic int raw = result[r][c];
                automatic int exp = (raw > 127) ? 127 : ((raw < -128) ? -128 : raw);
                if (q_result[r][c] !== exp) begin
                    $display("    q[%0d][%0d]: got %0d, exp %0d (raw=%0d)", r, c, q_result[r][c], exp, raw);
                    errors++;
                end
            end
        end
    end

    if (errors == 0) $display("  PASS\n");
endtask

`endif
