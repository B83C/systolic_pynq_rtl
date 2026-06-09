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
  // 0x04  MODE:  [1:0]=a_mode, [3:2]=b_mode  (0=normal, 1=reuse)
  // 0x08  LOOP:  [7:0]=loop_len_a (1..3*SIZE, default SIZE)

  reg        ctrl_start;
  reg        ctrl_done;
  reg        mode_reuse_a, mode_reuse_b;
  reg [7:0]  loop_len_a_q;

  wire       axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
  wire       axil_rd_en = s_axil_arvalid && !s_axil_rvalid;
  wire       start_wr   = axil_wr_en && (s_axil_awaddr == 4'h0) && s_axil_wdata[0];

  assign s_axil_awready = axil_wr_en;
  assign s_axil_wready  = axil_wr_en;
  assign s_axil_bresp   = 2'b00;
  assign s_axil_arready = axil_rd_en;
  assign s_axil_rresp   = 2'b00;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      ctrl_start    <= 0;
      mode_reuse_a  <= 0;
      mode_reuse_b  <= 0;
      loop_len_a_q  <= SIZE;
      s_axil_bvalid <= 0;
      s_axil_rvalid <= 0;
      s_axil_rdata  <= 0;
    end else begin
      // ---- write ----
      if (axil_wr_en) begin
        case (s_axil_awaddr)
          4'h0: begin
            ctrl_start <= s_axil_wdata[0];
          end
          4'h4: begin
            mode_reuse_a <= s_axil_wdata[0];
            mode_reuse_b <= s_axil_wdata[2];
          end
          4'h8: begin
            if (s_axil_wdata[7:0] >= 1 && s_axil_wdata[7:0] <= MAX_LOOP)
              loop_len_a_q <= s_axil_wdata[7:0];
          end
          default: ;
        endcase
        s_axil_bvalid <= 1;
      end else if (s_axil_bready) begin
        s_axil_bvalid <= 0;
      end

      // ---- read ----
      if (axil_rd_en) begin
        case (s_axil_araddr)
          4'h0: s_axil_rdata <= {29'h0, ctrl_start, ctrl_done, state_running};
          4'h4: s_axil_rdata <= {28'h0, mode_reuse_b, 2'b00, mode_reuse_a};
          4'h8: s_axil_rdata <= {24'h0, loop_len_a_q};
          default: s_axil_rdata <= 0;
        endcase
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
    LOAD_A_B,        // loading both A and B (first phase)
    LOAD_A,          // still loading A, B done
    LOAD_B,          // still loading B, A done
    COMPUTE,         // main computation, at least one channel streaming
    DRAIN_HOLD,      // one-cycle pause to let valid=1 propagate
    DRAIN,           // flushing pipeline
    DONE_ST
  } state_t;

  state_t state, state_nxt;

  reg [7:0]          a_load_cnt;
  reg [ROW_BITS:0]   b_load_cnt;
  reg [ROW_BITS:0]   beat_cnt;
  reg [7:0]          compute_cnt;
  reg [ROW_BITS:0]   drain_cnt;
  reg                a_loading_done, b_loading_done;
  reg                a_loading_done_d, b_loading_done_d;
  reg                state_running;
  reg                tlast_seen;  // latched when any consume cycle sees tlast
  reg                first_drain;

  wire both_reuse = mode_reuse_a && mode_reuse_b;

  reg  [SIZE-1:0]    cur_row_r;

  // one‑hot current row (rotate on every valid beat except the first drain
  // cycle which is skipped for output — see output_available)
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) cur_row_r <= 1;
    else if (valid && !first_drain) cur_row_r <= {cur_row_r[SIZE-2:0], cur_row_r[SIZE-1]};
  end

  // ======================================================================
  // Handshake and SA control
  // ======================================================================

  // When a channel is in reuse, we ignore its input (treat as always valid)
  wire a_treat_valid = mode_reuse_a || s_axis_A_tvalid;
  wire b_treat_valid = mode_reuse_b || s_axis_B_tvalid;
  wire both_valid    = a_treat_valid && b_treat_valid;

  // Only assert tready for channels that are NOT in reuse
  wire a_accept = !mode_reuse_a;
  wire b_accept = !mode_reuse_b;

  wire draining  = (state == DRAIN) || (state == DRAIN_HOLD);
  wire can_output = !m_axis_tvalid || m_axis_tready;
  wire consume    = both_valid && !draining && can_output && (state != IDLE);
  wire drain_active = (state == DRAIN) && can_output;
  wire valid      = consume || drain_active;

  // SA load controls
  // During initial loading (!done): load enabled
  // After loading (done): normal mode keeps loading, reuse mode stops
  // During drain: force loads to flush with zeros
  wire a_load_en = a_loading_done ? (mode_reuse_a ? 1'b0 : 1'b1) : 1'b1;
  wire b_load_en = b_loading_done ? (mode_reuse_b ? 1'b0 : 1'b1) : 1'b1;
  wire load_a    = valid && (a_load_en || (state == DRAIN));
  wire load_b    = valid && (b_load_en || (state == DRAIN));

  // tready is high during loading (a_loading_done=0) even for reuse channels.
  // After loading completes, reuse channels drop tready.
  // Use delayed done signals so tready stays high on the cycle the last beat is accepted.
  assign s_axis_A_tready = (a_accept || !a_loading_done_d) && both_valid
                           && can_output && !draining && (state != IDLE);
  assign s_axis_B_tready = (b_accept || !b_loading_done_d) && both_valid
                           && can_output && !draining && (state != IDLE);

  // A/B data mux: use AXI input when not in drain, else zeros
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
      .loop_len_a  (loop_len_a_q),
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
        if (ctrl_start) begin
          if      ( mode_reuse_a && !mode_reuse_b) state_nxt = LOAD_B;
          else if (!mode_reuse_a &&  mode_reuse_b) state_nxt = LOAD_A;
          else                                     state_nxt = LOAD_A_B;
        end
      end

      LOAD_A_B: begin
        if (tlast_seen) begin
          state_nxt = COMPUTE;  // finish loading, then drain from COMPUTE
        end else if (!a_loading_done &&  b_loading_done) state_nxt = LOAD_A;
        else if ( a_loading_done && !b_loading_done) state_nxt = LOAD_B;
        else if ( a_loading_done &&  b_loading_done) state_nxt = COMPUTE;
      end

      LOAD_A: begin
        if (tlast_seen) state_nxt = COMPUTE;
        else if (a_loading_done) state_nxt = COMPUTE;
      end

      LOAD_B: begin
        if (tlast_seen) state_nxt = COMPUTE;
        else if (b_loading_done) state_nxt = COMPUTE;
      end

      COMPUTE: begin
        if (can_output) begin
          if (both_reuse) begin
            if (compute_cnt >= loop_len_a_q) state_nxt = DRAIN_HOLD;
          end else if (tlast_seen) begin
            state_nxt = DRAIN_HOLD;
          end
        end
      end

      DRAIN_HOLD: begin
        state_nxt = DRAIN;
      end

      DRAIN: begin
        if (drain_cnt == 1) state_nxt = DONE_ST;
      end

      DONE_ST: begin
        state_nxt = IDLE;
      end
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
      beat_cnt       <= 0;
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

      // latch tlast_seen on consume cycles; clear at drain entry
      if (state == DRAIN_HOLD || state == IDLE)
        tlast_seen <= 0;
      else if (consume && (s_axis_A_tlast || s_axis_B_tlast))
        tlast_seen <= 1;

      // a_load_cnt: counts valid cycles during A loading phase
      if (state == IDLE) begin
        a_load_cnt     <= 0;
        a_loading_done <= 0;
      end else if (valid && !a_loading_done && s_axis_A_tvalid) begin
        if (a_load_cnt == (loop_len_a_q - 1)) begin
          a_loading_done <= 1;
          a_load_cnt     <= a_load_cnt + 1;
        end else begin
          a_load_cnt <= a_load_cnt + 1;
        end
      end

      // b_load_cnt: counts valid cycles during B loading phase
      if (state == IDLE) begin
        b_load_cnt     <= 0;
        b_loading_done <= 0;
      end else if (valid && !b_loading_done && s_axis_B_tvalid) begin
        if (b_load_cnt == SIZE - 1) begin
          b_loading_done <= 1;
          b_load_cnt     <= b_load_cnt + 1;
        end else begin
          b_load_cnt <= b_load_cnt + 1;
        end
      end

      // beat_cnt counts total valid cycles in the compute phase
      if (state == IDLE || state == DONE_ST)
        beat_cnt <= 0;
      else if (consume)
        beat_cnt <= beat_cnt + 1;

      // compute_cnt counts valid cycles in COMPUTE (used for both-reuse timeout)
      if (state == IDLE || state == DONE_ST)
        compute_cnt <= 0;
      else if (state == COMPUTE && consume)
        compute_cnt <= compute_cnt + 1;

      // drain counter
      if (state == DRAIN_HOLD) begin
        drain_cnt   <= SIZE + 1;
        first_drain <= 1;
      end else if (state == DRAIN && drain_active) begin
        drain_cnt   <= drain_cnt - 1;
        first_drain <= 0;
      end

      // running / done flags
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

      // delayed loading-done signals for tready gating
      a_loading_done_d <= a_loading_done;
      b_loading_done_d <= b_loading_done;
    end
  end

  // ======================================================================
  // Output pipeline — latch result_row on the cycle AFTER computation
  // ======================================================================

  reg [AXI_OUT_WIDTH-1:0] m_axis_tdata_reg;

  // output_available when pipeline is full or during drain (skip first drain cycle)
  wire output_available = (state == DRAIN) ? !first_drain : (beat_cnt >= SIZE);

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
        for (int i = 0; i < SIZE; i++) begin
          m_axis_tdata_reg[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] <= result_row[i];
        end
        m_axis_tvalid <= 1;
        m_axis_tlast  <= (state == DRAIN) && (drain_cnt == 1);
      end else if (m_axis_tvalid && m_axis_tready) begin
        m_axis_tvalid <= 0;
        m_axis_tlast  <= 0;
      end
    end
  end

endmodule
