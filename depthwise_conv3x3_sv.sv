`timescale 1ns / 1ps

// 3×3 depthwise convolution engine, NHWC pixel stream.
//  - 3 line buffers (BRAM) for row buffering
//  - 3×3 window extracted via shift registers
//  - Per-channel MAC over the 9-window tap
//  - Zero-padding at image edges

module depthwise_conv3x3_sv #(
    parameter DATA_WIDTH = 8,
    parameter CHANNELS   = 8,
    parameter IMG_W      = 32,
    parameter IMG_H      = 32
) (
    input  logic                              clk,
    rst_n,

    // AXI4-Stream: input (NHWC raster)
    input  logic [CHANNELS*DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                              s_axis_tvalid,
    output logic                              s_axis_tready,
    input  logic                              s_axis_tlast,

    // AXI4-Stream: output
    output logic [CHANNELS*DATA_WIDTH-1:0] m_axis_tdata,
    output logic                              m_axis_tvalid,
    input  logic                              m_axis_tready,
    output logic                              m_axis_tlast,

    // AXI4-Lite: weights + config
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

  // ─── Parameters & constants ──────────────────────────────────────────
  localparam unsigned PIX_W = CHANNELS * DATA_WIDTH;  // bits per pixel
  localparam unsigned ADDR_W = (IMG_W > 1) ? $clog2(IMG_W) : 1;
  localparam unsigned WIN_TAPS = 9;  // 3×3
  // Number of AXI-Lite words for weights (9 weights per channel)
  localparam unsigned WEIGHT_WORDS = 9 * CHANNELS;
  // Weight register-file width (address = 4 * weight_word)
  localparam unsigned WEIGHT_ADDR_W = $clog2(WEIGHT_WORDS * 4 + 1);
  localparam unsigned REG_DW = 8'h00;      // image width (RO)
  localparam unsigned REG_DH = 8'h04;      // image height (RO)
  localparam unsigned REG_WEIGHT_BASE = 8'h10;  // weights start here

  // ─── Weight register file ────────────────────────────────────────────
  // Packed: 9 weights per channel, each DATA_WIDTH bits wide.
  // Storage: 9 * CHANNELS * DATA_WIDTH bits (e.g. 9*8*8 = 576 bits).
  logic [DATA_WIDTH-1:0] weights[9 * CHANNELS];

  // Weight index from AXI-Lite write address:
  //   addr = REG_WEIGHT_BASE + 4 * idx  →  idx = (addr - REG_WEIGHT_BASE) / 4
  // We store only one weight per 32-bit word (lower DATA_WIDTH bits).
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
    end else begin
      if (axil_wr_en) begin
        if (s_axil_awaddr >= REG_WEIGHT_BASE) begin
          automatic int w_idx = int'(s_axil_awaddr - REG_WEIGHT_BASE) >> 2;
          if (w_idx < 9 * CHANNELS)
            weights[w_idx] <= s_axil_wdata[DATA_WIDTH-1:0];
        end
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (axil_rd_en) begin
        if (s_axil_araddr == REG_DW)
          s_axil_rdata <= IMG_W;
        else if (s_axil_araddr == REG_DH)
          s_axil_rdata <= IMG_H;
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

  // ─── Line buffers ────────────────────────────────────────────────────
  // 3 rows × IMG_W entries, each entry = one pixel (all channels).
  // BRAM-inferred; single write port + single read port per buffer.
  (* ram_style = "block" *) logic [PIX_W-1:0] line_buf[3][IMG_W];

  logic [ADDR_W-1:0] col_cnt;        // current column (0..IMG_W-1)
  logic [$clog2(IMG_H)-1:0] row_cnt; // current row (0..IMG_H-1)
  logic [1:0] row_sel;               // which line buffer is the current write row
  logic s_newline;                   // first pixel of a new line
  logic input_valid;                 // qualified handshake

  assign input_valid = s_axis_tvalid && s_axis_tready;

  // Row/col counters
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      col_cnt  <= 0;
      row_cnt  <= 0;
      row_sel  <= 0;
    end else if (input_valid) begin
      if (col_cnt == ADDR_W'(IMG_W - 1) || s_axis_tlast) begin
        col_cnt <= 0;
        row_cnt <= s_axis_tlast ? 0 : row_cnt + 1;
        row_sel <= s_axis_tlast ? 0 : row_sel + 1;
      end else begin
        col_cnt <= col_cnt + 1;
      end
    end
  end

  assign s_newline = input_valid && (col_cnt == ADDR_W'(0));

  // Write current input pixel to the current line buffer
  always @(posedge clk) begin
    if (input_valid)
      line_buf[row_sel][col_cnt] <= s_axis_tdata;
  end

  // Read from the 2 previous rows (for window extraction).
  // row_sel = row currently being written (row N).
  // Previous rows: row_sel - 1 (N-1), row_sel - 2 (N-2).
  // Read at col_cnt (same column as write), output registered.
  wire [1:0] prev_sel = row_sel - 1;
  wire [1:0] prev2_sel = row_sel - 2;
  logic [PIX_W-1:0] line_n1_out, line_n2_out;

  always @(posedge clk) begin
    line_n1_out <= line_buf[prev_sel][col_cnt];
    line_n2_out <= line_buf[prev2_sel][col_cnt];
  end

  // ─── 3×3 window shift registers ──────────────────────────────────────
  //   tap_r0: row N-2, 3 columns  (from line_n2_out)
  //   tap_r1: row N-1, 3 columns  (from line_n1_out)
  //   tap_r2: row N,   3 columns  (from current input, s_axis_tdata)
  // Shift left each cycle: new pixel enters at position 0, oldest drops.
  logic [PIX_W-1:0] tap_r0[3];  // [2]=col-1, [1]=col, [0]=col+1
  logic [PIX_W-1:0] tap_r1[3];
  logic [PIX_W-1:0] tap_r2[3];

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      tap_r0[0] <= 0; tap_r0[1] <= 0; tap_r0[2] <= 0;
      tap_r1[0] <= 0; tap_r1[1] <= 0; tap_r1[2] <= 0;
      tap_r2[0] <= 0; tap_r2[1] <= 0; tap_r2[2] <= 0;
    end else if (input_valid) begin
      // Shift: new pixel at LSB, oldest drops (from MSB)
      tap_r0[0] <= tap_r0[1];
      tap_r0[1] <= tap_r0[2];
      tap_r0[2] <= line_n2_out;

      tap_r1[0] <= tap_r1[1];
      tap_r1[1] <= tap_r1[2];
      tap_r1[2] <= line_n1_out;

      tap_r2[0] <= tap_r2[1];
      tap_r2[1] <= tap_r2[2];
      tap_r2[2] <= s_axis_tdata;
    end else if (col_cnt == 0 && row_cnt == 0) begin
      // Reset on new frame (s_axis_tlast wraps row_cnt to 0)
      tap_r0[0] <= 0; tap_r0[1] <= 0; tap_r0[2] <= 0;
      tap_r1[0] <= 0; tap_r1[1] <= 0; tap_r1[2] <= 0;
      tap_r2[0] <= 0; tap_r2[1] <= 0; tap_r2[2] <= 0;
    end
  end

  // ─── Per-channel MAC ─────────────────────────────────────────────────
  //   For each channel c:
  //     sum = 0
  //     for i = 0..2, j = 0..2:
  //       tap_idx = i * 3 + j
  //       sum += tap_{r_i}[2-j][c] * weights[tap_idx * CHANNELS + c]
  //
  // We multiply 9 taps per channel; 9 cycles of pipelined MAC.
  // Pipelined: 1-cycle MUL, 3-cycle adder tree (9→5→3→1).
  // Simplified: use DSP48 for signed multiply-accumulate.

  // Weights per tap: weights[tap * CHANNELS + c]  (tap 0..8, c 0..CH-1)
  // Tap order: (r=0,c=0), (r=0,c=1), (r=0,c=2),
  //            (r=1,c=0), (r=1,c=1), (r=1,c=2),
  //            (r=2,c=0), (r=2,c=1), (r=2,c=2)

  // First compute weighted products for all 9 taps in parallel
  logic signed [DATA_WIDTH-1:0] tap_s[9][CHANNELS];  // signed pixel values
  logic signed [DATA_WIDTH-1:0] wgt_s[9][CHANNELS];  // signed weight values
  logic signed [2*DATA_WIDTH-1:0] prod[9][CHANNELS]; // product

  always_comb begin
    for (int t = 0; t < 9; t++) begin
      for (int c = 0; c < CHANNELS; c++) begin
        // Extract channel c from the tap pixel
        automatic int r = t / 3;
        automatic int p = 2 - (t % 3);  // column position in shift reg
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

  // Adder tree: 9 products → 1 sum per channel
  // Stage 1: 4+4+1 = 9 → 5
  // Stage 2: 4+1    = 5 → 3
  // Stage 3: 2+1    = 3 → 2
  // Stage 4: 1+1    = 2 → 1
  localparam ACC_W = 2*DATA_WIDTH + $clog2(WIN_TAPS);
  logic signed [ACC_W-1:0] sum_stage1[5][CHANNELS];
  logic signed [ACC_W-1:0] sum_stage2[3][CHANNELS];
  logic signed [ACC_W-1:0] sum_stage3[2][CHANNELS];
  logic signed [ACC_W-1:0] sum_final[CHANNELS];
  logic signed [ACC_W-1:0] result_int[CHANNELS];  // result before clamp
  logic        [DATA_WIDTH-1:0] result[CHANNELS];

  genvar gi, gc;
  generate
    for (gc = 0; gc < CHANNELS; gc++) begin : gen_mac
      // Pipeline stage 1: 4+4+1
      always @(posedge clk) begin
        sum_stage1[0][gc] <= $signed(prod[0][gc]) + $signed(prod[1][gc]);
        sum_stage1[1][gc] <= $signed(prod[2][gc]) + $signed(prod[3][gc]);
        sum_stage1[2][gc] <= $signed(prod[4][gc]) + $signed(prod[5][gc]);
        sum_stage1[3][gc] <= $signed(prod[6][gc]) + $signed(prod[7][gc]);
        sum_stage1[4][gc] <= $signed(prod[8][gc]);
      end
      // Stage 2: 5→3
      always @(posedge clk) begin
        sum_stage2[0][gc] <= $signed(sum_stage1[0][gc]) + $signed(sum_stage1[1][gc]);
        sum_stage2[1][gc] <= $signed(sum_stage1[2][gc]) + $signed(sum_stage1[3][gc]);
        sum_stage2[2][gc] <= $signed(sum_stage1[4][gc]);
      end
      // Stage 3: 3→2
      always @(posedge clk) begin
        sum_stage3[0][gc] <= $signed(sum_stage2[0][gc]) + $signed(sum_stage2[1][gc]);
        sum_stage3[1][gc] <= $signed(sum_stage2[2][gc]);
      end
      // Stage 4: 2→1
      always @(posedge clk) begin
        sum_final[gc] <= $signed(sum_stage3[0][gc]) + $signed(sum_stage3[1][gc]);
      end
      // Clamp to DATA_WIDTH
      always @(posedge clk) begin
        result_int[gc] <= sum_final[gc];
        if (sum_final[gc] > ACC_W'(2**(DATA_WIDTH-1) - 1))
          result[gc] <= {1'b0, {DATA_WIDTH-1{1'b1}}};  // sat max
        else if (sum_final[gc] < ACC_W'(-2**(DATA_WIDTH-1)))
          result[gc] <= {1'b1, {DATA_WIDTH-1{1'b0}}};  // sat min
        else
          result[gc] <= DATA_WIDTH'(sum_final[gc]);
      end
    end
  endgenerate

  // ─── Output pipeline ─────────────────────────────────────────────────
  //   The MAC pipeline is 5 stages deep.  After the first valid pixel,
  //   the first valid output appears 5 cycles later.  We track valid
  //   through the pipeline with a shift register.
  localparam PIPELINE_DEPTH = 4;  // #pipe stages in MAC
  logic [PIPELINE_DEPTH:0] valid_pipe;  // +1 for the output reg

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n)
      valid_pipe <= 0;
    else begin
      valid_pipe[0] <= input_valid && row_cnt >= 1 && col_cnt >= 1;
      for (int i = 0; i < PIPELINE_DEPTH; i++)
        valid_pipe[i+1] <= valid_pipe[i];
    end
  end

  // Assemble output pixel
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata  <= 0;
      m_axis_tvalid <= 0;
      m_axis_tlast  <= 0;
    end else begin
      m_axis_tvalid <= valid_pipe[PIPELINE_DEPTH] && (!m_axis_tvalid || m_axis_tready);
      if (!m_axis_tvalid || m_axis_tready) begin
        for (int c = 0; c < CHANNELS; c++)
          m_axis_tdata[c*DATA_WIDTH+:DATA_WIDTH] <= result[c];
        m_axis_tlast <= valid_pipe[PIPELINE_DEPTH] && (row_cnt == IMG_H - 1) && (col_cnt == IMG_W - 1);
      end
    end
  end

  // Back-pressure: stall input when output pipeline is full
  assign s_axis_tready = rst_n && (!m_axis_tvalid || m_axis_tready);

endmodule
