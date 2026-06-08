`timescale 1ns / 1ps

module sa_wrapper_axi_ctrl_tb;

  localparam SIZE = 4;
  localparam DATA_WIDTH_IN = 8;
  localparam DATA_WIDTH_OUT = 32;
  localparam AXI_IN_WIDTH = SIZE * DATA_WIDTH_IN;
  localparam AXI_OUT_WIDTH = SIZE * DATA_WIDTH_OUT;

  logic                     clk;
  logic                     rst_n;

  logic [ AXI_IN_WIDTH-1:0] s_axis_A_tdata;
  logic                     s_axis_A_tvalid;
  logic                     s_axis_A_tready;
  logic                     s_axis_A_tlast;

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
  logic [              3:0] s_axil_awaddr;
  logic [             31:0] s_axil_wdata;
  logic                     s_axil_wvalid;
  logic                     s_axil_wready;
  logic [              1:0] s_axil_bresp;
  logic                     s_axil_bvalid;
  logic                     s_axil_bready;

  logic                     s_axil_arvalid;
  logic                     s_axil_arready;
  logic [              3:0] s_axil_araddr;
  logic [             31:0] s_axil_rdata;
  logic [              1:0] s_axil_rresp;
  logic                     s_axil_rvalid;
  logic                     s_axil_rready;

  sa_wrapper_axi_ctrl_sv #(
      .SIZE(SIZE),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) dut (
      .clk(clk),
      .rst_n(rst_n),
      .s_axis_A_tdata (s_axis_A_tdata),
      .s_axis_A_tvalid(s_axis_A_tvalid),
      .s_axis_A_tready(s_axis_A_tready),
      .s_axis_A_tlast (s_axis_A_tlast),
      .s_axis_B_tdata (s_axis_B_tdata),
      .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready),
      .s_axis_B_tlast (s_axis_B_tlast),
      .m_axis_tdata   (m_axis_tdata),
      .m_axis_tvalid  (m_axis_tvalid),
      .m_axis_tready  (m_axis_tready),
      .m_axis_tlast   (m_axis_tlast),
      .s_axil_awvalid (s_axil_awvalid),
      .s_axil_awready (s_axil_awready),
      .s_axil_awaddr  (s_axil_awaddr),
      .s_axil_wdata   (s_axil_wdata),
      .s_axil_wvalid  (s_axil_wvalid),
      .s_axil_wready  (s_axil_wready),
      .s_axil_bresp   (s_axil_bresp),
      .s_axil_bvalid  (s_axil_bvalid),
      .s_axil_bready  (s_axil_bready),
      .s_axil_arvalid (s_axil_arvalid),
      .s_axil_arready (s_axil_arready),
      .s_axil_araddr  (s_axil_araddr),
      .s_axil_rdata   (s_axil_rdata),
      .s_axil_rresp   (s_axil_rresp),
      .s_axil_rvalid  (s_axil_rvalid),
      .s_axil_rready  (s_axil_rready)
  );

  // clock
  always #1 clk = ~clk;

  // ======================================================================
  // AXI-Lite driver
  // ======================================================================
  task axil_write(input [3:0] addr, input [31:0] data);
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

  // ======================================================================
  // AXI-Stream helpers
  // ======================================================================
  task automatic send_both(input int Arow[4], input int Brow[4], input bit last);
    @(posedge clk);
    for (int i = 0; i < 4; i++) begin
      s_axis_A_tdata[i*8+:8] = 8'(Arow[i]);
      s_axis_B_tdata[i*8+:8] = 8'(Brow[i]);
    end
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 1;
    s_axis_A_tlast  = last;
    s_axis_B_tlast  = last;
    @(posedge clk);
    while (!(s_axis_A_tready && s_axis_B_tready)) @(posedge clk);
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
  endtask

  task automatic send_A(input int Arow[4], input bit last);
    @(posedge clk);
    for (int i = 0; i < 4; i++) s_axis_A_tdata[i*8+:8] = 8'(Arow[i]);
    s_axis_A_tvalid = 1;
    s_axis_A_tlast  = last;
    @(posedge clk);
    while (!s_axis_A_tready) @(posedge clk);
    s_axis_A_tvalid = 0;
  endtask

  task automatic send_B(input int Brow[4], input bit last);
    @(posedge clk);
    for (int i = 0; i < 4; i++) s_axis_B_tdata[i*8+:8] = 8'(Brow[i]);
    s_axis_B_tvalid = 1;
    s_axis_B_tlast  = last;
    @(posedge clk);
    while (!s_axis_B_tready) @(posedge clk);
    s_axis_B_tvalid = 0;
  endtask

  // ======================================================================
  // Test data
  // ======================================================================
  int A[4][4] = '{'{10, 11, 12, 13}, '{11, 12, 13, 14}, '{12, 13, 14, 15}, '{13, 14, 15, 16}};
  int B_id[4][4] = '{'{1, 0, 0, 0}, '{0, 1, 0, 0}, '{0, 0, 1, 0}, '{0, 0, 0, 1}};
  int A_fill_row[4] = '{10, 11, 12, 13};
  int zero_row[4] = '{0, 0, 0, 0};

  int out_count;
  int errors;

  // ======================================================================
  // Main test sequence
  // ======================================================================
  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_wrapper_axi_ctrl_tb);

    clk = 0;
    rst_n = 0;
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;
    s_axis_A_tdata = 0;
    s_axis_B_tdata = 0;
    s_axil_awvalid = 0;
    s_axil_wvalid = 0;
    s_axil_arvalid = 0;
    s_axil_bready = 0;
    s_axil_rready = 0;
    m_axis_tready = 1;
    errors = 0;
    out_count = 0;

    $display("STARTING reset sequence");
    $fflush();
    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    $display("Reset done, calling axil_write");
    $fflush();

    // =====================================================================
    // TEST 1: Normal mode (both normal)
    // =====================================================================
    $display("=== TEST 1: Normal mode (both streaming) ===");
    $fflush();
    axil_write(4'h0, 32'h1);  // start

    for (int i = 0; i < 4; i++) send_both(A[i], B_id[i], i == 3);

    // drain should produce some outputs
    wait (m_axis_tlast);
    @(posedge clk);
    @(posedge clk);
    @(posedge clk);
    $display("  PASS: normal mode no deadlock");
    $fflush();
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;

    // =====================================================================
    // TEST 2: B reuse mode
    // =====================================================================
    $display("=== TEST 2: B reuse mode ===");
    $fflush();
    axil_write(4'h4, 32'h4);  // mode: b_reuse=1
    axil_write(4'h0, 32'h1);  // start

    // Load B (4 beats with both A and B valid)
    for (int i = 0; i < 4; i++) send_both(A[i], B_id[i], 0);

    // After loading: B in reuse, A still streaming.
    // Stream A data only (B input is ignored)
    for (int i = 0; i < 4; i++) send_A(A[i], i == 3);

    wait (m_axis_tlast);
    @(posedge clk);
    $display("  PASS: B reuse no deadlock");
    $fflush();
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;

    // =====================================================================
    // TEST 3: A reuse mode
    // =====================================================================
    $display("=== TEST 3: A reuse mode ===");
    axil_write(4'h8, 32'd12);  // loop_len_a = 12
    axil_write(4'h4, 32'h1);  // mode: a_reuse=1
    axil_write(4'h0, 32'h1);  // start

    // Load A for 12 beats (with B data)
    for (int i = 0; i < 12; i++) send_both(A_fill_row, B_id[0], 0);

    // After loading: A in reuse, B streaming.
    // Stream B data only (A input is ignored)
    for (int i = 0; i < 4; i++) begin
      zero_row[i] = 1;
      send_B(zero_row, i == 3);
      zero_row[i] = 0;
    end

    wait (m_axis_tlast);
    @(posedge clk);
    $display("  PASS: A reuse no deadlock");
    $fflush();
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;

    // =====================================================================
    // TEST 4: Both reuse
    // =====================================================================
    $display("=== TEST 4: Both reuse mode ===");
    $fflush();
    axil_write(4'h8, 32'd4);  // loop_len_a = 4
    axil_write(4'h4, 32'h5);  // mode: a_reuse=1, b_reuse=1
    axil_write(4'h0, 32'h1);  // start

    // Load both (4 beats)
    for (int i = 0; i < 4; i++) send_both(A[i], B_id[i], i == 3);

    // compute_cnt will trigger drain after loop_len_a=4 compute cycles
    wait (m_axis_tlast);
    @(posedge clk);
    $display("  PASS: both reuse no deadlock");
    $fflush();

    // =====================================================================
    // Summary
    // =====================================================================
    if (errors != 0) $display("\nFAIL: %0d errors", errors);
    else $display("\nPASS: all tests passed");
    $fflush();
    $finish;
  end

endmodule
