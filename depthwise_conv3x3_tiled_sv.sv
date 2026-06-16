`timescale 1ns / 1ps

// 3×3 depthwise conv, tiled NHWC input.
//   Each beat carries SPATIAL horizontal pixels of ONE channel.
//   Input order within a row:
//     ch0_tile0, ch0_tile1, ..., ch0_tileN-1,
//     ch1_tile0, ..., chCH-1_tileN-1
//   with tile count N = IMG_W / SPATIAL.
//   SAME padding (zp_in), output same format and dimensions.

module depthwise_conv3x3_tiled_sv #(
    parameter DATA_WIDTH = 8,
    parameter CHANNELS   = 8,
    parameter SPATIAL    = 4,           // pixels per beat (tile width)
    parameter IMG_W      = 32,
    parameter IMG_H      = 32
) (
    input  logic                               clk,
    rst_n,
    input  logic [SPATIAL*DATA_WIDTH-1:0]     s_axis_tdata,
    input  logic                               s_axis_tvalid,
    output logic                               s_axis_tready,
    input  logic                               s_axis_tlast,

    output logic [SPATIAL*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                              m_axis_tvalid,
    input  logic                              m_axis_tready,
    output logic                              m_axis_tlast,

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

  localparam unsigned TILES = IMG_W / SPATIAL;  // tiles per row per channel
  localparam unsigned TOT_BEATS_PER_ROW = CHANNELS * TILES;  // beats per image row
  localparam unsigned TOT_ROWS = IMG_H + 2;  // padded rows
  localparam unsigned TOT_OUT_ROWS = IMG_H;   // output rows
  localparam unsigned TOT_OUT_TILES = TOT_OUT_ROWS * CHANNELS * TILES;

  // AXI-Lite
  localparam REG_WEIGHT_BASE = 8'h10;
  localparam REG_ZP_IN       = 8'h00;
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
      s_axil_bvalid <= 0; s_axil_rvalid <= 0; s_axil_rdata <= 0; zp_in <= 0;
    end else begin
      if (axil_wr_en) begin
        if (s_axil_awaddr == REG_ZP_IN)
          zp_in <= s_axil_wdata[DATA_WIDTH-1:0];
        else if (s_axil_awaddr >= REG_WEIGHT_BASE) begin
          int w_idx = int'(s_axil_awaddr - REG_WEIGHT_BASE) >> 2;
          if (w_idx < 9 * CHANNELS) weights[w_idx] <= s_axil_wdata[DATA_WIDTH-1:0];
        end
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) s_axil_bvalid <= 0;
      if (axil_rd_en) begin
        if (s_axil_araddr == REG_ZP_IN)
          s_axil_rdata <= {DATA_WIDTH'(zp_in)};
        else if (s_axil_araddr >= REG_WEIGHT_BASE) begin
          int w_idx = int'(s_axil_araddr - REG_WEIGHT_BASE) >> 2;
          s_axil_rdata <= w_idx < 9*CHANNELS ? {DATA_WIDTH'(weights[w_idx])} : 0;
        end else s_axil_rdata <= 0;
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) s_axil_rvalid <= 0;
    end
  end

  // ── Input beat counters ──────────────────────────────────────────────
  logic [$clog2(TILES)-1:0]   tile_cnt;   // 0..TILES-1
  logic [$clog2(CHANNELS)-1:0] ch_cnt;    // 0..CHANNELS-1
  logic [$clog2(TOT_ROWS)-1:0] row_cnt;   // 0..IMG_H+1 (padded row index)
  logic line_done;  // one row of all channels received
  logic frame_done;
  assign s_axis_tready = rst_n;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      tile_cnt <= 0; ch_cnt <= 0; row_cnt <= 0;
    end else if (s_axis_tvalid) begin
      if (tile_cnt == TILES - 1) begin
        tile_cnt <= 0;
        if (ch_cnt == CHANNELS - 1) begin
          ch_cnt <= 0;
          row_cnt <= s_axis_tlast ? 0 : row_cnt + 1;
        end else ch_cnt <= ch_cnt + 1;
      end else tile_cnt <= tile_cnt + 1;
    end
  end

  assign line_done = s_axis_tvalid && (tile_cnt == TILES - 1) && (ch_cnt == CHANNELS - 1);
  assign frame_done = line_done && s_axis_tlast;

  // ── Tiled row buffer ─────────────────────────────────────────────────
  //   Per channel: 3 rows × TILES tiles, each tile = SPATIAL × DATA_WIDTH.
  (* ram_style = "block" *) logic [SPATIAL*DATA_WIDTH-1:0]
      row_buf[CHANNELS][3][TILES];

  // Padded input: replicate zp_in for padding tiles/rows.
  wire is_pad_row = (row_cnt == 0) || (row_cnt == IMG_H + 1);
  wire [SPATIAL*DATA_WIDTH-1:0] beat_data;
  genvar bi;
  generate
    for (bi = 0; bi < SPATIAL; bi++)
      assign beat_data[bi*DATA_WIDTH+:DATA_WIDTH] = is_pad_row ? zp_in : s_axis_tdata[bi*DATA_WIDTH+:DATA_WIDTH];
  endgenerate

  wire [$clog2(3)-1:0] wr_row = row_cnt[1:0];  // row buffer index (0..2)

  always @(posedge clk)
    if (s_axis_tvalid)
      row_buf[ch_cnt][wr_row][tile_cnt] <= beat_data;

  // ── Processing FSM ───────────────────────────────────────────────────
  //   After 3 rows of any channel, start stitching output.
  //   States: FILL → PROCESS_CH → OUTPUT_TILE → next channel.
  typedef enum {FILL, PROC, OUT_TILE} state_t;
  state_t state;

  // Per-channel processing: for each tile, run 3×3 conv over SPATIAL pixels.
  logic [$clog2(CHANNELS)-1:0] out_ch;
  logic [$clog2(TILES)-1:0]    out_tile;
  logic [$clog2(SPATIAL)-1:0]  out_pix;   // pixel within the tile
  logic [$clog2(TOT_OUT_ROWS)-1:0] out_row;

  // Registers: 3 tiles × SPATIAL pixels each
  logic [DATA_WIDTH-1:0] tap_row_buf[3][SPATIAL+2];  // +2 for padding

  // Load a tile's worth of data into the tap_row_buf for one row
  // The buffer has SPATIAL+2 entries: 1 zp_in (left pad) + SPATIAL data + 1 zp_in (right pad)
  always @(posedge clk) begin
    if (state == PROC && out_pix == 0) begin
      // Load tap_row_buf: left pad + tile data + right pad
      for (int r = 0; r < 3; r++) begin
        automatic logic [SPATIAL*DATA_WIDTH-1:0] td = row_buf[out_ch][r][out_tile];
        tap_row_buf[r][0] <= zp_in;  // left pad
        for (int p = 0; p < SPATIAL; p++)
          tap_row_buf[r][p+1] <= td[p*DATA_WIDTH+:DATA_WIDTH];
        tap_row_buf[r][SPATIAL+1] <= zp_in;  // right pad
      end
    end
  end

  // MAC pipeline (same as non-tiled version)
  logic signed [DATA_WIDTH-1:0] tap_s[9];
  logic signed [DATA_WIDTH-1:0] wgt_s[9];
  logic signed [2*DATA_WIDTH-1:0] prod[9];

  always_comb begin
    for (int t = 0; t < 9; t++) begin
      automatic int r = t / 3;
      automatic int p = (t % 3) + out_pix;  // pixel position + offset
      tap_s[t] = $signed(tap_row_buf[r][p]);
      wgt_s[t] = $signed(weights[t * CHANNELS + out_ch]);
      prod[t]  = tap_s[t] * wgt_s[t];
    end
  end

  localparam ACC_W = 2*DATA_WIDTH + 4;
  logic signed [ACC_W-1:0] s1[5], s2[3], s3[2], sum;
  logic [DATA_WIDTH-1:0] result;

  always @(posedge clk) begin
    s1[0] <= prod[0] + prod[1]; s1[1] <= prod[2] + prod[3];
    s1[2] <= prod[4] + prod[5]; s1[3] <= prod[6] + prod[7]; s1[4] <= prod[8];
    s2[0] <= s1[0] + s1[1];     s2[1] <= s1[2] + s1[3];     s2[2] <= s1[4];
    s3[0] <= s2[0] + s2[1];     s3[1] <= s2[2];
    sum   <= s3[0] + s3[1];
    if (sum > ACC_W'(2**(DATA_WIDTH-1)-1))
      result <= {1'b0, {DATA_WIDTH-1{1'b1}}};
    else if (sum < ACC_W'(-2**(DATA_WIDTH-1)))
      result <= {1'b1, {DATA_WIDTH-1{1'b0}}};
    else result <= DATA_WIDTH'(sum);
  end

  // Output buffer: accumulate SPATIAL results, emit one beat
  logic [SPATIAL*DATA_WIDTH-1:0] out_buf;

  always @(posedge clk) begin
    unique case (state)
      FILL: begin
        if (row_cnt >= 3 && s_axis_tvalid && tile_cnt == 0 && ch_cnt == 0) begin
          state <= PROC;
          out_ch <= 0; out_tile <= 0; out_pix <= 0; out_row <= 2;
        end
      end
      PROC: begin
        if (out_pix == SPATIAL - 1) begin
          // Last pixel of this tile — emit and advance
          out_buf[out_pix*DATA_WIDTH+:DATA_WIDTH] <= result;
          state <= OUT_TILE;
          m_axis_tdata <= out_buf;
          m_axis_tvalid <= 1;
        end else begin
          out_buf[out_pix*DATA_WIDTH+:DATA_WIDTH] <= result;
          out_pix <= out_pix + 1;
        end
      end
      OUT_TILE: begin
        if (m_axis_tready || !m_axis_tvalid) begin
          m_axis_tvalid <= 0;
          if (out_tile == TILES - 1) begin
            out_tile <= 0;
            if (out_ch == CHANNELS - 1) begin
              out_ch <= 0;
              out_row <= out_row + 1;
              if (out_row == IMG_H - 1) begin
                state <= FILL;  // or idle
                m_axis_tlast <= 1;
                out_tile <= TILES - 1;  // hack: force tlast on last tile... 
              end
            end else begin
              out_ch <= out_ch + 1;
              out_tile <= 0;
              out_pix <= 0;
              state <= PROC;
            end
          end else begin
            out_tile <= out_tile + 1;
            out_pix <= 0;
            state <= PROC;
          end
        end
      end
    endcase
  end

endmodule
