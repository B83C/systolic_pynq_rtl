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

  always #1 clk = ~clk;

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

  int A[4][4] = '{'{10, 11, 12, 13}, '{11, 12, 13, 14}, '{12, 13, 14, 15}, '{13, 14, 15, 16}};
  int B_id[4][4] = '{'{1, 0, 0, 0}, '{0, 1, 0, 0}, '{0, 0, 1, 0}, '{0, 0, 0, 1}};

  int out_count;
  int errors;
  int result[4][4];
  int B_nontriv[4][4];
  int expected2[4][4];

  always @(posedge clk) begin
    if (m_axis_tvalid && m_axis_tready) begin
      for (int i = 0; i < SIZE; i++)
        result[out_count][i] = m_axis_tdata[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT];
      out_count++;
    end
  end

  task check_result(int expected[4][4]);
    for (int r = 0; r < SIZE; r++) begin
      for (int c = 0; c < SIZE; c++) begin
        if (result[r][c] !== expected[r][c]) begin
          $display("  MISMATCH [%0d][%0d]: got %0d, expected %0d",
                   r, c, result[r][c], expected[r][c]);
          errors++;
        end
      end
    end
    if (errors == 0)
      $display("  PASS: all %0dx%0d results correct", SIZE, SIZE);
    else
      $display("  FAIL: %0d errors", errors);
  endtask

  initial begin
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
    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);

    // TEST 1: Identity B
    $display("=== TEST 1: Identity B ===");
    out_count = 0;
    axil_write(4'h0, 32'h1);
    for (int i = 0; i < 4; i++) send_both(A[i], B_id[i], i == 3);
    wait (m_axis_tlast);
    @(posedge clk); @(posedge clk); @(posedge clk);
    check_result(A);

    // TEST 2: Non-identity B
    $display("=== TEST 2: Non-identity B ===");
    out_count = 0;
    axil_write(4'h0, 32'h1);
    for (int r = 0; r < 4; r++)
      for (int c = 0; c < 4; c++)
        B_nontriv[r][c] = r*10 + c;
    for (int r = 0; r < 4; r++)
      for (int c = 0; c < 4; c++) begin
        expected2[r][c] = 0;
        for (int k = 0; k < 4; k++)
          expected2[r][c] += A[r][k] * B_nontriv[k][c];
      end
    for (int i = 0; i < 4; i++) send_both(A[i], B_nontriv[i], i == 3);
    wait (m_axis_tlast);
    @(posedge clk); @(posedge clk); @(posedge clk);
    check_result(expected2);

    // TEST 3: Sequential runs
    $display("=== TEST 3: Two sequential runs ===");
    for (int run = 0; run < 2; run++) begin
      out_count = 0;
      axil_write(4'h0, 32'h1);
      for (int i = 0; i < 4; i++) send_both(A[i], B_nontriv[i], i == 3);
      wait (m_axis_tlast);
      @(posedge clk); @(posedge clk); @(posedge clk);
      if (run == 1) check_result(expected2);
    end

    if (errors != 0) $display("\nFAIL: %0d errors", errors);
    else             $display("\nPASS: all tests passed");
    $finish;
  end

endmodule
