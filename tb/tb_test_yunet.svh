`ifndef TB_TEST_YUNET_SVH
`define TB_TEST_YUNET_SVH

// Test SA with weight from YuNet first pointwise layer (185_quantized)
// W[0:4][0:4] as weight, random activations, verify against Python ref.

task test_yunet_pw();
    $display("=== YuNet PW layer: 4x4 sub-block ===");
    reset_test();
    soft_rst_via_axil();
    errors = 0; out_count = 0; q_out_count = 0;

    // Expected from Python: A44 × W44 (INT32, no zp)
    // row 0: [-764, 2818, 4138, 892]
    // row 1: [803, -1037, -134, 1123]
    // row 2: [785, -1277, 746, 215]
    // row 3: [-1026, 1177, -145, -925]

    axil_write(REG_FB_CNT, 0);
    qaxil_write(REG_Q_MUL_Q, 0);
    axil_write(REG_ZP_IN, 0);
    axil_write(REG_A_LOOP_START, 0);
    axil_write(REG_A_LOOP_END, 3);
    axil_write(REG_A_LOAD, 0);

    // Load weight W as A (4 rows)
    for (int r = 0; r < 4; r++) begin
        @(posedge clk);
        for (int c = 0; c < 4; c++)
            s_axis_B_tdata[c*DWI+:DWI] = 8'(yunet_W[r][c]);
        s_axis_B_tvalid = 1; s_axis_B_tlast = 0;
        while (!s_axis_B_tready) @(posedge clk);
        @(posedge clk);
        s_axis_B_tvalid = 0;
    end
    repeat (10) @(posedge clk);

    // Stream activation A as B (4 rows)
    for (int r = 0; r < 4; r++) begin
        @(posedge clk);
        for (int c = 0; c < 4; c++)
            s_axis_B_tdata[c*DWI+:DWI] = 8'(yunet_A[r][c]);
        s_axis_B_tvalid = 1;
        s_axis_B_tlast = (r == 3);
        while (!s_axis_B_tready) @(posedge clk);
        @(posedge clk);
        s_axis_B_tvalid = 0; s_axis_B_tlast = 0;
    end

    wait_output_done();
    repeat (20) @(posedge clk);

    $display("  outputs: %0d", out_count);

    if (out_count >= 4) begin
        $write("  row 0:"); for (int c=0; c<4; c++) $write(" %0d", result[0][c]); $display("  (exp -764 2818 4138 892)");
        $write("  row 1:"); for (int c=0; c<4; c++) $write(" %0d", result[1][c]); $display("  (exp 803 -1037 -134 1123)");
        $write("  row 2:"); for (int c=0; c<4; c++) $write(" %0d", result[2][c]); $display("  (exp 785 -1277 746 215)");
        $write("  row 3:"); for (int c=0; c<4; c++) $write(" %0d", result[3][c]); $display("  (exp -1026 1177 -145 -925)");

        if (result[0][0]==-764 && result[0][1]==2818 && result[0][2]==4138 && result[0][3]==892 &&
            result[1][0]==803 && result[1][1]==-1037 && result[1][2]==-134 && result[1][3]==1123 &&
            result[2][0]==785 && result[2][1]==-1277 && result[2][2]==746 && result[2][3]==215 &&
            result[3][0]==-1026 && result[3][1]==1177 && result[3][2]==-145 && result[3][3]==-925)
            $display("  PASS\n");
        else begin
            $display("  FAIL"); errors++;
        end
    end else begin
        $display("  FAIL: not enough outputs"); errors++;
    end
endtask

`endif
