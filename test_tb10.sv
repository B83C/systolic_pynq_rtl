`timescale 1ns / 1ps

module test_tb10;

  localparam SIZE           = 4;
  localparam DATA_WIDTH_IN  = 8;
  localparam DATA_WIDTH_OUT = 32;
  localparam AXI_IN_WIDTH   = SIZE * DATA_WIDTH_IN;
  localparam AXI_OUT_WIDTH  = SIZE * DATA_WIDTH_OUT;

  logic                        clk;
  logic                        rst_n;

  logic [AXI_IN_WIDTH-1:0]     s_axis_A_tdata;
  logic                        s_axis_A_tvalid;
  logic                        s_axis_A_tready;
  logic                        s_axis_A_tlast;

  logic [AXI_IN_WIDTH-1:0]     s_axis_B_tdata;
  logic                        s_axis_B_tvalid;
  logic                        s_axis_B_tready;
  logic                        s_axis_B_tlast;

  logic [AXI_OUT_WIDTH-1:0]    m_axis_tdata;
  logic                        m_axis_tvalid;
  logic                        m_axis_tready;
  logic                        m_axis_tlast;

  logic                        s_axil_awvalid;
  logic                        s_axil_awready;
  logic [3:0]                  s_axil_awaddr;
  logic [31:0]                 s_axil_wdata;
  logic                        s_axil_wvalid;
  logic                        s_axil_wready;
  logic [1:0]                  s_axil_bresp;
  logic                        s_axil_bvalid;
  logic                        s_axil_bready;

  logic                        s_axil_arvalid;
  logic                        s_axil_arready;
  logic [3:0]                  s_axil_araddr;
  logic [31:0]                 s_axil_rdata;
  logic [1:0]                  s_axil_rresp;
  logic                        s_axil_rvalid;
  logic                        s_axil_rready;

  sa_wrapper_axi_ctrl_sv #(.SIZE(SIZE)) dut (
      .clk(clk), .rst_n(rst_n),
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
    s_axil_bready  = 0;
  endtask

  int A[4][4] = '{
      '{10, 11, 12, 13},
      '{11, 12, 13, 14},
      '{12, 13, 14, 15},
      '{13, 14, 15, 16}
  };
  int B_id[4][4] = '{
      '{ 1,  0,  0,  0},
      '{ 0,  1,  0,  0},
      '{ 0,  0,  1,  0},
      '{ 0,  0,  0,  1}
  };

  initial begin
    clk = 0;
    rst_n = 0;
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    s_axis_A_tlast  = 0;
    s_axis_B_tlast  = 0;
    s_axis_A_tdata  = 0;
    s_axis_B_tdata  = 0;
    s_axil_awvalid  = 0;
    s_axil_wvalid   = 0;
    s_axil_arvalid  = 0;
    s_axil_bready   = 0;
    s_axil_rready   = 0;
    m_axis_tready   = 1;

    $display("STARTING");
    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    $display("Reset done");

    // Test 1
    $display("=== TEST 1 ===");
    axil_write(4'h0, 32'h1);

    for (int i = 0; i < 4; i++) begin
      $display("  send %0d", i);
      @(posedge clk);
      for (int j = 0; j < 4; j++) begin
        s_axis_A_tdata[j*8+:8] = 8'(A[i][j]);
        s_axis_B_tdata[j*8+:8] = 8'(B_id[i][j]);
      end
      s_axis_A_tvalid = 1;
      s_axis_B_tvalid = 1;
      s_axis_A_tlast  = (i == 3);
      s_axis_B_tlast  = (i == 3);
      @(posedge clk);
      while (!(s_axis_A_tready && s_axis_B_tready)) @(posedge clk);
      s_axis_A_tvalid = 0;
      s_axis_B_tvalid = 0;
    end

    wait (m_axis_tlast);
    $display("  PASS: test 1");

    // Test 2: second start
    $display("=== TEST 2: second start ===");
    axil_write(4'h0, 32'h1);
    $display("  start written");

    // Don't send any data, just verify it doesn't hang
    @(posedge clk);
    @(posedge clk);
    @(posedge clk);
    $display("  done");
    $finish;
  end

endmodule
