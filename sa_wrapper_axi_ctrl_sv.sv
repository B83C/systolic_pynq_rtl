`timescale 1ns / 1ps

module sa_wrapper_axi_ctrl_sv #(
    parameter unsigned SIZE           = 4,
    parameter unsigned DATA_WIDTH_IN  = 8,
    parameter unsigned DATA_WIDTH_OUT = 32,
    localparam unsigned AXI_IN_WIDTH  = SIZE * DATA_WIDTH_IN,
    localparam unsigned AXI_OUT_WIDTH = SIZE * DATA_WIDTH_OUT,
    localparam unsigned AXI_ADDR_W    = 4
) (
    input wire clk,
    input wire rst_n,

    // AXI4-Stream: input A
    input  wire [AXI_IN_WIDTH-1:0] s_axis_A_tdata,
    input  wire                    s_axis_A_tvalid,
    output wire                    s_axis_A_tready,
    input  wire                    s_axis_A_tlast,

    // AXI4-Stream: input B
    input  wire [AXI_IN_WIDTH-1:0] s_axis_B_tdata,
    input  wire                    s_axis_B_tvalid,
    output wire                    s_axis_B_tready,
    input  wire                    s_axis_B_tlast,

    // AXI4-Stream: output
    output wire [AXI_OUT_WIDTH-1:0] m_axis_tdata,
    output reg                      m_axis_tvalid,
    input  wire                     m_axis_tready,
    output reg                      m_axis_tlast,

    // AXI4-Lite: control / status
    input  wire                    s_axil_awvalid,
    output wire                    s_axil_awready,
    input  wire [AXI_ADDR_W-1:0]   s_axil_awaddr,
    input  wire [31:0]             s_axil_wdata,
    input  wire                    s_axil_wvalid,
    output wire                    s_axil_wready,
    output wire [1:0]              s_axil_bresp,
    output reg                     s_axil_bvalid,
    input  wire                    s_axil_bready,

    input  wire                    s_axil_arvalid,
    output wire                    s_axil_arready,
    input  wire [AXI_ADDR_W-1:0]   s_axil_araddr,
    output reg  [31:0]             s_axil_rdata,
    output wire [1:0]              s_axil_rresp,
    output reg                     s_axil_rvalid,
    input  wire                    s_axil_rready
);

  localparam ROW_BITS = SIZE > 1 ? $clog2(SIZE) : 1;
  localparam MAX_LOOP = 3 * SIZE;

  // ======================================================================
  // AXI-Lite registers
  // ======================================================================
  // 0x00  CTRL:  [0]=start (SW), [1]=done (RO), [2]=running (RO)

  reg        ctrl_start;
  reg        ctrl_done;

  wire       axil_wr_en  = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire       axil_rd_en  = s_axil_arvalid && !s_axil_rvalid;
  wire       start_wr    = axil_wr_en && (s_axil_awaddr == 4'h0) && s_axil_wdata[0];

  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      ctrl_start    <= 0;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
    end else begin
      if (axil_wr_en) begin
        if (s_axil_awaddr == 4'h0)
          ctrl_start <= s_axil_wdata[0];
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      if (axil_rd_en) begin
        if (s_axil_araddr == 4'h0)
          s_axil_rdata <= {29'h0, ctrl_start, ctrl_done, state_running};
        else
          s_axil_rdata <= 0;
        s_axil_rvalid <= 1;
      end else if (s_axil_rready) begin
        s_axil_rvalid <= 0;
      end

      if (ctrl_done && !start_wr) ctrl_start <= 0;
    end
  end

  // ======================================================================
  // FSM
  // ======================================================================
  typedef enum logic [2:0] {
    IDLE,
    LOAD_A_B,
    COMPUTE,
    DRAIN_HOLD,
    DRAIN,
    DONE_ST
  } state_t;

  state_t state, state_nxt;

  reg [7:0]          a_load_cnt;
  reg [ROW_BITS:0]   b_load_cnt;
  reg [ROW_BITS:0]   drain_cnt;
  reg                a_loading_done, b_loading_done;
  reg                a_loading_done_d, b_loading_done_d;
  reg                state_running;
  reg                tlast_seen;
  reg                first_drain;

  reg  [SIZE-1:0]    cur_row_r;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) cur_row_r <= 1;
    else if (valid && !first_drain)
      cur_row_r <= {cur_row_r[SIZE-2:0], cur_row_r[SIZE-1]};
  end

  // ======================================================================
  // Handshake and SA control
  // ======================================================================

  wire both_valid    = s_axis_A_tvalid && s_axis_B_tvalid;

  wire draining  = (state == DRAIN) || (state == DRAIN_HOLD);
  wire can_output = !m_axis_tvalid || m_axis_tready;
  wire consume    = both_valid && !draining && can_output && (state != IDLE);
  wire drain_active = (state == DRAIN) && can_output;
  wire valid      = consume || drain_active;

  wire load_a    = valid;
  wire load_b    = valid && !b_loading_done;

  assign s_axis_A_tready = consume;
  assign s_axis_B_tready = consume;

  wire [DATA_WIDTH_IN-1:0] a_row[SIZE];
  wire [DATA_WIDTH_IN-1:0] b_row[SIZE];
  wire [DATA_WIDTH_OUT-1:0] c_row[SIZE];
  wire [DATA_WIDTH_OUT-1:0] result_row[SIZE];

  genvar gi;
  generate
    for (gi = 0; gi < SIZE; gi++) begin : gen_a_row
      assign a_row[gi] = state == DRAIN ? '0 :
                         s_axis_A_tdata[gi*DATA_WIDTH_IN+:DATA_WIDTH_IN];
    end
    for (gi = 0; gi < SIZE; gi++) begin : gen_b_row
      assign b_row[gi] = state == DRAIN ? '0 :
                         s_axis_B_tdata[gi*DATA_WIDTH_IN+:DATA_WIDTH_IN];
    end
  endgenerate

  assign c_row = '{default: '0};

  SA #(
      .SIZE(SIZE),
      .MAX_LOOP(MAX_LOOP),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) sa_core (
      .clk         (clk),
      .rst_n       (rst_n),
      .valid       (valid),
      .current_row (cur_row_r),
      .load_a      (load_a),
      .load_b      (load_b),
      .loop_len_a  (SIZE[7:0]),
      .a_row       (a_row),
      .b_row       (b_row),
      .c_row       (c_row),
      .result_row  (result_row)
  );

  // ======================================================================
  // FSM next-state logic
  // ======================================================================

  always_comb begin
    state_nxt = state;
    case (state)
      IDLE: begin
        if (ctrl_start) state_nxt = LOAD_A_B;
      end

      LOAD_A_B: begin
        if (tlast_seen) state_nxt = COMPUTE;
        else if (a_loading_done && b_loading_done) state_nxt = COMPUTE;
      end

      COMPUTE: begin
        if (can_output) begin
          if (tlast_seen) state_nxt = DRAIN_HOLD;
        end
      end

      DRAIN_HOLD: state_nxt = DRAIN;

      DRAIN: begin
        if (drain_cnt == 1) state_nxt = DONE_ST;
      end

      DONE_ST: state_nxt = IDLE;
    endcase
  end

  // ======================================================================
  // FSM sequential + counters
  // ======================================================================

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      state          <= IDLE;
      a_load_cnt     <= 0;
      b_load_cnt     <= 0;
      drain_cnt      <= 0;
      a_loading_done <= 0;
      b_loading_done <= 0;
      a_loading_done_d <= 0;
      b_loading_done_d <= 0;
      state_running  <= 0;
      tlast_seen     <= 0;
      first_drain    <= 0;
    end else begin
      state <= state_nxt;

      if (state == DRAIN_HOLD || state == IDLE)
        tlast_seen <= 0;
      else if (consume && (s_axis_A_tlast || s_axis_B_tlast))
        tlast_seen <= 1;

      if (state == IDLE) begin
        a_load_cnt     <= 0;
        a_loading_done <= 0;
      end else if (valid && !a_loading_done) begin
        if (a_load_cnt == SIZE - 1) begin
          a_loading_done <= 1;
          a_load_cnt     <= a_load_cnt + 1;
        end else begin
          a_load_cnt <= a_load_cnt + 1;
        end
      end

      if (state == IDLE) begin
        b_load_cnt     <= 0;
        b_loading_done <= 0;
      end else if (valid && !b_loading_done) begin
        if (b_load_cnt == SIZE - 1) begin
          b_loading_done <= 1;
          b_load_cnt     <= b_load_cnt + 1;
        end else begin
          b_load_cnt <= b_load_cnt + 1;
        end
      end

      if (state == DRAIN_HOLD) begin
        drain_cnt   <= SIZE + 1;
        first_drain <= 1;
      end else if (state == DRAIN && drain_active) begin
        drain_cnt   <= drain_cnt - 1;
        first_drain <= 0;
      end

      if (state == IDLE) begin
        state_running <= 0;
      end else if (state != DONE_ST) begin
        state_running <= 1;
      end

      if (start_wr) begin
        ctrl_done <= 0;
      end else if (state_nxt == DONE_ST) begin
        ctrl_done <= 1;
      end

      a_loading_done_d <= a_loading_done;
      b_loading_done_d <= b_loading_done;
    end
  end

  // ======================================================================
  // Output pipeline
  // ======================================================================

  reg [AXI_OUT_WIDTH-1:0] m_axis_tdata_reg;
  wire output_available = (state == DRAIN) ? !first_drain : 1'b1;

  generate
    for (gi = 0; gi < SIZE; gi++) begin : gen_pack
      assign m_axis_tdata[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] =
             m_axis_tdata_reg[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT];
    end
  endgenerate

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      m_axis_tdata_reg <= 0;
      m_axis_tvalid    <= 0;
      m_axis_tlast     <= 0;
    end else begin
      if (output_available && valid) begin
        for (int i = 0; i < SIZE; i++)
          m_axis_tdata_reg[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] <= result_row[i];
        m_axis_tvalid <= 1;
        m_axis_tlast  <= (state == DRAIN) && (drain_cnt == 1);
      end else if (m_axis_tvalid && m_axis_tready) begin
        m_axis_tvalid <= 0;
        m_axis_tlast  <= 0;
      end
    end
  end

endmodule
