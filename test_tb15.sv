`timescale 1ns / 1ps

module test_tb15;

  localparam SIZE           = 4;
  localparam AXI_IN_WIDTH   = SIZE * 8;

  logic                        clk, rst_n;
  logic [AXI_IN_WIDTH-1:0]     s_axis_A_tdata, s_axis_B_tdata;
  logic                        s_axis_A_tvalid, s_axis_A_tready, s_axis_A_tlast;
  logic                        s_axis_B_tvalid, s_axis_B_tready, s_axis_B_tlast;
  logic [127:0]                m_axis_tdata;
  logic                        m_axis_tvalid, m_axis_tready, m_axis_tlast;

  logic                        s_axil_awvalid, s_axil_awready;
  logic [3:0]                  s_axil_awaddr;
  logic [31:0]                 s_axil_wdata;
  logic                        s_axil_wvalid, s_axil_wready;
  logic [1:0]                  s_axil_bresp;
  logic                        s_axil_bvalid, s_axil_bready;
  logic                        s_axil_arvalid, s_axil_arready;
  logic [3:0]                  s_axil_araddr;
  logic [31:0]                 s_axil_rdata;
  logic [1:0]                  s_axil_rresp;
  logic                        s_axil_rvalid, s_axil_rready;

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

  always #1 clk = ~clk;

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
    $display("START"); $fflush();
    clk = 0; rst_n = 0;
    s_axis_A_tvalid = 0; s_axis_B_tvalid = 0;
    s_axis_A_tlast = 0; s_axis_B_tlast = 0;
    s_axis_A_tdata = 0; s_axis_B_tdata = 0;
    s_axil_awvalid = 0; s_axil_wvalid = 0;
    s_axil_arvalid = 0; s_axil_bready = 0; s_axil_rready = 0;
    m_axis_tready = 1;

    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);
    $display("RESET_DONE"); $fflush();

    // === TEST 1 ===
    $display("=== T1 ==="); $fflush();
    @(posedge clk);
    s_axil_awvalid = 1; s_axil_awaddr = 0; s_axil_wdata = 1; s_axil_wvalid = 1;
    @(posedge clk);
    while (!s_axil_bvalid) @(posedge clk);
    s_axil_awvalid = 0; s_axil_wvalid = 0; s_axil_bready = 1;
    @(posedge clk);
    s_axil_bready = 0;
    $display("T1 start done"); $fflush();

    for (int i = 0; i < 4; i++) begin
      @(posedge clk);
      for (int j = 0; j < 4; j++) begin
        s_axis_A_tdata[j*8+:8] = 8'(A[i][j]);
        s_axis_B_tdata[j*8+:8] = 8'(B_id[i][j]);
      end
      s_axis_A_tvalid = 1; s_axis_B_tvalid = 1;
      s_axis_A_tlast = (i == 3); s_axis_B_tlast = (i == 3);
      @(posedge clk);
      while (!(s_axis_A_tready && s_axis_B_tready)) @(posedge clk);
      s_axis_A_tvalid = 0; s_axis_B_tvalid = 0;
    end
    $display("T1 data sent"); $fflush();
    wait (m_axis_tlast);
    $display("T1 done"); $fflush();
    @(posedge clk);

    // === TEST 2 ===
    $display("=== T2 ==="); $fflush();
    @(posedge clk);
    s_axil_awvalid = 1; s_axil_awaddr = 0; s_axil_wdata = 1; s_axil_wvalid = 1;
    @(posedge clk);
    while (!s_axil_bvalid) @(posedge clk);
    s_axil_awvalid = 0; s_axil_wvalid = 0; s_axil_bready = 1;
    @(posedge clk);
    s_axil_bready = 0;
    $display("T2 start done"); $fflush();
    $display("T2: state=%0d ctrl_start=%0d ctrl_done=%0d a_loading_done=%0d b_loading_done=%0d", 
             dut.state, dut.ctrl_start, dut.ctrl_done, dut.a_loading_done, dut.b_loading_done); $fflush();

    // Try sending one item
    $display("T2: attempting send"); $fflush();
    @(posedge clk);
    for (int j = 0; j < 4; j++) begin
      s_axis_A_tdata[j*8+:8] = 8'(A[0][j]);
      s_axis_B_tdata[j*8+:8] = 8'(B_id[0][j]);
    end
    s_axis_A_tvalid = 1; s_axis_B_tvalid = 1;
    s_axis_A_tlast = 0; s_axis_B_tlast = 0;
    @(posedge clk);
    $display("T2: tvalid set, state=%0d tready_A=%0d tready_B=%0d both_valid=%0d can_output=%0d draining=%0d",
             dut.state, s_axis_A_tready, s_axis_B_tready, 
             dut.a_treat_valid && dut.b_treat_valid,
             !dut.m_axis_tvalid || m_axis_tready,
             dut.draining); $fflush();
    while (!(s_axis_A_tready && s_axis_B_tready)) begin
      @(posedge clk);
      $display("T2: waiting... state=%0d tready_A=%0d tready_B=%0d", dut.state, s_axis_A_tready, s_axis_B_tready); $fflush();
    end
    $display("T2: ready!"); $fflush();
    s_axis_A_tvalid = 0; s_axis_B_tvalid = 0;

    $display("ALL_DONE"); $fflush();
    $finish;
  end

endmodule
