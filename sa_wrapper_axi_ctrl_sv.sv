`timescale 1ns / 1ps

module sa_wrapper_axi_ctrl_sv #(
    parameter  unsigned SIZE           = 4,
    parameter  unsigned A_DEPTH        = 4,
    parameter  unsigned DATA_WIDTH_IN  = 8,
    parameter  unsigned DATA_WIDTH_OUT = 32,
    localparam unsigned AXI_IN_WIDTH   = SIZE * DATA_WIDTH_IN,
    localparam unsigned AXI_OUT_WIDTH  = SIZE * DATA_WIDTH_OUT,
    localparam unsigned AXI_ADDR_W     = 5
) (
    input wire clk,
    input wire rst_n,

    input  wire [AXI_IN_WIDTH-1:0] s_axis_A_tdata,
    input  wire                    s_axis_A_tvalid,
    output wire                    s_axis_A_tready,
    input  wire                    s_axis_A_tlast,

    input  wire [AXI_IN_WIDTH-1:0] s_axis_B_tdata,
    input  wire                    s_axis_B_tvalid,
    output wire                    s_axis_B_tready,
    input  wire                    s_axis_B_tlast,

    output wire [AXI_OUT_WIDTH-1:0] m_axis_tdata,
    output reg                      m_axis_tvalid,
    input  wire                     m_axis_tready,
    output reg                      m_axis_tlast,

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
    input  wire                  s_axil_rready
);

  localparam ROW_BITS = SIZE > 1 ? $clog2(SIZE) : 1;

  // AXI-Lite registers
  // 0x00  CTRL:    [0]=start (SW), [1]=done (RO), [2]=running (RO)
  // 0x04  A_DATA:  write A buffer at current a_wr_ptr (column‑major packing:
  //                byte[row] = A[row][col] for the current column pointer)
  // 0x08  A_PTR:   set/read A write pointer
  // 0x0C  FB_CNT:  group accumulation counter.  Each group accumulates
  //                (FB_CNT+1) matrices and the counter auto-reloads from
  //                FB_CNT for the next group.  Write 0 (default) to disable.
  //                When > 0 the PE sum is preserved between runs within a
  //                group and cleared at the group boundary.
  //                Sample: FB_CNT=2 → groups of 3 runs (fresh + 2 accumulate).

  reg ctrl_start;
  reg ctrl_done;
  reg [7:0] fb_reload;
  reg [7:0] acc_cnt;
  reg [7:0] a_wr_ptr;
  reg state_running;

  // Local A buffer: a_buffer[row][col] — loaded via AXIL, streamed into SA during LOAD_B
  reg [DATA_WIDTH_IN-1:0] a_buffer[SIZE][SIZE];

  wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;
  wire start_wr = axil_wr_en && (s_axil_awaddr == 5'h0) && s_axil_wdata[0];
  wire a_data_wr = axil_wr_en && (s_axil_awaddr == 5'h04);
  wire a_ptr_wr = axil_wr_en && (s_axil_awaddr == 5'h08);
  wire fb_cnt_wr = axil_wr_en && (s_axil_awaddr == 5'h0C);

  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  // Unpack 32-bit AXIL data into per-row values
  wire [DATA_WIDTH_IN-1:0] a_wr_data[SIZE];
  genvar gw;
  generate
    for (gw = 0; gw < SIZE; gw++) begin : gen_a_wr
      assign a_wr_data[gw] = s_axil_wdata[gw*DATA_WIDTH_IN+:DATA_WIDTH_IN];
    end
  endgenerate

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      ctrl_start    <= 0;
      fb_reload     <= 0;
      acc_cnt       <= 0;
      a_wr_ptr      <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
    end else begin
      if (axil_wr_en) begin
        case (s_axil_awaddr)
          5'h00:   ctrl_start <= s_axil_wdata[0];
          5'h04: begin
            for (integer i = 0; i < SIZE; i++) a_buffer[i][a_wr_ptr[ROW_BITS-1:0]] <= a_wr_data[i];
            a_wr_ptr <= a_wr_ptr + 1;
          end
          5'h08:   a_wr_ptr <= s_axil_wdata[7:0];
          5'h0C: begin
            fb_reload <= s_axil_wdata[7:0];
            acc_cnt   <= s_axil_wdata[7:0];
          end
          default: ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (axil_rd_en) begin
        case (s_axil_araddr)
          5'h00:   s_axil_rdata <= {29'h0, ctrl_start, ctrl_done, state_running};
          5'h08:   s_axil_rdata <= {24'h0, a_wr_ptr};
          5'h0C:   s_axil_rdata <= {24'h0, fb_reload};
          default: s_axil_rdata <= 32'h0;
        endcase
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end

      if (ctrl_done && !start_wr) ctrl_start <= 0;
    end
  end

  // FSM: IDLE -> LOAD_B (SIZE cycles) -> DRAIN (SIZE cycles) -> IDLE
  typedef enum logic [1:0] {
    IDLE,
    LOAD_B,
    DRAIN
  } state_t;

  state_t state, state_nxt;

  reg [ROW_BITS:0] b_load_cnt;
  reg [ROW_BITS:0] drain_cnt;
  reg              b_loading_done;
  reg              first_drain;

  reg [  SIZE-1:0] cur_row_r;

  reg              ctrl_start_prev;
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) ctrl_start_prev <= 0;
    else ctrl_start_prev <= ctrl_start;
  end
  wire ctrl_start_rise = ctrl_start && !ctrl_start_prev;

  // Handshake and SA control

  wire can_output = !m_axis_tvalid || m_axis_tready;
  wire consume = s_axis_B_tvalid && (state == LOAD_B) && can_output;
  wire drain_active = (state == DRAIN) && can_output && (drain_cnt > 0);
  wire valid = consume || drain_active;

  wire load_a = 1'b1;
  wire load_b = (state == LOAD_B);

  assign s_axis_A_tready = 1'b0;
  assign s_axis_B_tready = consume;

  logic [DATA_WIDTH_IN-1:0] a_row[SIZE];
  logic [DATA_WIDTH_IN-1:0] b_row[SIZE][SIZE];
  logic [DATA_WIDTH_OUT-1:0] c_row[SIZE];
  logic [DATA_WIDTH_OUT-1:0] result_row[SIZE];

  localparam unsigned A_RING_DEPTH = A_DEPTH;
  localparam unsigned A_RING_ADDR_W = $clog2(A_RING_DEPTH);
  logic [(DATA_WIDTH_IN * SIZE)-1:0] a_ring[A_RING_DEPTH];
  logic [A_RING_ADDR_W-1:0] a_rd_ptr;

  logic [SIZE - 1:0] current_row;
  logic operate;
  logic last_row = current_row[SIZE-1];

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      current_row <= 1;
    end else if (operate) begin
      current_row <= {current_row[SIZE-2:0], current_row[SIZE-1]};
      foreach (b_row[i]) begin
        if (current_row[i]) begin
          b_row[i] <= {<<{s_axis_B_tdata}};
        end
      end
    end
  end

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      a_row <= '{default: '0};
      a_rd_ptr <= 0;
    end else if (operate) begin
      a_row <= {<<{a_ring[a_rd_ptr]}};
      if (a_rd_ptr == A_RING_ADDR_W'(A_DEPTH - 1)) begin
        a_rd_ptr <= 0;
      end else begin
        a_rd_ptr <= a_rd_ptr + 1;
      end
    end
  end

  logic stop_feedback;
  counter #(
      .DYN(1),
      .COUNTDOWN(1)
  ) c_cntr (
      .clk(clk),
      .en(last_row && operate),
      .rst_n(rst_n),
      .ending(stop_feedback),
      .count()
  );
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      c_row <= '{default: '0};
    end else if (operate) begin
      if (stop_feedback) begin
        c_row <= '{default: '0};
      end else begin
        c_row <= result_row;
      end
    end
  end

  SA #(
      .SIZE(SIZE),
      .MAX_LOOP(3 * SIZE),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) sa_core (
      .clk       (clk),
      .rst_n     (rst_n),
      .valid     (valid),
      .a_row     (a_row),
      .b_row     (b_row),
      .c_row     (c_row),
      .result_row(result_row)
  );

  // FSM next-state logic

  always_comb begin
    state_nxt = state;
    case (state)
      IDLE: begin
        if (ctrl_start_rise) state_nxt = LOAD_B;
      end

      LOAD_B: begin
        if (b_loading_done) state_nxt = DRAIN;
      end

      DRAIN: begin
        if (drain_cnt == 0) state_nxt = IDLE;
      end
      default: state_nxt = IDLE;
    endcase
  end

  // FSM sequential + counters

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      state          <= IDLE;
      b_load_cnt     <= 0;
      drain_cnt      <= 0;
      b_loading_done <= 0;
      state_running  <= 0;
    end else begin
      state <= state_nxt;

      if (state == IDLE) begin
        b_load_cnt     <= 0;
        b_loading_done <= 0;
      end else if (consume && !b_loading_done) begin
        if (b_load_cnt == SIZE - 1) begin
          b_loading_done <= 1;
          b_load_cnt     <= b_load_cnt + 1;
        end else begin
          b_load_cnt <= b_load_cnt + 1;
        end
      end

      if (state == IDLE) begin
        drain_cnt   <= 0;
        first_drain <= 0;
      end else if (state_nxt == DRAIN && state != DRAIN) begin
        drain_cnt   <= SIZE + 1;
        first_drain <= 1;
      end else if (drain_active) begin
        drain_cnt   <= drain_cnt - 1;
        first_drain <= 0;
      end

      if (state == IDLE) begin
        state_running <= 0;
      end else begin
        state_running <= 1;
      end

      // if (start_wr) begin
      //   ctrl_done <= 0;
      // end else if (state_nxt == IDLE && state != IDLE) begin
      //   ctrl_done <= 1;
      //   // Countdown counter: decrements every SIZE cycles (at run boundary).
      //   // cnt>0: feed previous result back into c_row for accumulation.
      //   // cnt=0: start fresh (c_row=0), reload counter.
      //   if (acc_cnt > 0) begin
      //     acc_cnt <= acc_cnt - 1;
      //     for (int i = 0; i < SIZE; i++) c_row_fb[i] <= result_row[i];
      //   end else if (fb_reload > 0) begin
      //     acc_cnt <= fb_reload;
      //     for (int i = 0; i < SIZE; i++) c_row_fb[i] <= 0;
      //   end
      // end
    end
  end

  // Output register

  reg [AXI_OUT_WIDTH-1:0] m_axis_tdata_reg;

  // generate
  //   for (gi = 0; gi < SIZE; gi++) begin : gen_pack
  //     assign m_axis_tdata[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] =
  //            m_axis_tdata_reg[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT];
  //   end
  // endgenerate

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata_reg <= 0;
      m_axis_tvalid    <= 0;
      m_axis_tlast     <= 0;
    end else begin
      if (drain_active && !first_drain) begin
        for (int i = 0; i < SIZE; i++)
        m_axis_tdata_reg[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] <= result_row[i];
        m_axis_tvalid <= 1;
        m_axis_tlast  <= (drain_cnt == 1);
      end else if (m_axis_tvalid && m_axis_tready) begin
        m_axis_tvalid <= 0;
        m_axis_tlast  <= 0;
      end
    end
  end

endmodule
