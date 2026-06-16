`timescale 1ns / 1ps
// SAME-padding depthwise conv smoke test.
// Check: out_count = H×W, outputs non-zero.
module tb_dw;
  localparam DW=8, CH=2, W=4, H=4;

  logic clk=0; always #5 clk=~clk;
  logic rst_n;
  logic [CH*DW-1:0] s_data, m_data;
  logic s_valid, s_ready, s_last, m_valid, m_ready, m_last;
  int out=0, err=0;

  logic c_awvalid, c_awready, c_wvalid, c_wready, c_bvalid, c_bready;
  logic [7:0] c_awaddr; logic [31:0] c_wdata;

  depthwise_conv3x3_sv #(.DATA_WIDTH(DW),.CHANNELS(CH),.IMG_W(W),.IMG_H(H)) dut (
    .clk,.rst_n,
    .s_axis_tdata(s_data),.s_axis_tvalid(s_valid),
    .s_axis_tready(s_ready),.s_axis_tlast(s_last),
    .m_axis_tdata(m_data),.m_axis_tvalid(m_valid),
    .m_axis_tready(m_ready),.m_axis_tlast(m_last),
    .s_axil_awvalid(c_awvalid),.s_axil_awready(c_awready),
    .s_axil_awaddr(c_awaddr),.s_axil_wdata(c_wdata),
    .s_axil_wvalid(c_wvalid),.s_axil_wready(c_wready),
    .s_axil_bresp(),.s_axil_bvalid(c_bvalid),.s_axil_bready(c_bready),
    .s_axil_arvalid(0),.s_axil_arready(),.s_axil_araddr(0),
    .s_axil_rdata(),.s_axil_rresp(),.s_axil_rvalid(),.s_axil_rready(0));

  task axw(input [7:0] a, input [31:0] d);
    @(posedge clk); c_awvalid=1; c_awaddr=a; c_wdata=d; c_wvalid=1;
    @(posedge clk); c_awvalid=0; c_wvalid=0;
    @(posedge clk); c_bready=1;
    @(posedge clk); c_bready=0;
  endtask

  initial begin
    rst_n=0; s_valid=0; s_last=0; m_ready=1;
    c_awvalid=0; c_wvalid=0; c_bready=0;
    repeat (5) @(posedge clk); rst_n=1; @(posedge clk);

    axw(8'h00, 0);  // zp_in=0
    for (int i=0; i<9*CH; i++) axw(8'h10+4*i, 1);  // weights=1
    @(posedge clk);

    for (int r=0; r<H; r++)
      for (int c=0; c<W; c++) begin
        @(negedge clk);
        s_data = 0;
        s_data[0+:DW] = r*W + c + 1;
        s_valid=1;
        s_last=(r==H-1 && c==W-1);
        while (!s_ready) @(posedge clk);
        @(negedge clk); s_valid=0;
      end
    s_last=0;

    repeat (H*W*3) @(posedge clk);
    $display("out=%0d (expect %0d)", out, H*W);
    if (out != H*W) err++;
    if (out == 0)   err++;
    if (err==0) $display("ALL PASSED");
    else $display("FAIL");
    $finish;
  end

  always @(posedge clk)
    if (m_valid && m_ready) begin
      out <= out + 1;
      if (m_data[0+:DW] == 0) begin
        $display("WARN: ch0=0 at out %0d", out);
        err++;
      end
    end
  initial begin #200000; $display("TIMEOUT"); $finish; end
endmodule
