`timescale 1ns / 1ps
`include "defs.svh"

module sa_wrapper_axi_ctrl_sv #(
    parameter  unsigned SIZE           = 4,
    parameter  unsigned A_DEPTH        = 4,
    parameter  unsigned C_DEPTH        = 4,
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

  state_t state, state_nxt;

  localparam ROW_BITS = SIZE > 1 ? $clog2(SIZE) : 1;
  localparam unsigned A_RING_DEPTH = A_DEPTH;
  localparam unsigned A_RING_ADDR_W = $clog2(A_RING_DEPTH);
  localparam unsigned C_RING_ADDR_W = $clog2(C_DEPTH);

  logic last_row;
  reg [SIZE-1:0] current_row, output_idx_oh;
  logic output_going_on;
  logic delayed_b_consume;

  wire  operate = s_axis_B_tvalid && can_output;
  wire  b_consume = operate && (state == LOAD_B);
  wire  a_consume = operate && (state == LOAD_A);
  wire  c_consume = operate && (state == LOAD_C);

  wire  can_output = !m_axis_tvalid || m_axis_tready;
  wire  output_stalled = output_going_on && !m_axis_tready;
  wire  advance = b_consume | (output_going_on && m_axis_tready);

  assign s_axis_B_tready = can_output && state != IDLE && !output_stalled;

  logic [DATA_WIDTH_IN-1:0] a_ring_internal[SIZE][SIZE];
  logic [DATA_WIDTH_IN-1:0] a_row[SIZE];
  logic [DATA_WIDTH_IN-1:0] b_row[SIZE][SIZE];
  logic [DATA_WIDTH_OUT-1:0] c_row[SIZE];
  logic [DATA_WIDTH_OUT-1:0] result_row[SIZE];

  logic [(DATA_WIDTH_IN * SIZE)-1:0] a_ring[A_RING_DEPTH];
  logic [A_RING_ADDR_W-1:0] a_rd_ptr;

  logic [DATA_WIDTH_OUT-1:0] c_ring[C_DEPTH];
  logic [C_RING_ADDR_W-1:0] c_rd_ptr;

  // AXI-Lite registers (6-bit addresses, 0x00..0x3F)
  // 0x00  CTRL:         [0]=state (RO)
  // 0x04  STATUS:       [0]=b_underflow, [1]=s_axis_B_tvalid, [2]=can_output (RO, clear-on-read for bit0)
  // 0x08  C_LOAD:       write to trigger C_LOAD
  // 0x0C  FB_CNT:       group accumulation counter
  // 0x10  A_LOAD:       write to trigger A_LOAD
  // 0x14  ACC_OUT:      [0]=enable output during accumulation
  // 0x18  A_LOOP_START: first ring index for A
  // 0x1C  A_LOOP_END:   last ring index for A
  // 0x20  C_LOOP_START: first ring index for C
  // 0x24  C_LOOP_END:   last ring index for C
  // 0x2C  RST_INDEX:    write to reset ring pointers and pending flags

  reg acc_output_en;
  reg b_underflow;
  reg [7:0] acc_cnt;
  reg a_load_pending;
  reg c_load_pending;
  reg soft_rst;
  reg [A_RING_ADDR_W-1:0] a_loop_start;
  reg [A_RING_ADDR_W-1:0] a_loop_end;
  reg [C_RING_ADDR_W-1:0] c_loop_start;
  reg [C_RING_ADDR_W-1:0] c_loop_end;

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
        if (a_rd_ptr == a_loop_end || (state == LOAD_A && s_axis_B_tlast)) begin
          state_nxt = IDLE;
        end else begin
          state_nxt = LOAD_A;
        end
      end
      LOAD_C: begin
        if (c_rd_ptr == c_loop_end || (state == LOAD_C && s_axis_B_tlast)) begin
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
    end else if (soft_rst) begin
      state <= IDLE;
    end else begin
      state <= state_nxt;
    end
  end


  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      a_loop_start  <= 0;
      a_loop_end    <= SIZE - 1;
      c_loop_start  <= 0;
      c_loop_end    <= C_DEPTH - 1;
      acc_output_en <= 0;
      acc_cnt       <= 0;
      b_underflow   <= 0;
      soft_rst      <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
    end else begin

      if (axil_wr_en) begin
        case (s_axil_awaddr)
          6'h0C:   acc_cnt <= s_axil_wdata[7:0];
          6'h08:   c_load_pending <= 1;
          6'h10:   a_load_pending <= 1;
          6'h14:   acc_output_en <= s_axil_wdata[0];
          6'h18:   a_loop_start <= s_axil_wdata[A_RING_ADDR_W-1:0];
          6'h1C:   a_loop_end <= s_axil_wdata[A_RING_ADDR_W-1:0];
          6'h20:   c_loop_start <= s_axil_wdata[C_RING_ADDR_W-1:0];
          6'h24:   c_loop_end <= s_axil_wdata[C_RING_ADDR_W-1:0];
          6'h2C:   ;  // RST_INDEX handled below
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
          6'h00:   s_axil_rdata <= state;
          6'h04: begin
            s_axil_rdata <= {29'h0, can_output, s_axis_B_tvalid, b_underflow};
            b_underflow  <= 0;
          end
          6'h0C:   s_axil_rdata <= {24'h0, acc_cnt};
          6'h10:   s_axil_rdata <= {31'h0, a_load_pending};
          6'h14:   s_axil_rdata <= {31'h0, acc_output_en};
          6'h18:   s_axil_rdata <= {{32 - A_RING_ADDR_W{1'h0}}, a_loop_start};
          6'h1C:   s_axil_rdata <= {{32 - A_RING_ADDR_W{1'h0}}, a_loop_end};
          6'h20:   s_axil_rdata <= {{32 - C_RING_ADDR_W{1'h0}}, c_loop_start};
          6'h24:   s_axil_rdata <= {{32 - C_RING_ADDR_W{1'h0}}, c_loop_end};
          default: s_axil_rdata <= 32'h0;
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
      if (axil_wr_en && (s_axil_awaddr == 6'h2C)) begin
        soft_rst <= 1;
      end else begin
        soft_rst <= 0;
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
            b_row[i][j] <= s_axis_B_tdata[j*DATA_WIDTH_IN+:DATA_WIDTH_IN];
          end
        end
      end

      if (state_nxt == IDLE) begin
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
        c_ring[c_rd_ptr] <= s_axis_B_tdata[DATA_WIDTH_OUT-1:0];
        if (c_rd_ptr == c_loop_end) c_rd_ptr <= c_loop_start;
        else c_rd_ptr <= c_rd_ptr + 1;
      end else if (stop_feedback && b_consume) begin
        if (c_rd_ptr == c_loop_end) c_rd_ptr <= c_loop_start;
        else c_rd_ptr <= c_rd_ptr + 1;
      end
    end
  end


  logic stop_feedback;
  counter #(
      .DYN(1),
      .MAX(256)
  ) c_cntr (
      .clk(clk),
      .en(last_row && b_consume),
      .rst_n(rst_n),
      .dyn_max(acc_cnt),
      .zero(stop_feedback),
      .ending(),
      .count()
  );

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      c_row <= '{default: '0};
    end else if (soft_rst) begin
      c_row <= '{default: '0};
    end else if (state == LOAD_A || state == LOAD_C) begin
      if (acc_cnt == 0) c_row <= '{default: '0};
    end else begin
      if (stop_feedback) begin
        c_row <= '{default: c_ring[c_rd_ptr]};
      end else begin
        c_row <= result_row;
      end
    end
  end

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
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) sa_core (
      .clk       (clk),
      .rst_n     (rst_n),
      .valid     (delayed_b_consume),
      .a_row     (a_row),
      .b_row     (b_row),
      .c_row     (c_row),
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
      output_valid <= output_going_on && (stop_feedback || acc_output_en);
      output_last  <= output_is_last && output_idx_oh[SIZE-1];
    end
  end

  assign m_axis_tvalid = output_valid;
  assign m_axis_tdata  = {<<DATA_WIDTH_OUT{result_row}};
  assign m_axis_tlast  = output_last && output_valid;

endmodule
