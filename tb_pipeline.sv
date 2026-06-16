`timescale 1ns / 1ps
`include "defs.svh"
`include "SA.sv"
`include "pe.sv"
`include "counter.sv"

// Full pipeline: chlast → SA → quantizer → tiled_to_chlast
// 16×16 GEMM on 4×4 SA. FB_CNT=3, repeat_cnt=3.
// A: block-major via chlast bypass, B: channel-last via chlast normal.

module tb_pipeline;

  localparam SZ = 4, DWI = 8, DWO = 32, DWO_Q = 8;
  localparam IW = SZ * DWI, OW = SZ * DWO;
  localparam CH_PER = 4, CH = 16, OUT_COL = 4;

  logic clk, rst_n;

  logic [CH_PER*DWI-1:0] cl_in;
  logic cl_v, cl_r, cl_l;
  logic [OUT_COL*DWI-1:0] cl_out;
  logic cl_ov, cl_or, cl_ol;
  // cl_bypass is now controlled via CL_REG_BYPASS AXI-Lite register on
  // the chlast_to_tiled module.  No top-level signal needed.

  logic [IW-1:0] sa_in;
  logic sa_iv, sa_ir, sa_il;
  logic [OW-1:0] sa_out;
  logic sa_ov, sa_or, sa_ol;

  logic sav, saa, wv, wr, br, bv, arv, ara, rv, rr;
  logic [6:0] sa_addr;
  logic [31:0] wd, rd;
  logic [1:0] d0, d1;
  logic abp, axp, idl;

  wire [SZ*DWO_Q-1:0] q_out;
  logic q_ov, q_or, q_ol;
  logic [CH_PER*DWO_Q-1:0] tc_out;
  logic tc_ov, tc_or, tc_ol;

  logic cl_cfg_wen, cl_rpt_wen;
  logic sa_mq_wen, sa_sh_wen, sa_zo_wen;
  initial begin
    cl_cfg_wen = 0;
    cl_rpt_wen = 0;
    sa_mq_wen  = 0;
    sa_sh_wen  = 0;
    sa_zo_wen  = 0;
  end

  // chlast_to_tiled AXI-Lite
  logic        cl_awvalid;
  wire         cl_awready;
  logic [ 3:0] cl_awaddr;
  logic [31:0] cl_wdata;
  logic        cl_wvalid;
  wire         cl_wready;
  wire  [ 1:0] cl_bresp;
  wire         cl_bvalid;
  logic        cl_bready;
  logic        cl_arvalid;
  wire         cl_arready;
  logic [ 3:0] cl_araddr;
  wire  [31:0] cl_rdata;
  wire  [ 1:0] cl_rresp;
  wire         cl_rvalid;
  logic        cl_rready;
  localparam CL_REG_CH = 4'h0;
  localparam CL_REG_RPT = 4'h4;
  localparam CL_REG_BYPASS = 4'h8;

  // quantizer AXI-Lite
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
  localparam Q_REG_MUL = 4'h0;
  localparam Q_REG_SH = 4'h4;
  localparam Q_REG_ZO = 4'h8;

  // tiled_to_chlast AXI-Lite
  logic        tc_awvalid;
  wire         tc_awready;
  logic [ 3:0] tc_awaddr;
  logic [31:0] tc_wdata;
  logic        tc_wvalid;
  wire         tc_wready;
  wire  [ 1:0] tc_bresp;
  wire         tc_bvalid;
  logic        tc_bready;
  logic        tc_arvalid;
  wire         tc_arready;
  logic [ 3:0] tc_araddr;
  wire  [31:0] tc_rdata;
  wire  [ 1:0] tc_rresp;
  wire         tc_rvalid;
  logic        tc_rready;
  localparam TC_REG_CH = 4'h0;
  localparam TC_REG_BYPASS = 4'h4;

  chlast_to_tiled_sv #(
      .DATA_WIDTH(DWI),
      .CH_PER_BEAT(CH_PER),
      .MAX_CHANNELS(CH),
      .OUT_COL(OUT_COL),
      .MAX_REPLAY_CNT(16)
  ) u_chlast (
      .clk,
      .rst_n,
      .s_axis_tdata  (cl_in),
      .s_axis_tvalid (cl_v),
      .s_axis_tready (cl_r),
      .s_axis_tlast  (cl_l),
      .m_axis_tdata  (cl_out),
      .m_axis_tvalid (cl_ov),
      .m_axis_tready (cl_or),
      .m_axis_tlast  (cl_ol),
      .s_axil_awvalid(cl_awvalid),
      .s_axil_awready(cl_awready),
      .s_axil_awaddr (cl_awaddr),
      .s_axil_wdata  (cl_wdata),
      .s_axil_wvalid (cl_wvalid),
      .s_axil_wready (cl_wready),
      .s_axil_bresp  (cl_bresp),
      .s_axil_bvalid (cl_bvalid),
      .s_axil_bready (cl_bready),
      .s_axil_arvalid(cl_arvalid),
      .s_axil_arready(cl_arready),
      .s_axil_araddr (cl_araddr),
      .s_axil_rdata  (cl_rdata),
      .s_axil_rresp  (cl_rresp),
      .s_axil_rvalid (cl_rvalid),
      .s_axil_rready (cl_rready)
  );

  sa_wrapper_axi_ctrl_sv #(
      .SIZE(SZ),
      .A_DEPTH(16),
      .C_DEPTH(8),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO)
  ) u_sa (
      .clk,
      .rst_n,
      .s_axis_B_tdata(sa_in),
      .s_axis_B_tvalid(sa_iv),
      .s_axis_B_tready(sa_ir),
      .s_axis_B_tlast(sa_il),
      .m_axis_tdata(sa_out),
      .m_axis_tvalid(sa_ov),
      .m_axis_tready(sa_or),
      .m_axis_tlast(sa_ol),
      .s_axil_awvalid(sav),
      .s_axil_awready(saa),
      .s_axil_awaddr(sa_addr),
      .s_axil_wdata(wd),
      .s_axil_wvalid(wv),
      .s_axil_wready(wr),
      .s_axil_bresp(d0),
      .s_axil_bvalid(bv),
      .s_axil_bready(br),
      .s_axil_arvalid(arv),
      .s_axil_arready(ara),
      .s_axil_araddr(sa_addr),
      .s_axil_rdata(rd),
      .s_axil_rresp(d1),
      .s_axil_rvalid(rv),
      .s_axil_rready(rr)
  );

  quantizer #(
      .SIZE(SZ),
      .DATA_WIDTH_IN(DWO),
      .DATA_WIDTH_OUT(DWO_Q)
  ) u_quant (
      .clk,
      .rst_n,
      .s_axil_awvalid(q_awvalid),
      .s_axil_awready(q_awready),
      .s_axil_awaddr (q_awaddr),
      .s_axil_wdata  (q_wdata),
      .s_axil_wvalid (q_wvalid),
      .s_axil_wready (q_wready),
      .s_axil_bresp  (q_bresp),
      .s_axil_bvalid (q_bvalid),
      .s_axil_bready (q_bready),
      .s_axil_arvalid(q_arvalid),
      .s_axil_arready(q_arready),
      .s_axil_araddr (q_araddr),
      .s_axil_rdata  (q_rdata),
      .s_axil_rresp  (q_rresp),
      .s_axil_rvalid (q_rvalid),
      .s_axil_rready (q_rready),
      .s_axis_tdata  (sa_out),
      .s_axis_tvalid (sa_ov),
      .s_axis_tready (sa_or),
      .s_axis_tlast  (sa_ol),
      .m_axis_tdata  (q_out),
      .m_axis_tvalid (q_ov),
      .m_axis_tready (q_or),
      .m_axis_tlast  (q_ol)
  );

  tiled_to_chlast_sv #(
      .DATA_WIDTH(DWO_Q),
      .CH_PER_BEAT(CH_PER),
      .MAX_CHANNELS(CH),
      .OUT_COL(OUT_COL)
  ) u_tc (
      .clk,
      .rst_n,
      .s_axis_tdata  (q_out),
      .s_axis_tvalid (q_ov),
      .s_axis_tready (q_or),
      .s_axis_tlast  (q_ol),
      .m_axis_tdata  (tc_out),
      .m_axis_tvalid (tc_ov),
      .m_axis_tready (tc_or),
      .m_axis_tlast  (tc_ol),
      .s_axil_awvalid(tc_awvalid),
      .s_axil_awready(tc_awready),
      .s_axil_awaddr (tc_awaddr),
      .s_axil_wdata  (tc_wdata),
      .s_axil_wvalid (tc_wvalid),
      .s_axil_wready (tc_wready),
      .s_axil_bresp  (tc_bresp),
      .s_axil_bvalid (tc_bvalid),
      .s_axil_bready (tc_bready),
      .s_axil_arvalid(tc_arvalid),
      .s_axil_arready(tc_arready),
      .s_axil_araddr (tc_araddr),
      .s_axil_rdata  (tc_rdata),
      .s_axil_rresp  (tc_rresp),
      .s_axil_rvalid (tc_rvalid),
      .s_axil_rready (tc_rready)
  );

  // Debug wires for waveform
  logic signed [31:0] sa_row[0:3];
  logic signed [ 7:0] q_row [0:3];
  logic signed [ 7:0] tc_row[0:3];
  generate
    for (genvar gi = 0; gi < 4; gi++) begin : gen_dbg
      assign sa_row[gi] = $signed(sa_out[gi*DWO+:DWO]);
      assign q_row[gi]  = $signed(q_out[gi*DWO_Q+:DWO_Q]);
      assign tc_row[gi] = $signed(tc_out[gi*DWO_Q+:DWO_Q]);
    end
  endgenerate
  assign sa_in = cl_out[IW-1:0];
  assign sa_iv = cl_ov;
  assign sa_il = cl_ol;
  assign cl_or = sa_ir;

  always #1 clk = ~clk;
  int errs, oc;
  int res[256][4];

  always @(posedge clk) begin
    if (tc_ov && tc_or) begin
      for (int i = 0; i < SZ; i++) res[oc][i] = $signed(tc_out[i*DWO_Q+:DWO_Q]);
      oc <= oc + 1;
    end
    if (tc_ol) $display("  TC_TLAST at t=%0t ov=%0d or=%0d oc=%0d", $time, tc_ov, tc_or, oc);
    if (cl_v && cl_r) begin
      $write("  CL_IN:  t=%0t ", $time);
      for (int i = CH_PER - 1; i >= 0; i--) $write("%02X", cl_in[i*DWI+:DWI]);
      $display(" last=%0d", cl_l);
    end
    if (cl_ov && cl_or) begin
      $write("  CL_OUT: t=%0t ", $time);
      for (int i = OUT_COL - 1; i >= 0; i--) $write("%02X", cl_out[i*DWI+:DWI]);
      $display(" last=%0d", cl_ol);
    end
    if (sa_ov && sa_or) begin
      $write("  SA:");
      for (int i = 0; i < SZ; i++) $write(" %0d", $signed(sa_out[i*DWO+:DWO]));
      $display();
    end
  end

  task saw(input [6:0] a, input [31:0] d);
    @(posedge clk);
    sa_addr = a;
    sav = 1;
    wd = d;
    wv = 1;
    @(posedge clk);
    sav = 0;
    wv  = 0;
    while (!bv) @(posedge clk);
    br = 1;
    @(posedge clk);
    br = 0;
  endtask

  // AXI-Lite write to chlast_to_tiled slave
  task cl_axil_write(input [3:0] a, input [31:0] d);
    @(posedge clk);
    cl_awvalid = 1;
    cl_awaddr  = a;
    cl_wdata   = d;
    cl_wvalid  = 1;
    @(posedge clk);
    cl_awvalid = 0;
    cl_wvalid  = 0;
    while (!cl_bvalid) @(posedge clk);
    cl_bready = 1;
    @(posedge clk);
    cl_bready = 0;
  endtask

  // AXI-Lite write to quantizer slave
  task q_axil_write(input [3:0] a, input [31:0] d);
    @(posedge clk);
    q_awvalid = 1;
    q_awaddr  = a;
    q_wdata   = d;
    q_wvalid  = 1;
    @(posedge clk);
    q_awvalid = 0;
    q_wvalid  = 0;
    while (!q_bvalid) @(posedge clk);
    q_bready = 1;
    @(posedge clk);
    q_bready = 0;
  endtask

  // AXI-Lite write to tiled_to_chlast slave
  task tc_axil_write(input [3:0] a, input [31:0] d);
    @(posedge clk);
    tc_awvalid = 1;
    tc_awaddr  = a;
    tc_wdata   = d;
    tc_wvalid  = 1;
    @(posedge clk);
    tc_awvalid = 0;
    tc_wvalid  = 0;
    while (!tc_bvalid) @(posedge clk);
    tc_bready = 1;
    @(posedge clk);
    tc_bready = 0;
  endtask

  task send_to_chlast(input logic [IW-1:0] beat, input bit last);
    cl_in = {{(CH_PER - SZ) {1'b0}}, beat};
    cl_v  = 1;
    cl_l  = last;
    @(negedge clk);
    while (!cl_r) @(negedge clk);
    cl_v = 0;
    cl_l = 0;
  endtask

  int A[16][16], B[16][16], E[16][16];
  logic [IW-1:0] beat;

  int A_init[16][16] = '{
      '{4, 5, 4, 4, 5, 5, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4},
      '{2, 3, 2, 2, 3, 3, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3},
      '{2, 3, 2, 2, 3, 3, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2},
      '{4, 5, 4, 4, 5, 5, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4},
      '{4, 5, 4, 4, 5, 5, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4},
      '{1, 2, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3},
      '{2, 3, 2, 2, 3, 3, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3},
      '{1, 2, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 1, 1, 3, 1},
      '{4, 5, 4, 4, 5, 5, 6, 4, 4, 4, 4, 4, 4, 4, 6, 4},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3},
      '{2, 3, 2, 2, 3, 3, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3},
      '{3, 4, 3, 3, 4, 4, 5, 3, 3, 3, 3, 3, 3, 3, 5, 3}
  };
  int B_init[16][16] = '{
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{2, 2, 2, 3, 2, 3, 3, 1, 2, 3, 3, 2, 2, 3, 3, 1},
      '{4, 4, 4, 5, 4, 5, 5, 3, 4, 5, 5, 4, 4, 5, 5, 3},
      '{4, 4, 4, 5, 4, 5, 5, 3, 4, 5, 5, 4, 4, 5, 5, 3},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{4, 4, 4, 5, 4, 5, 5, 3, 4, 5, 5, 4, 4, 5, 5, 3},
      '{2, 2, 2, 3, 2, 3, 3, 1, 2, 3, 3, 2, 2, 3, 3, 1},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{2, 2, 2, 3, 2, 3, 3, 1, 2, 3, 3, 2, 2, 3, 3, 1},
      '{4, 4, 4, 5, 4, 5, 5, 3, 4, 5, 5, 4, 4, 5, 5, 3},
      '{3, 3, 3, 4, 3, 4, 4, 2, 3, 4, 4, 3, 3, 4, 4, 2},
      '{4, 4, 4, 5, 4, 5, 5, 3, 4, 5, 5, 4, 4, 5, 5, 3},
      '{2, 2, 2, 3, 2, 3, 3, 1, 2, 3, 3, 2, 2, 3, 3, 1}
  };
  int B_new[32][16] = '{
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {2, 5, 3, 4, 2, 2, 3, 5, 3, 5, 2, 2, 4, 2, 3, 4},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {1, 4, 2, 3, 1, 1, 2, 4, 2, 4, 1, 1, 3, 1, 2, 3},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5},
      {3, 6, 4, 5, 3, 3, 4, 6, 4, 6, 3, 3, 5, 3, 4, 5}
  };

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, tb_pipeline);
    clk = 0;
    rst_n = 0;
    cl_v = 0;
    cl_l = 0;
    cl_in = 0;
    sav = 0;
    wv = 0;
    br = 0;
    arv = 0;
    rr = 0;
    // Per-slave AXI-Lite init
    cl_awvalid = 0;
    cl_wvalid = 0;
    cl_bready = 0;
    cl_arvalid = 0;
    cl_rready = 0;
    q_awvalid = 0;
    q_wvalid = 0;
    q_bready = 0;
    q_arvalid = 0;
    q_rready = 0;
    tc_awvalid = 0;
    tc_wvalid = 0;
    tc_bready = 0;
    tc_arvalid = 0;
    tc_rready = 0;
    q_or = 1;
    tc_or = 1;
    errs = 0;
    oc = 0;

    // Copy user-provided A and B
    begin
      for (int r = 0; r < 16; r++)
      for (int c = 0; c < 16; c++) begin
        A[r][c] = A_init[r][c];
        B[r][c] = B_init[r][c];
        E[r][c] = 0;
        for (int k = 0; k < 16; k++) E[r][c] += A[r][k] * B[c][k];  // A × B^T
        E[r][c] = E[r][c] / 4 + (r + 1) * 4;  // /4 quant + c_ring bias
      end
    end

    repeat (5) @(posedge clk);
    rst_n = 1;
    @(posedge clk);

    $display("=== 16×16 A × B^T with /4 quant + c_ring bias ===");

    tc_axil_write(TC_REG_BYPASS, 1);
    // ── Load c_ring: 16 values, c[i] = (i+1)*4 ──
    $display("  Loading c_ring: c[i] = (i+1)*4");
    saw(REG_C_LOOP_START, 0);
    saw(REG_C_LOOP_END, 15);
    saw(REG_C_LOAD, 0);
    cl_axil_write(CL_REG_BYPASS, 1);  // bypass chlast for c_ring load
    for (int i = 0; i < 16; i++) begin
      beat  = {32'((i + 1) * 4)};  // c_vals[i] = (i+1)*4
      cl_in = {{(CH_PER - SZ) {1'b0}}, beat};
      cl_v  = 1;
      cl_l  = (i == 15);
      @(negedge clk);
      while (!cl_r) @(negedge clk);
      cl_v = 0;
      cl_l = 0;
    end
    repeat (10) @(posedge clk);

    saw(REG_FB_CNT, 3);
    q_axil_write(Q_REG_MUL, 1);  // REG_MUL_Q = 1, quant passthrough
    q_axil_write(Q_REG_SH, 2);  // REG_SHIFT = 2, divide by 4
    q_axil_write(Q_REG_ZO, 0);  // REG_ZP_OUT = 0
    saw(REG_ZP_IN, 0);
    saw(REG_A_LOOP_START, 0);
    saw(REG_A_LOOP_END, 63);  // 64 A entries (16 blocks × 4 rows)
    saw(REG_C_LOOP_START, 0);
    saw(REG_C_LOOP_END, 15);  // C matrix, every column share the same values, 16 rows

    // cfg_channels -> chlast_to_tiled AND tiled_to_chlast
    cl_axil_write(CL_REG_CH, CH);
    tc_axil_write(TC_REG_CH, CH);
    tc_axil_write(TC_REG_BYPASS, 0);
    cl_axil_write(CL_REG_RPT, 4);  // chlast replay 4 times
    saw(REG_A_LOAD, 0);

    // ── Load A: block-major via chlast bypass ──
    @(negedge clk);
    cl_axil_write(CL_REG_BYPASS, 1);  // bypass chlast for A load
    for (int by = 0; by < 4; by++)
    for (int bx = 0; bx < 4; bx++)
    for (int r = 0; r < 4; r++) begin
      beat = 0;
      for (int c = 0; c < 4; c++) beat[c*DWI+:DWI] = 8'(A[by*4+r][bx*4+c]);
      send_to_chlast(beat, (by == 3 && bx == 3 && r == 3));
    end
    repeat (20) @(posedge clk);

    // ── Stream B: channel-last via chlast normal ──
    cl_axil_write(CL_REG_BYPASS, 0);
    // prime
    cl_in = 0;
    cl_v  = 0;
    cl_l  = 0;
    @(negedge clk);
    while (!cl_r) @(negedge clk);
    cl_v = 0;
    // 16 rows, each row = 4 beats of 4 values
    for (int r = 0; r < 32; r++)
    for (int bc = 0; bc < 4; bc++) begin
      beat = 0;
      for (int c = 0; c < 4; c++) beat[c*DWI+:DWI] = 8'(B_new[r][bc*4+c]);
      send_to_chlast(beat, r == 31 && bc == 3);
    end
    // complete fill with tlast
    cl_in = 0;
    cl_v  = 0;
    cl_l  = 0;
    @(negedge clk);
    while (!cl_r) @(negedge clk);
    cl_v = 0;
    cl_l = 0;

    repeat (500) @(posedge clk);

    $display("  pipeline oc=%0d", oc);
    for (int r = 0; r < 16 && r < oc; r++) begin
      $write("  r%0d:", r);
      for (int c = 0; c < SZ; c++) $write(" %0d", res[r][c]);
      $display(" (exp: %0d %0d %0d %0d)", E[r][0], E[r][1], E[r][2], E[r][3]);
      for (int c = 0; c < SZ; c++)
      if (res[r][c] !== E[r][c]) begin
        $display("    err[%0d][%0d]", r, c);
        errs++;
      end
    end
    if (errs) $display("FAIL: %0d errors", errs);
    else $display("PASS");
    $finish;
  end
  initial
    #2000000 begin
      $display("TIMEOUT");
      $finish;
    end
endmodule
