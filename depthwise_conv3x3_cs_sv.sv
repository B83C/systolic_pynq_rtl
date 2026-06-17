`timescale 1ns / 1ps
// Channel-serial 3×3 depthwise conv, SAME padding, NHWC input.
// 3 BRAMs (row buffers), 9 DSPs (registered multiply), 1 result / 3 cycles.
// CH programmable at runtime.
module depthwise_conv3x3_cs_sv #(
    parameter DATA_WIDTH   = 8,
    parameter MAX_CHANNELS = 64,
    parameter IMG_W        = 320,
    parameter IMG_H        = 240
) (
    input  logic                                clk, rst_n,
    input  logic [MAX_CHANNELS*DATA_WIDTH-1:0]  s_axis_tdata,
    input  logic                                 s_axis_tvalid,
    output logic                                 s_axis_tready,
    input  logic                                 s_axis_tlast,
    output logic [MAX_CHANNELS*DATA_WIDTH-1:0]  m_axis_tdata,
    output logic                                 m_axis_tvalid,
    input  logic                                 m_axis_tready,
    output logic                                 m_axis_tlast,
    input  wire        s_axil_awvalid,  output wire s_axil_awready,
    input  wire [ 7:0] s_axil_awaddr,   input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,   output wire s_axil_wready,
    output wire [ 1:0] s_axil_bresp,    output reg  s_axil_bvalid,
    input  wire        s_axil_bready,   input  wire s_axil_arvalid,
    output wire        s_axil_arready,  input  wire [ 7:0] s_axil_araddr,
    output reg  [31:0] s_axil_rdata,    output wire [ 1:0] s_axil_rresp,
    output reg         s_axil_rvalid,   input  wire s_axil_rready
);

  localparam CHW = $clog2(MAX_CHANNELS+1);
  localparam RWW = $clog2(IMG_W);
  localparam RHW = $clog2(IMG_H+2);

  // AXI-Lite
  localparam REG_CH = 8'h00, REG_ZP = 8'h04, REG_WB = 8'h10;
  logic [CHW-1:0] num_ch;  logic [DATA_WIDTH-1:0] zp_in;
  logic [DATA_WIDTH-1:0] wmem[9 * MAX_CHANNELS];
  wire awen = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire aren = s_axil_arvalid && !s_axil_rvalid;
  assign s_axil_awready = awen; assign s_axil_wready = awen;
  assign s_axil_bresp = 0; assign s_axil_arready = aren; assign s_axil_rresp = 0;
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin s_axil_bvalid <= 0; s_axil_rvalid <= 0; s_axil_rdata <= 0;
      num_ch <= MAX_CHANNELS; zp_in <= 0;
    end else begin
      if (awen) begin
        if (s_axil_awaddr == REG_CH) num_ch <= s_axil_wdata[CHW-1:0];
        else if (s_axil_awaddr == REG_ZP) zp_in <= s_axil_wdata[DATA_WIDTH-1:0];
        else if (s_axil_awaddr >= REG_WB) begin int i = int'(s_axil_awaddr - REG_WB)>>2;
          if (i < 9*MAX_CHANNELS) wmem[i] <= s_axil_wdata[DATA_WIDTH-1:0]; end
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) s_axil_bvalid <= 0;
      if (aren) begin
        if (s_axil_araddr == REG_CH) s_axil_rdata <= num_ch;
        else if (s_axil_araddr == REG_ZP) s_axil_rdata <= zp_in;
        else if (s_axil_araddr >= REG_WB) begin int i = int'(s_axil_araddr - REG_WB)>>2;
          s_axil_rdata <= i < 9*MAX_CHANNELS ? wmem[i] : 0; end
        else s_axil_rdata <= 0; s_axil_rvalid <= 1;
      end else if (s_axil_rready) s_axil_rvalid <= 0;
    end
  end

  // Line buffer: 3 rows × all channels
  (* ram_style = "block" *) logic [MAX_CHANNELS*DATA_WIDTH-1:0] lb[3][IMG_W];
  logic [RWW-1:0] wc; logic [RHW-1:0] wr; logic [1:0] wr3;
  assign s_axis_tready = rst_n;
  wire wi = s_axis_tvalid && s_axis_tready;
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin wc <= 0; wr <= 0; wr3 <= 1; end  // start at 1 for zp_in padding row
    else if (wi) begin
      if (wc == IMG_W-1) begin wc <= 0;
        if (s_axis_tlast) begin wr <= 0; wr3 <= 0; end
        else begin wr <= wr + 1; wr3 <= (wr3 == 2) ? 0 : wr3 + 1; end
      end else wc <= wc + 1;
    end
  end
  always @(posedge clk) if (wi) lb[wr3][wc] <= s_axis_tdata;

  // SAME-padding pixel read
  function [DATA_WIDTH-1:0] rdp(input int r, input int c);
    if (c < 0 || c >= IMG_W) rdp = zp_in;
    else rdp = lb[r][c][DATA_WIDTH*och +: DATA_WIDTH];
  endfunction

  // FSM: 3-phase pipeline per channel
  typedef enum {IDLE, RUN, DRAIN} s_t;
  s_t st;
  logic [RHW-1:0] orow; logic [RWW-1:0] ocol;  // output pos (padded)
  logic [CHW-1:0] och;
  logic [3:0] tap;  // 0..8 for 9 taps
  logic och_last;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) st <= IDLE;
    else begin
      case (st)
        IDLE: if (wr >= 3 && wi && wc == 0) begin
          st <= RUN; orow <= 1; ocol <= 1; och <= 0; tap <= 0;
        end
        RUN: begin
          och_last <= (tap == 8) & (och == num_ch - 1);
          if (tap == 8) begin
            tap <= 0;
            if (och == num_ch - 1) begin
              och <= 0;
              if (ocol == IMG_W) begin ocol <= 1;
                if (orow == IMG_H) st <= DRAIN;
                else orow <= orow + 1;
              end else ocol <= ocol + 1;
            end else och <= och + 1;
          end else tap <= tap + 1;
        end
        DRAIN: st <= IDLE;
      endcase
    end
  end
  logic signed [2*DATA_WIDTH+3:0] acc;
  logic [DATA_WIDTH-1:0] res;

  always @(posedge clk) begin
    if (st == RUN) begin
      int cc = int'(ocol) + int'(tap % 3) - 1;
      automatic logic signed [DATA_WIDTH-1:0] px =
        $signed(rdp((orow - 1 + tap/3) % 3, cc));
      if (tap == 0) acc <= px * $signed(wmem[tap * MAX_CHANNELS + och]);
      else          acc <= acc + px * $signed(wmem[tap * MAX_CHANNELS + och]);
      tap <= (tap == 8) ? 0 : tap + 1;
    end
  end

  // Clamp
  logic vp;
  always @(posedge clk) begin
    vp <= (st == RUN && tap == 8);
    if (vp) begin
      if (acc > (2**(DATA_WIDTH-1)-1)) res <= {1'b0,{DATA_WIDTH-1{1'b1}}};
      else if (acc < -2**(DATA_WIDTH-1)) res <= {1'b1,{DATA_WIDTH-1{1'b0}}};
      else res <= DATA_WIDTH'(acc);
    end
  end

  // Output
  logic [MAX_CHANNELS*DATA_WIDTH-1:0] obuf, obuf2;
  logic [CHW-1:0] vch;  // channel of the result
  logic ep;

  always @(posedge clk) begin
    vch <= (st == RUN && tap == 8) ? och : vch;
    if (vp) begin
      obuf[DATA_WIDTH*vch +: DATA_WIDTH] <= res;
      if (vch == num_ch - 1) begin
        for (int c = 0; c < MAX_CHANNELS; c++)
          obuf2[c*DATA_WIDTH+:DATA_WIDTH] <= obuf[c*DATA_WIDTH+:DATA_WIDTH];
        obuf2[DATA_WIDTH*vch +: DATA_WIDTH] <= res;
        ep <= 1;
      end
    end
    if (ep && (!m_axis_tvalid || m_axis_tready)) begin
      m_axis_tdata <= obuf2; m_axis_tvalid <= 1; m_axis_tlast <= vch == num_ch-1; ep <= 0;
    end else if (!ep) m_axis_tvalid <= 0;
  end
  assign m_axis_tlast = 0;
endmodule
