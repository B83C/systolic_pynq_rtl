`timescale 1ns / 1ps
`include "defs.svh"

module sa_wrapper_axi_ctrl_tb;

  localparam SIZE = 4, A_DEPTH = 8, C_DEPTH = 4, DWI = 8, DWO = 32;
  localparam AXI_IN_W = SIZE * DWI, AXI_OUT_W = SIZE * DWO;

  // -----------------------------------------------------------------------
  // DUT & signals
  // -----------------------------------------------------------------------
  logic clk, rst_n;
  logic [AXI_IN_W-1:0] s_axis_B_tdata;
  logic s_axis_B_tvalid, s_axis_B_tready, s_axis_B_tlast;
  logic [AXI_OUT_W-1:0] m_axis_tdata;
  logic m_axis_tvalid, m_axis_tready, m_axis_tlast;
  logic s_axil_awvalid, s_axil_awready;
  logic [ 5:0] s_axil_awaddr;
  logic [31:0] s_axil_wdata;
  logic s_axil_wvalid, s_axil_wready;
  logic [1:0] s_axil_bresp;
  logic s_axil_bvalid, s_axil_bready;
  logic s_axil_arvalid, s_axil_arready;
  logic [ 5:0] s_axil_araddr;
  logic [31:0] s_axil_rdata;
  logic [ 1:0] s_axil_rresp;
  logic s_axil_rvalid, s_axil_rready;

  // AXI-Lite signals for the quantizer (separate slave)
  logic        q_awvalid;
  wire         q_awready;
  logic [ 3:0] q_awaddr;
  logic [31:0] q_wdata;
  logic        q_wvalid;
  wire         q_wready;
  wire  [ 1:0] q_bresp;
  wire         q_bvalid;
  logic        q_bready;
  logic        q_arvalid;
  wire         q_arready;
  logic [ 3:0] q_araddr;
  wire  [31:0] q_rdata;
  wire  [ 1:0] q_rresp;
  wire         q_rvalid;
  logic        q_rready;

  // Latched copies of quantizer config (driven by quantizer's AXI-Lite)
  logic [15:0] cur_mul_q;
  logic [ 4:0] cur_shift;
  logic [ 7:0] cur_zp_out;
  // Quantizer internal register addresses (match quantizer.sv)
  localparam REG_Q_MUL_Q  = 4'h0;
  localparam REG_Q_SHIFT  = 4'h4;
  localparam REG_Q_ZP_OUT = 4'h8;
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      cur_mul_q  <= 0;
      cur_shift  <= 0;
      cur_zp_out <= 0;
    end else if (q_bvalid) begin
      /* verilator lint_off CASEOVERLAP */
      /* verilator lint_off CASEINCOMPLETE */
      case (q_awaddr)
        REG_Q_MUL_Q:  cur_mul_q  <= q_wdata[15:0];
        REG_Q_SHIFT:  cur_shift  <= q_wdata[4:0];
        REG_Q_ZP_OUT: cur_zp_out <= q_wdata[7:0];
      endcase
      /* verilator lint_on CASEOVERLAP */
      /* verilator lint_on CASEINCOMPLETE */
    end
  end

  // quantizer signals
  logic [SIZE*8-1:0] q_m_axis_tdata;
  logic q_m_axis_tvalid, q_m_axis_tready, q_m_axis_tlast;

  sa_wrapper_axi_ctrl_sv #(
      .SIZE(SIZE),
      .A_DEPTH(A_DEPTH),
      .C_DEPTH(C_DEPTH),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO)
  ) dut (
      .clk(clk), .rst_n(rst_n),
      .s_axis_B_tdata(s_axis_B_tdata), .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready), .s_axis_B_tlast(s_axis_B_tlast),
      .m_axis_tdata(m_axis_tdata), .m_axis_tvalid(m_axis_tvalid),
      .m_axis_tready(m_axis_tready), .m_axis_tlast(m_axis_tlast),
      .s_axil_awvalid(s_axil_awvalid), .s_axil_awready(s_axil_awready),
      .s_axil_awaddr(s_axil_awaddr), .s_axil_wdata(s_axil_wdata),
      .s_axil_wvalid(s_axil_wvalid), .s_axil_wready(s_axil_wready),
      .s_axil_bresp(s_axil_bresp), .s_axil_bvalid(s_axil_bvalid),
      .s_axil_bready(s_axil_bready), .s_axil_arvalid(s_axil_arvalid),
      .s_axil_arready(s_axil_arready), .s_axil_araddr(s_axil_araddr),
      .s_axil_rdata(s_axil_rdata), .s_axil_rresp(s_axil_rresp),
      .s_axil_rvalid(s_axil_rvalid), .s_axil_rready(s_axil_rready)
  );

  quantizer #(.SIZE(SIZE), .DATA_WIDTH_IN(DWO)) u_quant (
      .clk, .rst_n,
      .s_axil_awvalid(q_awvalid), .s_axil_awready(q_awready),
      .s_axil_awaddr(q_awaddr),
      .s_axil_wdata(q_wdata), .s_axil_wvalid(q_wvalid), .s_axil_wready(q_wready),
      .s_axil_bresp(q_bresp), .s_axil_bvalid(q_bvalid), .s_axil_bready(q_bready),
      .s_axil_arvalid(q_arvalid), .s_axil_arready(q_arready),
      .s_axil_araddr(q_araddr),
      .s_axil_rdata(q_rdata), .s_axil_rresp(q_rresp),
      .s_axil_rvalid(q_rvalid), .s_axil_rready(q_rready),
      .s_axis_tdata (m_axis_tdata),
      .s_axis_tvalid(m_axis_tvalid),
      .s_axis_tready(),
      .s_axis_tlast (m_axis_tlast),
      .m_axis_tdata (q_m_axis_tdata),
      .m_axis_tvalid(q_m_axis_tvalid),
      .m_axis_tready(1'b1),
      .m_axis_tlast (q_m_axis_tlast)
  );
  always #1 clk = ~clk;

  // -----------------------------------------------------------------------
  // shared test data, tasks, and test-task definitions
  // -----------------------------------------------------------------------
  int errors;
  int out_count;
  int result[4096][4];
  bit result_tlast[4096];

  `include "tb/tb_common.svh"
  `include "tb/tb_test_01.svh"
  `include "tb/tb_test_02.svh"
  `include "tb/tb_test_03.svh"
  `include "tb/tb_test_04.svh"
  `include "tb/tb_test_05.svh"
  `include "tb/tb_test_06.svh"
  `include "tb/tb_test_07.svh"
  `include "tb/tb_test_08.svh"
  `include "tb/tb_test_09.svh"
  `include "tb/tb_test_10.svh"
  `include "tb/tb_test_11.svh"
  `include "tb/tb_test_12.svh"
  `include "tb/tb_test_13.svh"
  `include "tb/tb_test_14.svh"
  `include "tb/tb_test_15.svh"
  `include "tb/tb_test_16.svh"
  `include "tb/tb_test_17.svh"
  `include "tb/tb_test_18.svh"
  `include "tb/tb_test_19.svh"
  `include "tb/tb_test_20.svh"
  `include "tb/tb_test_yunet.svh"

  // output monitor — always-on capture (raw SA output)
  always @(posedge clk)
    if (m_axis_tvalid && m_axis_tready) begin
      for (int i = 0; i < SIZE; i++) result[out_count][i] = m_axis_tdata[i*32+:32];
      result_tlast[out_count] = m_axis_tlast;
      out_count++;
    end

  // quantizer output monitor
  int q_out_count;
    int q_result[4096][4];
    bit  q_result_tlast[4096];

    // YuNet test data
    int yunet_W[4][4] = '{'{ -1,  -8, -15, -30}, '{-30,   2,   2, -16}, '{  6,  11,  71,  35}, '{  9, -67, -48,  -9}};
    int yunet_A[4][4] = '{'{ 11,  21,  35, -37}, '{-26, -19,   6,  19}, '{ 31, -15,  31,  20}, '{  6,  25, -15, -20}};
  always @(posedge clk)
    if (q_m_axis_tvalid) begin
      for (int i = 0; i < SIZE; i++) q_result[q_out_count][i] = $signed(q_m_axis_tdata[i*8+:8]);
      q_result_tlast[q_out_count] = q_m_axis_tlast;
      q_out_count++;
    end

  // -----------------------------------------------------------------------
  // test harness
  // -----------------------------------------------------------------------
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

    // Compute expected once
    matmul(A, B1, exp_B1);
    matmul(A, B2, exp_B2);
    matmul(A, B_eye, exp_eye);
    matmul(A, B1, exp_AB1);
    matmul(A, B1, exp_A1B);
    matmul(A2, B1, exp_A2B);
    for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) exp_ring_acc[r][c] = exp_A1B[r][c] + exp_A2B[r][c];
    Bneg[0][0] = -1;
    Bneg[0][1] = 2;
    Bneg[0][2] = -3;
    Bneg[0][3] = 4;
    Bneg[1][0] = 5;
    Bneg[1][1] = -6;
    Bneg[1][2] = 7;
    Bneg[1][3] = -8;
    Bneg[2][0] = -9;
    Bneg[2][1] = 10;
    Bneg[2][2] = -11;
    Bneg[2][3] = 12;
    Bneg[3][0] = 13;
    Bneg[3][1] = -14;
    Bneg[3][2] = 15;
    Bneg[3][3] = -16;
    matmul(A, Bneg, exp_neg);
    for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) begin
      exp_acc[r][c]  = exp_B1[r][c] + exp_B2[r][c];
      exp_3way[r][c] = exp_B1[r][c] + exp_B2[r][c] + exp_eye[r][c];
    end

    test_01_single_multiply();
    test_02_backtoback();
    test_03_accumulation_accout0();
    test_04_accumulation_accout1();
    test_05_eye();
    test_06_consecutive_groups();
    test_07_undersized();
    test_08_output_pressure();
    test_09_continuous_streaming();
    test_10_random_backpressure();
    test_11_ring_multi_a();
    test_12_ring_accumulation();
    test_13_c_matrix();
    test_14_negative();
    test_15_state_fuzz();
    test_16_softrst_in_loadc();
    test_17_deadlock_stress();
    test_18_single_element_ring();
    test_19_quantizer_identity();
    test_20_quantizer_scale();
    test_21_quantizer_shift();
    test_22_quantizer_zp();
    // test_yunet_pw();  // WIP: state isolation
    // test_23_quantizer_negative();  // WIP: stream_mat hangs
    // test_24_quantizer_mixed();

    // ═════════════════════════════════════════════════════════════════════
    // Summary
    // ═════════════════════════════════════════════════════════════════════
    if (errors != 0) $display("\nFAIL: %0d total errors", errors);
    else $display("\nPASS: all tests passed");
    $finish;
  end

  initial
    #5000 begin
      $display("\nTIMEOUT: simulation took too long");
      $finish;
    end

endmodule
