`timescale 1ns / 1ps

// Channel-last → tiled converter with on-the-fly gather.
// Input writes to buffer + gather (byte per channel per column).  Output
// reads a single gather address per beat — no multiport BRAM reads, no
// GATHERING state, no tvalid gaps.

module chlast_to_tiled_gather_sv #(
    parameter DATA_WIDTH = 8,
    CH_PER_BEAT = 8,
    MAX_CHANNELS = 64,
    OUT_COL = 8,
    MAX_REPLAY_CNT = 16
) (
    input logic clk,
    rst_n,
    input logic [CH_PER_BEAT*DATA_WIDTH-1:0] s_axis_tdata,
    input logic s_axis_tvalid,
    output logic s_axis_tready,
    input logic s_axis_tlast,
    output logic [OUT_COL*DATA_WIDTH-1:0] m_axis_tdata,
    output logic m_axis_tvalid,
    input logic m_axis_tready,
    output logic m_axis_tlast,
    input wire s_axil_awvalid,
    output wire s_axil_awready,
    input wire [3:0] s_axil_awaddr,
    input wire [31:0] s_axil_wdata,
    input wire s_axil_wvalid,
    output wire s_axil_wready,
    output wire [1:0] s_axil_bresp,
    output reg s_axil_bvalid,
    input wire s_axil_bready,
    input wire s_axil_arvalid,
    output wire s_axil_arready,
    input wire [3:0] s_axil_araddr,
    output reg [31:0] s_axil_rdata,
    output wire [1:0] s_axil_rresp,
    output reg s_axil_rvalid,
    input wire s_axil_rready
);

  initial
    assert ((MAX_CHANNELS & (MAX_CHANNELS - 1)) == 0)
    else $fatal(1, "chlast_to_tiled_gather_sv: MAX_CHANNELS must be power of 2");
  localparam unsigned CPB_LOG = $clog2(CH_PER_BEAT);
  localparam CB = MAX_CHANNELS / CH_PER_BEAT, CB_BITS = (CB > 1) ? $clog2(
      CB
  ) : 1, SP_BITS = (CH_PER_BEAT > 2) ? $clog2(
      CH_PER_BEAT
  ) : 1;
  localparam OC_BITS = $clog2(
      OUT_COL
  ), CFG_W = $clog2(
      MAX_CHANNELS + 1
  ), RPT_W = $clog2(
      MAX_REPLAY_CNT + 1
  );
  localparam unsigned BIW = (CH_PER_BEAT * DATA_WIDTH > 1) ? $clog2(CH_PER_BEAT * DATA_WIDTH) : 1;
  localparam GIW = (OUT_COL * DATA_WIDTH > 1) ? $clog2(OUT_COL * DATA_WIDTH) : 1;
  localparam REG_CH = 4'h0, REG_RPT = 4'h4, REG_BP = 4'h8;
  logic bp;

  (* ram_style = "block" *) logic [CH_PER_BEAT*DATA_WIDTH-1:0] bufr[2*OUT_COL*CB];
  (* ram_style = "distributed" *) logic [OUT_COL*DATA_WIDTH-1:0] gath[2][MAX_CHANNELS];

  logic [CFG_W-1:0] cfg, cfg_q;
  logic [RPT_W-1:0] rpt, rpt_q;
  wire [CFG_W-1:0] cbm = (cfg_q + CH_PER_BEAT - 1) >> CPB_LOG;

  wire awen=s_axil_awvalid&&s_axil_wvalid&&!s_axil_bvalid, aren=s_axil_arvalid&&!s_axil_rvalid;
  assign s_axil_awready = awen;
  assign s_axil_wready  = awen;
  assign s_axil_bresp   = 0;
  assign s_axil_arready = aren;
  assign s_axil_rresp   = 0;

  typedef enum {
    IDLE,
    REPLAYING
  } st_t;
  st_t st, sn;
  logic sel;
  logic [CFG_W-1:0] ccnt;
  logic [RPT_W-1:0] rcnt;
  wire lr = rcnt == 0;
  wire ol = lr && ccnt == cfg_q - 1;
  wire il;
  logic pend, pht;
  logic tseen, oht;

  always_comb begin
    unique case (st)
      IDLE: sn = il ? REPLAYING : IDLE;
      REPLAYING: begin
        if (ol && !il && oac) sn = pend ? REPLAYING : IDLE;
        else sn = REPLAYING;
      end
      default: sn = IDLE;
    endcase
  end
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) st <= IDLE;
    else if (bp) st <= IDLE;
    else st <= sn;
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cfg <= MAX_CHANNELS;
      cfg_q <= MAX_CHANNELS;
      rpt <= 1;
      rpt_q <= 1;
      ccnt <= 0;
      rcnt <= rpt - 1;
      bp <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata <= 0;
    end else begin
      if (awen) begin
        case (s_axil_awaddr)
          REG_CH:
          cfg<=(s_axil_wdata[CFG_W-1:0]==0||s_axil_wdata[CFG_W-1:0]>MAX_CHANNELS)?MAX_CHANNELS:s_axil_wdata[CFG_W-1:0];
          REG_RPT:
          rpt<=(s_axil_wdata[RPT_W-1:0]==0)?1:(s_axil_wdata[RPT_W-1:0]>MAX_REPLAY_CNT?MAX_REPLAY_CNT:s_axil_wdata[RPT_W-1:0]);
          REG_BP: bp <= s_axil_wdata[0];
          default: ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) s_axil_bvalid <= 0;
      if (st == IDLE) begin
        cfg_q <= cfg;
        rpt_q <= rpt;
      end
      if (aren) begin
        case (s_axil_araddr)
          REG_CH:  s_axil_rdata <= {{32 - CFG_W{1'b0}}, cfg};
          REG_RPT: s_axil_rdata <= {{32 - RPT_W{1'b0}}, rpt};
          REG_BP:  s_axil_rdata <= {31'h0, bp};
          default: s_axil_rdata <= 0;
        endcase
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) s_axil_rvalid <= 0;
      if (bp) begin
        ccnt <= 0;
        rcnt <= rpt_q - 1;
      end else if (st == REPLAYING && oac) begin
        if (ccnt == cfg_q - 1) begin
          ccnt <= 0;
          if (rcnt == 0) rcnt <= rpt_q - 1;
          else rcnt <= rcnt - 1;
        end else ccnt <= ccnt + 1;
      end else if (st == IDLE && sn == REPLAYING) begin
        ccnt <= 0;
        rcnt <= rpt_q - 1;
      end
    end
  end

  // Output
  wire [OUT_COL*DATA_WIDTH-1:0] orow;
  generate
    for (genvar i = 0; i < OUT_COL; i++) begin : go
      wire [GIW-1:0] oa = i * DATA_WIDTH;
      assign orow[i*DATA_WIDTH+:DATA_WIDTH] = gath[sel][ccnt][oa+:DATA_WIDTH];
    end
  endgenerate

  wire [OUT_COL*DATA_WIDTH-1:0] tdp = orow;
  wire tvp = st == REPLAYING;
  wire tlp = ol && oht;
  wire oac = !bp && (!mv || m_axis_tready);
  logic [OUT_COL*DATA_WIDTH-1:0] mr;
  logic mv, ml;
  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mr <= 0;
      mv <= 0;
      ml <= 0;
    end else if (oac) begin
      mr <= tvp ? tdp : 0;
      mv <= tvp;
      ml <= tlp;
    end
  end
  assign m_axis_tdata  = bp ? s_axis_tdata : mr;
  assign m_axis_tvalid = bp ? s_axis_tvalid : mv;
  assign m_axis_tlast  = bp ? s_axis_tlast : ml;

  // Input capture
  wire hf = (st == IDLE || !pend) && !tseen;
  assign s_axis_tready = bp ? m_axis_tready : hf;
  wire ad = !bp && s_axis_tvalid && s_axis_tready;
  logic [OC_BITS-1:0] cc;
  logic [CB_BITS-1:0] rc;
  wire isel = !sel;
  assign il = ad && rc == CB_BITS'(cbm - 1) && cc == OC_BITS'(OUT_COL - 1);

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cc <= 0;
      rc <= 0;
      pend <= 0;
      pht <= 0;
      tseen <= 0;
      oht <= 0;
      sel <= 0;
    end else begin
      if (pend && ol) begin
        pend <= 0;
        pht  <= 0;
        oht  <= pht;
        sel  <= !sel;
      end
      if (!bp && (ad || tseen)) begin
        if (tseen) bufr[(isel*OUT_COL+cc)*CB+rc] <= 0;
        else bufr[(isel*OUT_COL+cc)*CB+rc] <= s_axis_tdata;
        if (!tseen) begin
          for (int c = 0; c < CH_PER_BEAT; c++) begin
            automatic int ci = rc * CH_PER_BEAT + c;
            if (ci < MAX_CHANNELS)
              gath[isel][ci][GIW'(cc*DATA_WIDTH)+:DATA_WIDTH] <= s_axis_tdata[c*DATA_WIDTH+:DATA_WIDTH];
          end
        end
        if (s_axis_tlast && !tseen && !il) tseen <= 1;
        if (rc == CB_BITS'(cbm - 1)) begin
          rc <= 0;
          if (cc == OC_BITS'(OUT_COL - 1)) begin
            cc <= 0;
            if (st == REPLAYING) begin
              pend <= 1;
              pht  <= s_axis_tlast;
            end else begin
              tseen <= 0;
              oht   <= s_axis_tlast;
              sel   <= !sel;
            end
          end else cc <= cc + 1;
        end else rc <= rc + 1;
      end
    end
  end
endmodule
