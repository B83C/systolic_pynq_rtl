`timescale 1ns / 1ps

// 3×3 depthwise convolution, SAME padding, NHWC pixel stream.
//   - Line buffer stores 3 rows of (IMG_W + 2) pixels (1-pixel zp_in pad
//     on left and right).
//   - Top/bottom padding rows are generated internally.
//   - Padding value is zp_in (from AXI-Lite), so border pixels contribute
//     zero to the accumulator after the input-zero-point subtraction that
//     the upstream SA already performs.
//   - Output spatial dimensions = IMG_H × IMG_W  (SAME padding).
//   - Per-channel 9-tap MAC with pipelined adder tree + saturation clamp.

module depthwise_conv3x3_sv #(
    parameter DATA_WIDTH = 8,
    parameter CHANNELS   = 8,
    parameter IMG_W      = 32,
    parameter IMG_H      = 32
) (
    input  logic                              clk,
    rst_n,

    // AXI4-Stream: input (NHWC raster, IMG_H × IMG_W pixels)
    input  logic [CHANNELS*DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                              s_axis_tvalid,
    output logic                              s_axis_tready,
    input  logic                              s_axis_tlast,

    // AXI4-Stream: output (SAME-padded, zero-point = zp_in)
    output logic [CHANNELS*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                              m_axis_tvalid,
    input  logic                              m_axis_tready,
    output logic                              m_axis_tlast,

    // AXI4-Lite
    input  wire        s_axil_awvalid,
    output wire        s_axil_awready,
    input  wire [ 7:0] s_axil_awaddr,
    input  wire [31:0] s_axil_wdata,
    input  wire        s_axil_wvalid,
    output wire        s_axil_wready,
    output wire [ 1:0] s_axil_bresp,
    output reg         s_axil_bvalid,
    input  wire        s_axil_bready,

    input  wire        s_axil_arvalid,
    output wire        s_axil_arready,
    input  wire [ 7:0] s_axil_araddr,
    output reg  [31:0] s_axil_rdata,
    output wire [ 1:0] s_axil_rresp,
    output reg         s_axil_rvalid,
    input  wire        s_axil_rready
);

  localparam unsigned PIX_W  = CHANNELS * DATA_WIDTH;
  localparam unsigned ADDR_W = (IMG_W + 2 > 1) ? $clog2(IMG_W + 2) : 1;
  localparam unsigned WIN_TAPS = 9;

  // AXI-Lite registers
  localparam REG_WEIGHT_BASE = 8'h10;   // weights[0..9*CH-1] at 0x10 + 4*n
  localparam REG_ZP_IN       = 8'h00;   // zero-point for padding

  logic [DATA_WIDTH-1:0] weights[9 * CHANNELS];
  logic [DATA_WIDTH-1:0] zp_in;

  wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;
  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
      zp_in         <= 0;
    end else begin
      if (axil_wr_en) begin
        if (s_axil_awaddr == REG_ZP_IN)
          zp_in <= s_axil_wdata[DATA_WIDTH-1:0];
        else if (s_axil_awaddr >= REG_WEIGHT_BASE) begin
          automatic int w_idx = int'(s_axil_awaddr - REG_WEIGHT_BASE) >> 2;
          if (w_idx < 9 * CHANNELS)
            weights[w_idx] <= s_axil_wdata[DATA_WIDTH-1:0];
        end
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (axil_rd_en) begin
        if (s_axil_araddr == REG_ZP_IN)
          s_axil_rdata <= {{32 - DATA_WIDTH{1'b0}}, zp_in};
        else if (s_axil_araddr >= REG_WEIGHT_BASE) begin
          automatic int w_idx = int'(s_axil_araddr - REG_WEIGHT_BASE) >> 2;
          s_axil_rdata <= w_idx < 9 * CHANNELS ? {{32 - DATA_WIDTH{1'b0}}, weights[w_idx]} : 0;
        end else
          s_axil_rdata <= 0;
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end
    end
  end

  // ─── Padded stream generator ────────────────────────────────────────
  //   The padded image is (IMG_H+2) × (IMG_W+2).  Top and bottom padding
  //   rows are all zp_in.  Each real row has one zp_in left pad, W real
  //   pixels, one zp_in right pad.
  localparam unsigned PW = IMG_W + 2;  // padded width
  localparam unsigned PH = IMG_H + 2;  // padded height
  localparam ADDR_W_PW = (PW > 1) ? $clog2(PW) : 1;
  localparam ADDR_W_PH = (PH > 1) ? $clog2(PH) : 1;

  // Indicates whether the current pixel in the padded stream is
  // a padding pixel (zp_in) or a real pixel from the input.
  logic is_pad;

  // S_axis_tready: accept real pixels only when NOT generating padding.
  assign s_axis_tready = rst_n && !is_pad;

  logic [ADDR_W_PH-1:0] pad_row;  // 0..IMG_H+1
  logic [ADDR_W_PW-1:0] pad_col;  // 0..IMG_W+1
  logic pad_row_last;             // last padded row
  logic pad_col_last;             // last padded column

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      pad_row <= 0;
      pad_col <= 0;
    end else begin
      // Advance every cycle (padding or real — one beat per padded pixel)
      if (pad_col_last) begin
        pad_col <= 0;
        pad_row <= pad_row_last ? 0 : pad_row + 1;
      end else begin
        pad_col <= pad_col + 1;
      end
    end
  end

  assign pad_col_last = pad_col == ADDR_W_PW'(PW - 1);
  assign pad_row_last = pad_row == ADDR_W_PH'(PH - 1);

  // A pixel is "padding" when:
  //   - row is top (0) or bottom (IMG_H+1) padding row, OR
  //   - column is left (0) or right (IMG_W+1) within a real row
  wire is_top_bottom = (pad_row == 0) || (pad_row == IMG_H + 1);
  wire is_left_right = (pad_col == 0) || (pad_col == IMG_W + 1);
  assign is_pad = is_top_bottom || is_left_right;

  // ─── Line buffers ────────────────────────────────────────────────────
  //   3 rows × PW entries, each entry = one pixel (all channels).
  (* ram_style = "block" *) logic [PIX_W-1:0] line_buf[3][PW];

  // Row-select modulo 3 — the line buffer has only 3 entries.
  reg [1:0] row_sel;  // 0,1,2,0,1,2,...

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n)
      row_sel <= 0;
    else if (pad_col_last)
      row_sel <= (row_sel == 2) ? 0 : row_sel + 1;
  end

  wire [1:0] prev_sel  = (row_sel == 0) ? 2 : row_sel - 1;
  wire [1:0] prev2_sel = (row_sel == 0) ? 1 : (row_sel == 1) ? 0 : 2;

  // Write current padded pixel to line_buf.
  // pad_data is COMBINATIONAL so the BRAM write uses the same-cycle pixel.
  logic [PIX_W-1:0] pad_data, pad_data_q;
  logic [PIX_W-1:0] line_n1_out, line_n2_out;

  always_comb begin
    if (is_pad)
      pad_data = {CHANNELS{zp_in}};
    else
      pad_data = s_axis_tdata;
  end

  // Registered copy for the tap shift register (matches BRAM read timing)
  always @(posedge clk) begin
    pad_data_q <= pad_data;
    line_buf[row_sel][pad_col] <= pad_data;
  end

  always @(posedge clk) begin
    line_n1_out <= line_buf[prev_sel][pad_col];
    line_n2_out <= line_buf[prev2_sel][pad_col];
  end

  // ─── 3×3 window shift registers ──────────────────────────────────────
  //   tap_r0: row N-2, tap_r1: row N-1, tap_r2: row N.
  //   Inside each row: [0] = left (col-1), [1] = center (col), [2] = right (col+1).
  logic [PIX_W-1:0] tap_r0[3], tap_r1[3], tap_r2[3];

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      tap_r0[0] <= 0; tap_r0[1] <= 0; tap_r0[2] <= 0;
      tap_r1[0] <= 0; tap_r1[1] <= 0; tap_r1[2] <= 0;
      tap_r2[0] <= 0; tap_r2[1] <= 0; tap_r2[2] <= 0;
    end else begin
      tap_r0[0] <= tap_r0[1];
      tap_r0[1] <= tap_r0[2];
      tap_r0[2] <= line_n2_out;

      tap_r1[0] <= tap_r1[1];
      tap_r1[1] <= tap_r1[2];
      tap_r1[2] <= line_n1_out;

      tap_r2[0] <= tap_r2[1];
      tap_r2[1] <= tap_r2[2];
      tap_r2[2] <= pad_data_q;
    end
  end

  // ─── Per-channel MAC ─────────────────────────────────────────────────
  logic signed [DATA_WIDTH-1:0] tap_s[9][CHANNELS];
  logic signed [DATA_WIDTH-1:0] wgt_s[9][CHANNELS];
  logic signed [2*DATA_WIDTH-1:0] prod[9][CHANNELS];

  always_comb begin
    for (int t = 0; t < 9; t++) begin
      for (int c = 0; c < CHANNELS; c++) begin
        automatic int r = t / 3;
        automatic int p = t % 3;  // 0=left, 1=center, 2=right
        case (r)
          0: tap_s[t][c] = $signed(tap_r0[p][c*DATA_WIDTH+:DATA_WIDTH]);
          1: tap_s[t][c] = $signed(tap_r1[p][c*DATA_WIDTH+:DATA_WIDTH]);
          2: tap_s[t][c] = $signed(tap_r2[p][c*DATA_WIDTH+:DATA_WIDTH]);
        endcase
        wgt_s[t][c] = $signed(weights[t * CHANNELS + c]);
        prod[t][c]  = tap_s[t][c] * wgt_s[t][c];
      end
    end
  end

  localparam ACC_W = 2*DATA_WIDTH + $clog2(WIN_TAPS);
  logic signed [ACC_W-1:0] sum_st1[5][CHANNELS];
  logic signed [ACC_W-1:0] sum_st2[3][CHANNELS];
  logic signed [ACC_W-1:0] sum_st3[2][CHANNELS];
  logic signed [ACC_W-1:0] sum_fl[CHANNELS];
  logic signed [ACC_W-1:0] res_int[CHANNELS];
  logic        [DATA_WIDTH-1:0] res[CHANNELS];

  genvar gc;
  generate
    for (gc = 0; gc < CHANNELS; gc++) begin : gen_mac
      always @(posedge clk) begin
        sum_st1[0][gc] <= $signed(prod[0][gc]) + $signed(prod[1][gc]);
        sum_st1[1][gc] <= $signed(prod[2][gc]) + $signed(prod[3][gc]);
        sum_st1[2][gc] <= $signed(prod[4][gc]) + $signed(prod[5][gc]);
        sum_st1[3][gc] <= $signed(prod[6][gc]) + $signed(prod[7][gc]);
        sum_st1[4][gc] <= $signed(prod[8][gc]);
      end
      always @(posedge clk) begin
        sum_st2[0][gc] <= $signed(sum_st1[0][gc]) + $signed(sum_st1[1][gc]);
        sum_st2[1][gc] <= $signed(sum_st1[2][gc]) + $signed(sum_st1[3][gc]);
        sum_st2[2][gc] <= $signed(sum_st1[4][gc]);
      end
      always @(posedge clk) begin
        sum_st3[0][gc] <= $signed(sum_st2[0][gc]) + $signed(sum_st2[1][gc]);
        sum_st3[1][gc] <= $signed(sum_st2[2][gc]);
      end
      always @(posedge clk) begin
        sum_fl[gc] <= $signed(sum_st3[0][gc]) + $signed(sum_st3[1][gc]);
      end
      always @(posedge clk) begin
        res_int[gc] <= sum_fl[gc];
        if (sum_fl[gc] > ACC_W'(2**(DATA_WIDTH-1) - 1))
          res[gc] <= {1'b0, {DATA_WIDTH-1{1'b1}}};
        else if (sum_fl[gc] < ACC_W'(-2**(DATA_WIDTH-1)))
          res[gc] <= {1'b1, {DATA_WIDTH-1{1'b0}}};
        else
          res[gc] <= DATA_WIDTH'(sum_fl[gc]);
      end
    end
  endgenerate

  // ─── Output valid tracking ───────────────────────────────────────────
  //   Cycle counter from padded-stream start.
  //   First real pixel at padded (1,1) arrives after PW+1 = W+3 cycles.
  //   Pipeline lag ≈ 6 cycles (pad_data_q + BRAM read + 3 tap shifts).
  //   So first valid window center at cycle W+3+6 = W+9.
  //   Output = IMG_H × IMG_W = TOTAL pixels.
  localparam TOTAL = IMG_H * IMG_W;
  localparam T_START = PW + 3;       // = IMG_W + 5
  logic started;
  logic [15:0] frame_cycle;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      started <= 0;
      frame_cycle <= 0;
    end else begin
      if (!started) begin
        if (s_axis_tvalid)
          started <= 1;
      end else begin
        frame_cycle <= frame_cycle + 1;
      end
    end
  end

  wire valid_win = started && (frame_cycle >= T_START) && (frame_cycle < T_START + TOTAL);

  // Count outputs for tlast
  logic [$clog2(TOTAL+1)-1:0] out_cnt;
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) out_cnt <= 0;
    else if (m_axis_tvalid && m_axis_tready) out_cnt <= out_cnt + 1;
  end
  localparam PIPELINE_DEPTH = 4;
  logic [PIPELINE_DEPTH:0] valid_pipe;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n)
      valid_pipe <= 0;
    else begin
      valid_pipe[0] <= valid_win;
      for (int i = 0; i < PIPELINE_DEPTH; i++)
        valid_pipe[i+1] <= valid_pipe[i];
    end
  end

  // Output assembly with backpressure
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata  <= 0;
      m_axis_tvalid <= 0;
      m_axis_tlast  <= 0;
    end else begin
      m_axis_tvalid <= valid_pipe[PIPELINE_DEPTH] && (!m_axis_tvalid || m_axis_tready);
      if (!m_axis_tvalid || m_axis_tready) begin
        for (int c = 0; c < CHANNELS; c++)
          m_axis_tdata[c*DATA_WIDTH+:DATA_WIDTH] <= res[c];
        m_axis_tlast <= valid_pipe[PIPELINE_DEPTH] && (out_cnt == TOTAL - 2) && m_axis_tvalid && m_axis_tready;
      end
    end
  end

  // (declared above)

endmodule
