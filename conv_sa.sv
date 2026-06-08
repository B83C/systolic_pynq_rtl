`timescale 1ns / 1ps

module conv_sa #(
    parameter unsigned IMG_W = 10,
    parameter unsigned IMG_H = 10,
    parameter unsigned K = 3,
    parameter unsigned DW = 8,
    parameter unsigned ACC_W = 32,
    parameter unsigned SA_SIZE = K * K
) (
    input clk,
    input rst_n,

    input [DW-1:0] s_axis_pix_tdata,
    input s_axis_pix_tvalid,
    output s_axis_pix_tready,
    input s_axis_pix_tlast,
    input s_axis_pix_tuser,

    input [DW-1:0] s_axis_kernel_tdata,
    input s_axis_kernel_tvalid,
    output s_axis_kernel_tready,

    output [ACC_W-1:0] m_axis_res_tdata,
    output m_axis_res_tvalid,
    input m_axis_res_tready,
    output m_axis_res_tlast
);

  localparam int TOTAL_WINDOWS = (IMG_W - K + 1) * (IMG_H - K + 1);
  localparam WIN_W = $clog2(TOTAL_WINDOWS + 1);

  typedef enum logic [2:0] {
    IDLE, KERNEL, BLOAD, FILL, COMPUTE, FINISH
  } state_t;
  state_t state, state_nxt;

  // ---- Kernel ----
  logic [DW-1:0] kernel_reg[K*K];
  logic [$clog2(K*K+1)-1:0] kernel_cnt;
  wire kernel_done = (int'(kernel_cnt) == int'(K * K));

  // ---- B-load counter ----
  logic [$clog2(SA_SIZE)-1:0] b_cycle;

  // ---- Line buffer ----
  logic [DW-1:0] line_buf[K][IMG_W];
  logic [$clog2(K)-1:0] wr_row;
  logic [$clog2(IMG_W)-1:0] wr_col;
  logic [$clog2(IMG_H+1):0] row_rx;
  logic in_frame;

  // ---- Window ----
  logic [DW-1:0] window_cap[K*K];
  wire window_valid_comb;
  wire new_window;

  // ---- Compute step ----
  logic [$clog2(SA_SIZE+1)-1:0] step;
  logic step_first, step_last;

  // ---- Counters ----
  logic [WIN_W-1:0] win_cnt;
  logic win_done;

  // ---- SA signals ----
  logic [DW-1:0]   sa_a_row [SA_SIZE];
  logic [DW-1:0]   sa_b_row [SA_SIZE];
  logic [ACC_W-1:0] sa_c_row [SA_SIZE];
  logic [ACC_W-1:0] sa_result_row [SA_SIZE];
  logic [SA_SIZE-1:0] sa_cur_row;
  logic sa_load_a, sa_load_b, sa_valid;

  // ============================================================
  //  Pipeline stall
  // ============================================================
  wire out_stall  = (state == COMPUTE) && step_first && result_v && !m_axis_res_tready;
  wire pipe_stall = out_stall;

  // ============================================================
  //  Kernel loading
  // ============================================================
  assign s_axis_kernel_tready = (state == IDLE || state == KERNEL) && !out_stall;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      kernel_cnt  <= 0;
    end else if (!out_stall) begin
      if ((state == KERNEL || state == IDLE) && s_axis_kernel_tvalid) begin
        kernel_reg[int'(kernel_cnt)] <= s_axis_kernel_tdata;
        kernel_cnt <= kernel_cnt + 1;
      end
    end
  end

  // ============================================================
  //  B‑load counter
  // ============================================================
  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      b_cycle <= 0;
    end else if (!pipe_stall) begin
      if (state == BLOAD) b_cycle <= b_cycle + 1;
      else                b_cycle <= 0;
    end
  end

  // ============================================================
  //  SA drive (combinational)
  //
  //  The SA reads a_inner_loop[i][i] as the a input for every PE
  //  in row i.  Loading all window values in one cycle (step_first)
  //  and a_row = 0 on subsequent steps naturally produces the
  //  anti‑diagonal skew: PE(i) sees window[i] at step i.
  // ============================================================
  always_comb begin
    for (int gi = 0; gi < K*K; gi++)
      sa_a_row[gi] = (state == COMPUTE && step_first) ? window_cap[gi] : '0;
    for (int gi = K*K; gi < SA_SIZE; gi++)
      sa_a_row[gi] = '0;
  end

  genvar gi;
  generate
    for (gi = 0; gi < SA_SIZE; gi++) begin : gen_sa_b
      assign sa_b_row[gi] = (gi == 0 && state == BLOAD && int'(b_cycle) < int'(K*K))
                              ? kernel_reg[int'(b_cycle)] : '0;
    end
    for (gi = 0; gi < SA_SIZE; gi++) begin : gen_sa_c
      assign sa_c_row[gi] = '0;
    end
  endgenerate

  assign sa_cur_row = (state == BLOAD) ? (1 << b_cycle) : '0;
  assign sa_load_a  = (state == BLOAD) || (state == COMPUTE);
  assign sa_load_b  = (state == BLOAD);

  // loop_len_a controls the circular‑buffer depth in the SA.
  // With load_a=1 throughout COMPUTE, the else‑branch is never
  // taken, so any value works.  SA_SIZE is conventional.
  assign sa_valid   = ((state == BLOAD) || (state == COMPUTE)) && !pipe_stall;

  // ============================================================
  //  SA instance
  // ============================================================
  SA #(
      .SIZE          (SA_SIZE),
      .DATA_WIDTH_IN (DW),
      .DATA_WIDTH_OUT(ACC_W)
  ) sa_core (
      .clk        (clk),
      .rst_n      (rst_n),
      .valid      (sa_valid),
      .current_row(sa_cur_row),
      .load_a     (sa_load_a),
      .load_b     (sa_load_b),
      .loop_len_a (SA_SIZE[7:0]),
      .a_row      (sa_a_row),
      .b_row      (sa_b_row),
      .c_row      (sa_c_row),
      .result_row (sa_result_row)
  );

  // ============================================================
  //  Line buffer state
  // ============================================================
  assign window_valid_comb = (row_rx >= K - 1) && (wr_col >= K - 1) && in_frame;
  assign new_window = s_axis_pix_tvalid && s_axis_pix_tready && window_valid_comb;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      wr_row   <= 0;
      wr_col   <= 0;
      row_rx   <= 0;
      in_frame <= 0;
    end else begin
      if (s_axis_pix_tuser) begin
        wr_row   <= 0;
        wr_col   <= 0;
        row_rx   <= 0;
        in_frame <= 1;
      end

      if (s_axis_pix_tvalid && s_axis_pix_tready) begin
        line_buf[wr_row][wr_col] <= s_axis_pix_tdata;

        if (s_axis_pix_tlast) begin
          wr_row <= (wr_row + 1 >= K) ? 0 : wr_row + 1;
          wr_col <= 0;
          row_rx <= row_rx + 1;
          if (row_rx == IMG_H - 1) in_frame <= 0;
        end else begin
          wr_col <= wr_col + 1;
        end
      end
    end
  end

  // ============================================================
  //  Window capture
  //
  //  At new_window   — wr_col is the bottom‑right column (BEFORE
  //                    increment); the bottom‑right pixel is on
  //                    s_axis_pix_tdata because the NBA write to
  //                    line_buf hasn't taken effect.
  //  During COMPUTE  — a complete K×K window is available whenever
  //                    window_valid_comb is true (wr_col >= K-1).
  //                    We capture when a new pixel is written
  //                    (new_window fires) rather than at step_last,
  //                    because after the final tlast wr_col is
  //                    reset to 0 and the step_last index would
  //                    be out of bounds.
  // ============================================================
  wire capture_next = new_window;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      for (int i = 0; i < K * K; i++) window_cap[i] <= '0;
    end else if (state == KERNEL || state == BLOAD) begin
      for (int i = 0; i < K * K; i++) window_cap[i] <= '0;
    end else if (capture_next) begin
      for (int ky = 0; ky < K; ky++) begin
        for (int kx = 0; kx < K; kx++) begin
          automatic int r = (int'(wr_row) + 1 + ky) % int'(K);
          automatic int c;
          if (new_window) begin
            // wr_col = pixel column (before increment)
            c = int'(wr_col) - int'(K) + 1 + kx;
          end else begin
            // wr_col was incremented past the pixel column
            c = int'(wr_col) - int'(K) + kx;
          end
          if (ky == K - 1 && kx == K - 1 && new_window)
            window_cap[ky * K + kx] <= s_axis_pix_tdata;
          else
            window_cap[ky * K + kx] <= line_buf[r][c];
        end
      end
    end
  end

  // ============================================================
  //  Step counter
  // ============================================================
  assign step_first = (step == 0);
  assign step_last  = (int'(step) == int'(SA_SIZE));

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      step <= 0;
    end else if (!pipe_stall) begin
      if (state == COMPUTE) begin
        if (step_last) step <= 0;
        else           step <= step + 1;
      end else begin
        step <= 0;
      end
    end
  end

  // ============================================================
  //  Window / result counters
  // ============================================================
  assign win_done = (int'(win_cnt) >= int'(TOTAL_WINDOWS));

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      win_cnt <= 0;
    end else if (!pipe_stall) begin
      if (state == COMPUTE && step_last) begin
        win_cnt <= win_cnt + 1;
      end else if (state == IDLE) begin
        win_cnt <= 0;
      end
    end
  end

  // ============================================================
  //  Result output
  //
  //  PE pipeline depth = SA_SIZE, loaded at step_first, result
  //  emerges at step 0 of the NEXT cycle.  Register result_v
  //  an extra stage so it matches the SA pipeline delay.
  // ============================================================
  logic result_v, result_v_d1;

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) {result_v, result_v_d1} <= 0;
    else begin
      result_v_d1 <= (state == COMPUTE) && step_last;
      result_v    <= result_v_d1;
    end
  end

  assign m_axis_res_tvalid = result_v;
  assign m_axis_res_tdata  = sa_result_row[0];
  assign m_axis_res_tlast  = result_v && win_done;

  // ============================================================
  //  Pixel ready
  // ============================================================
  assign s_axis_pix_tready = (state == FILL);

  // ============================================================
  //  FSM (independent of pipe_stall)
  // ============================================================
  always_comb begin
    state_nxt = state;
    case (state)
      IDLE:    if (s_axis_kernel_tvalid)               state_nxt = KERNEL;
      KERNEL:  if (kernel_done)                         state_nxt = BLOAD;
      BLOAD:   if (int'(b_cycle) == int'(SA_SIZE - 1))  state_nxt = FILL;
      FILL:    if (new_window)                          state_nxt = COMPUTE;
      COMPUTE: if (win_done && !out_stall)                     state_nxt = FINISH;
               else if (step_last && !pipe_stall)                state_nxt = FILL;
      FINISH:                                           state_nxt = IDLE;
      default:                                           state_nxt = IDLE;
    endcase
  end

  always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) state <= IDLE;
    else        state <= state_nxt;
  end

endmodule
