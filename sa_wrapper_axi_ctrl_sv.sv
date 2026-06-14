`timescale 1ns / 1ps
`include "defs.svh"

module sa_wrapper_axi_ctrl_sv #(
    parameter  unsigned A_DEPTH        = 4,
    parameter  unsigned C_DEPTH        = 4,
    parameter  unsigned ACCUM_WIDTH    = 32,
    parameter  unsigned SIZE           = 4,
    parameter  unsigned DATA_WIDTH_IN  = 8,
    parameter  unsigned DATA_WIDTH_OUT = 32,
    localparam unsigned AXI_IN_WIDTH   = SIZE * DATA_WIDTH_IN,
    localparam unsigned AXI_OUT_WIDTH  = SIZE * DATA_WIDTH_OUT,
    localparam unsigned AXI_ADDR_W     = 6
) (
    input wire clk,
    input wire rst_n,

    input  wire [AXI_IN_WIDTH-1:0] s_axis_B_tdata,
    input  wire                    s_axis_B_tvalid,
    output wire                    s_axis_B_tready,
    input  wire                    s_axis_B_tlast,

    output wire [AXI_OUT_WIDTH-1:0] m_axis_tdata,
    output wire                     m_axis_tvalid,
    input  wire                     m_axis_tready,
    output wire                     m_axis_tlast,

    input  wire                  s_axil_awvalid,
    output wire                  s_axil_awready,
    input  wire [AXI_ADDR_W-1:0] s_axil_awaddr,
    input  wire [          31:0] s_axil_wdata,
    input  wire                  s_axil_wvalid,
    output wire                  s_axil_wready,
    output wire [           1:0] s_axil_bresp,
    output reg                   s_axil_bvalid,
    input  wire                  s_axil_bready,

    input  wire                  s_axil_arvalid,
    output wire                  s_axil_arready,
    input  wire [AXI_ADDR_W-1:0] s_axil_araddr,
    output reg  [          31:0] s_axil_rdata,
    output wire [           1:0] s_axil_rresp,
    output reg                   s_axil_rvalid,
    input  wire                  s_axil_rready,

    output wire a_bypass,
    output wire idle
);

  logic new_batch;
  logic feedback_valid;
  state_t state, state_nxt;

  localparam ROW_BITS = SIZE > 1 ? $clog2(SIZE) : 1;
  localparam unsigned A_RING_DEPTH = A_DEPTH * SIZE;
  localparam unsigned A_RING_ADDR_W = $clog2(A_RING_DEPTH);
  localparam unsigned C_RING_ADDR_W = $clog2(C_DEPTH * SIZE);

  logic last_row;
  reg [SIZE-1:0] current_row, output_idx_oh;
  logic output_going_on;
  logic delayed_b_consume;

  wire  can_output = !m_axis_tvalid || m_axis_tready;

  wire  operate = s_axis_B_tvalid && can_output;
  wire  b_consume = operate && (state == LOAD_B);
  wire  a_consume = operate && (state == LOAD_A);
  wire  c_consume = operate && (state == LOAD_C);

  wire  output_stalled = output_going_on && !m_axis_tready;

  assign s_axis_B_tready = can_output && state != IDLE && !output_stalled;

  logic [DATA_WIDTH_IN-1:0] a_ring_internal[SIZE][SIZE];
  logic [DATA_WIDTH_IN-1:0] a_row[SIZE];
  logic [DATA_WIDTH_IN-1:0] b_row[SIZE][SIZE];
  logic [ACCUM_WIDTH-1:0] c_row[SIZE];
  /* verilator lint_off UNOPTFLAT */
  logic [ACCUM_WIDTH-1:0] result_row[SIZE];
  /* verilator lint_on UNOPTFLAT */

  logic [(DATA_WIDTH_IN * SIZE)-1:0] a_ring[A_RING_DEPTH];
  logic [A_RING_ADDR_W-1:0] a_rd_ptr;

  logic [ACCUM_WIDTH-1:0] c_ring[C_DEPTH * SIZE];
  logic [C_RING_ADDR_W-1:0] c_rd_ptr;


  // AXI-Lite registers (6-bit addresses, 0x00..0x3F)
  // 0x00  CTRL:         [0]=state (RO)
  // 0x04  STATUS:       [0]=b_underflow, [1]=s_axis_B_tvalid, [2]=can_output (RO, clear-on-read for bit0)
  // 0x08  C_LOAD:       write to trigger C_LOAD
  // 0x0C  FB_CNT:       group accumulation counter
  // 0x10  A_LOAD:       write to trigger A_LOAD
  // 0x14  ACC_CNT:      RO  current accumulation counter value
  // 0x18  A_LOOP_START: first ring index for A
  // 0x1C  A_LOOP_END:   last ring index for A
  // 0x20  C_LOOP_START: first ring index for C
  // 0x24  C_LOOP_END:   last ring index for C
  // 0x28  SIZE:         RO  array dimension parameter value
  // 0x2C  RST_INDEX:    write to reset ring pointers and pending flags
  // 0x30  MUL_Q:        RW  UINT16 quantized multiplier
  // 0x34  SHIFT:        RW  UINT5  right-shift amount
  // 0x38  ZP_OUT:       RW  INT8   output zero-point
  // 0x3C  ZP_IN:        RW  INT8   input zero-point (subtracted from B before PE)

  reg b_underflow;
  reg [7:0] acc_cnt;
  wire [7:0] current_acc_count;
  reg a_load_pending;
  reg c_load_pending;
  reg signed [7:0] zp_in;
  reg [15:0] mul_q;
  reg [4:0] shift;
  reg signed [7:0] zp_out;
  reg soft_rst;
  reg a_loop_active;
  reg c_loop_active;
  reg [A_RING_ADDR_W-1:0] a_loop_start;
  reg [A_RING_ADDR_W-1:0] a_loop_end;
  reg [C_RING_ADDR_W-1:0] c_loop_start;
  reg [C_RING_ADDR_W-1:0] c_loop_end;

  // We advance when new input comes in, or drain the output when master is ready and multplication is still going on and we do not need to feed back data
  wire advance = b_consume | (output_going_on && m_axis_tready && new_batch);

  assign idle = state == IDLE;

  wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid
                     && (state == IDLE || state == LOAD_B
                         || state == LOAD_A || state == LOAD_C);
  wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;

  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  always_comb begin
    unique case (state)
      IDLE: begin
        if (c_load_pending) begin
          state_nxt = LOAD_C;
        end else if (a_load_pending) begin
          state_nxt = LOAD_A;
        end else begin
          state_nxt = LOAD_B;
        end
      end
      LOAD_A: begin
        if ((a_rd_ptr == a_loop_end && a_loop_active) || (state == LOAD_A && s_axis_B_tlast)) begin
          state_nxt = IDLE;
        end else begin
          state_nxt = LOAD_A;
        end
      end
      LOAD_C: begin
        if ((c_rd_ptr == c_loop_end && c_loop_active) || (state == LOAD_C && s_axis_B_tlast)) begin
          state_nxt = IDLE;
        end else begin
          state_nxt = LOAD_C;
        end
      end

      LOAD_B: begin
        if (s_axis_B_tlast) begin
          state_nxt = IDLE;
        end else if (c_load_pending && !operate) begin
          state_nxt = LOAD_C;
        end else if (a_load_pending && !operate) begin
          state_nxt = LOAD_A;
        end else begin
          state_nxt = LOAD_B;
        end
      end
    endcase
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      state <= IDLE;
      a_loop_active <= 0;
      c_loop_active <= 0;
    end else if (soft_rst) begin
      state <= IDLE;
      a_loop_active <= 0;
      c_loop_active <= 0;
    end else begin
      state <= state_nxt;
      if (state_nxt != state) begin
        a_loop_active <= 0;
        c_loop_active <= 0;
      end else begin
        if (a_consume) a_loop_active <= 1;
        if (c_consume) c_loop_active <= 1;
      end
    end
  end


  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      a_loop_start  <= 0;
      a_loop_end    <= SIZE - 1;
      c_loop_start  <= 0;
      c_loop_end    <= (C_DEPTH * SIZE) - 1;
      acc_cnt       <= 0;
      b_underflow   <= 0;
      soft_rst      <= 0;
      zp_in         <= 0;
      mul_q         <= 0;
      shift         <= 0;
      zp_out        <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
    end else begin

      if (axil_wr_en) begin
        case (s_axil_awaddr)
          REG_FB_CNT: acc_cnt <= s_axil_wdata[7:0];
          REG_C_LOAD: c_load_pending <= 1;
          REG_A_LOAD: a_load_pending <= 1;
          REG_A_LOOP_START: a_loop_start <= s_axil_wdata[A_RING_ADDR_W-1:0];
          REG_ZP_IN:    zp_in <= s_axil_wdata[7:0];
          REG_MUL_Q:    mul_q <= s_axil_wdata[15:0];
          REG_SHIFT:    shift <= s_axil_wdata[4:0];
          REG_ZP_OUT:   zp_out <= s_axil_wdata[7:0];
          REG_A_LOOP_END: a_loop_end <= s_axil_wdata[A_RING_ADDR_W-1:0];
          REG_C_LOOP_START: c_loop_start <= s_axil_wdata[C_RING_ADDR_W-1:0];
          REG_C_LOOP_END: c_loop_end <= s_axil_wdata[C_RING_ADDR_W-1:0];
          REG_RST_INDEX: ;  // handled below
          default: ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (s_axis_B_tlast && (state == LOAD_B) && !current_row[SIZE-1]) begin
        b_underflow <= 1;
      end

      if (axil_rd_en) begin
        case (s_axil_araddr)
          REG_STATE:        s_axil_rdata <= state;
          REG_STATUS: begin
            s_axil_rdata <= {29'h0, can_output, s_axis_B_tvalid, b_underflow};
            b_underflow  <= 0;
          end
          REG_FB_CNT:       s_axil_rdata <= {24'h0, acc_cnt};
          REG_ACC_CNT:      s_axil_rdata <= {24'h0, current_acc_count};
          REG_A_LOAD:       s_axil_rdata <= {31'h0, a_load_pending};
          REG_A_LOOP_START: s_axil_rdata <= {{32 - A_RING_ADDR_W{1'h0}}, a_loop_start};
          REG_A_LOOP_END:   s_axil_rdata <= {{32 - A_RING_ADDR_W{1'h0}}, a_loop_end};
          REG_C_LOOP_START: s_axil_rdata <= {{32 - C_RING_ADDR_W{1'h0}}, c_loop_start};
          REG_C_LOOP_END:   s_axil_rdata <= {{32 - C_RING_ADDR_W{1'h0}}, c_loop_end};
          REG_SIZE:         s_axil_rdata <= SIZE;
          default:          s_axil_rdata <= 32'h0;
        endcase
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end
      if (state == LOAD_A && a_load_pending) begin
        a_load_pending <= 0;
      end
      if (state == LOAD_C && c_load_pending) begin
        c_load_pending <= 0;
      end


      // RST_INDEX: trigger soft reset pulse
      if (axil_wr_en && (s_axil_awaddr == REG_RST_INDEX)) begin
        soft_rst <= 1;
      end else begin
        soft_rst <= 0;
      end

      if (soft_rst) begin
        s_axil_bvalid <= 0;
        s_axil_rvalid <= 0;
      end
    end
  end

  logic output_is_last;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      current_row <= 1;
      b_row <= '{default: '0};
      output_is_last <= 0;
    end else if (soft_rst) begin
      current_row <= 1;
      b_row <= '{default: '0};
      output_is_last <= 0;
    end else begin
      if (b_consume) begin
        current_row <= {current_row[SIZE-2:0], current_row[SIZE-1]};
        for (int i = 0; i < SIZE; i++) begin
          if (current_row[i]) begin
            for (int j = 0; j < SIZE; j++)
            b_row[i][j] <= $signed({1'b0, s_axis_B_tdata[j*DATA_WIDTH_IN+:DATA_WIDTH_IN]})
                            - $signed(zp_in);
          end
        end
      end

      if (state_nxt == IDLE && !output_going_on) begin
        current_row <= 1;
      end
      if (state_nxt == LOAD_A || state_nxt == LOAD_C) begin
        current_row <= 1;
      end

      if (s_axis_B_tlast) begin
        output_is_last <= current_row[SIZE-1];
      end else if (output_is_last && output_idx_oh[SIZE-1]) begin
        output_is_last <= 0;
      end
    end
  end

  assign last_row = current_row[SIZE-1];

  genvar i;
  generate
    for (i = 0; i < SIZE; i++) begin : gen_a_row_diagonal_connections
      assign a_row[i] = a_ring_internal[i][i];
    end
  endgenerate

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      a_ring_internal <= '{default: '0};
    end else begin
      for (int j = 0; j < SIZE; j++) begin
        for (int k = 0; k < SIZE; k++) begin
          if (advance) begin
            a_ring_internal[j][(k+1)%SIZE] <= a_ring_internal[j][k];
          end
        end
        if (b_consume) begin
          a_ring_internal[j][0] <= a_ring[a_rd_ptr][j*DATA_WIDTH_IN+:DATA_WIDTH_IN];
        end
      end
    end
  end

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      a_rd_ptr <= a_loop_start;
    end else if (soft_rst) begin
      a_rd_ptr <= a_loop_start;
    end else begin
      if (state_nxt != state) begin
        a_rd_ptr <= a_loop_start;
      end

      if (a_consume) begin
        a_ring[a_rd_ptr] <= s_axis_B_tdata;
        if (a_rd_ptr == a_loop_end) begin
          a_rd_ptr <= a_loop_start;
        end else begin
          a_rd_ptr <= a_rd_ptr + 1;
        end
      end else if (b_consume) begin
        if (a_rd_ptr == a_loop_end) begin
          a_rd_ptr <= a_loop_start;
        end else begin
          a_rd_ptr <= a_rd_ptr + 1;
        end
      end
    end
  end

  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      c_rd_ptr <= c_loop_start;
      c_ring   <= '{default: '0};
    end else if (soft_rst) begin
      c_rd_ptr <= c_loop_start;
    end else begin
      if (state_nxt != state) begin
        c_rd_ptr <= c_loop_start;
      end
      if (c_consume) begin
        c_ring[c_rd_ptr] <= s_axis_B_tdata[ACCUM_WIDTH-1:0];
        if (c_rd_ptr == c_loop_end) c_rd_ptr <= c_loop_start;
        else c_rd_ptr <= c_rd_ptr + 1;
      end else if (new_batch && b_consume) begin
        if (c_rd_ptr == c_loop_end) c_rd_ptr <= c_loop_start;
        else c_rd_ptr <= c_rd_ptr + 1;
      end
    end
  end


  counter #(
      .DYN(1),
      .MAX(256)
  ) c_cntr (
      .clk(clk),
      .en(last_row && b_consume),
      .rst_n(rst_n && !soft_rst),
      .dyn_max(acc_cnt),
      .zero(new_batch),
      .ending(),
      .count(current_acc_count)
  );

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n || soft_rst) begin
      feedback_valid <= 0;
    end else begin
      feedback_valid <= !new_batch;
    end
  end

  wire [ACCUM_WIDTH-1:0] c_row_final[SIZE];
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      c_row <= '{default: '0};
    end else if (soft_rst) begin
      c_row <= '{default: '0};
    end else if (state == LOAD_A || state == LOAD_C) begin
      c_row <= '{default: '0};
    end else if (advance && new_batch) begin
      c_row <= '{default: c_ring[c_rd_ptr]};
    end
  end

  generate
    for (genvar i = 0; i < SIZE; i++) begin : gen_c_row_final
      assign c_row_final[i] = (state == LOAD_A || state == LOAD_C) ? 0 :
                                feedback_valid ? result_row[i] : c_row[i] ;
    end
  endgenerate

  always @(posedge clk) begin
    if (soft_rst) delayed_b_consume <= 0;
    else delayed_b_consume <= advance;
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      output_idx_oh <= 0;
    end else if (soft_rst) begin
      output_idx_oh <= 0;
    end else if (advance) begin
      output_idx_oh <= {output_idx_oh[SIZE-2:0], current_row[SIZE-1]};
    end
  end

  SA #(
      .SIZE(SIZE),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT),
      .ACCUM_WIDTH(ACCUM_WIDTH)
  ) sa_core (
      .clk       (clk),
      .rst_n     (rst_n),
      .valid     (delayed_b_consume),
      .a_row     (a_row),
      .b_row     (b_row),
      .c_row     (c_row_final),
      .result_row(result_row)
  );

  logic output_valid, output_last;
  assign output_going_on = output_idx_oh != 0;
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      output_valid <= 0;
      output_last  <= 0;
    end else if (soft_rst) begin
      output_valid <= 0;
      output_last  <= 0;
    end else begin
      output_valid <= output_going_on && new_batch;
      output_last  <= output_is_last && output_idx_oh[SIZE-1];
    end
  end

  // ---- Quantization ----
  wire signed [ACCUM_WIDTH+15:0] q_prod[SIZE];
  wire signed [ACCUM_WIDTH+15:0] q_shifted[SIZE];
  wire signed [          15:0] q_with_zp[SIZE];
  wire signed [           7:0] q_out[SIZE];
  wire [AXI_OUT_WIDTH-1:0] quant_data;
  generate
    for (genvar qi = 0; qi < SIZE; qi++) begin : gen_quant
      (* use_dsp = "yes" *)
      assign q_prod[qi]   = $signed(result_row[qi]) * $signed({16'h0, mul_q});
      assign q_shifted[qi] = $signed(q_prod[qi]) >>> shift;
      assign q_with_zp[qi] = $signed(q_shifted[qi][15:0]) + $signed(zp_out);
      assign q_out[qi]     = (q_with_zp[qi] > 127)   ? 8'sd127 :
                             (q_with_zp[qi] < -128) ? -8'sd128 :
                                                        q_with_zp[qi][7:0];
      assign quant_data[qi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT]
        = {{(DATA_WIDTH_OUT - 8){q_out[qi][7]}}, q_out[qi]};
    end
  endgenerate

  wire [AXI_OUT_WIDTH-1:0] raw_data;
  generate
    for (genvar gi = 0; gi < SIZE; gi++)
      assign raw_data[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] = result_row[gi];
  endgenerate

  assign m_axis_tvalid = output_valid;
  assign m_axis_tlast  = output_last && output_valid;
  assign m_axis_tdata  = (mul_q == 0) ? raw_data : quant_data;

endmodule
