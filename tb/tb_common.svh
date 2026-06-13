`ifndef TB_COMMON_SVH
`define TB_COMMON_SVH
// -----------------------------------------------------------------------
// Shared test data, helpers, and tasks for sa_wrapper_axi_ctrl_tb
// -----------------------------------------------------------------------

// Register address constants (matches defs.svh + wrapper)
localparam REG_STATE        = 5'h00;
localparam REG_STATUS       = 5'h04;
localparam REG_C_LOAD       = 5'h08;
localparam REG_FB_CNT       = 5'h0C;
localparam REG_A_LOAD       = 5'h10;
localparam REG_ACC_OUT      = 5'h14;  // unused in current RTL, kept for compat
localparam REG_A_LOOP_START = 5'h18;
localparam REG_A_LOOP_END   = 5'h1C;
localparam REG_C_LOOP_START = 5'h20;
localparam REG_C_LOOP_END   = 5'h24;
localparam REG_RST_INDEX    = 5'h2C;

// Test matrices
int A[4][4] = '{'{10, 11, 12, 13}, '{11, 12, 13, 14}, '{12, 13, 14, 15}, '{13, 14, 15, 16}};
int B1[4][4] = '{'{1, 2, 3, 4}, '{5, 6, 7, 8}, '{9, 10, 11, 12}, '{13, 14, 15, 16}};
int B2[4][4] = '{'{0, 1, 0, 0}, '{1, 0, 0, 0}, '{0, 0, 0, 1}, '{0, 0, 1, 0}};
int B_eye[4][4] = '{'{1, 0, 0, 0}, '{0, 1, 0, 0}, '{0, 0, 1, 0}, '{0, 0, 0, 1}};
int A2[4][4] = '{'{20,21,22,23}, '{21,22,23,24}, '{22,23,24,25}, '{23,24,25,26}};

// Expected results (computed in initial block)
int exp_B1[4][4], exp_B2[4][4], exp_eye[4][4], exp_acc[4][4], exp_3way[4][4];
int exp_AB1[4][4];
int exp_A1B[4][4], exp_A2B[4][4];
int exp_ring_acc[4][4];
int Bneg[4][4], exp_neg[4][4];

// -----------------------------------------------------------------------
// helper functions
// -----------------------------------------------------------------------
function automatic int matmul_row_sum(int row[4], int B[4][4], int col);
  int s = 0;
  for (int k = 0; k < SIZE; k++) s += row[k] * B[k][col];
  return s;
endfunction

function automatic void matmul(int A[4][4], int B[4][4], ref int out[4][4]);
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) out[r][c] = matmul_row_sum(A[r], B, c);
endfunction

// -----------------------------------------------------------------------
// tasks
// -----------------------------------------------------------------------
task axil_read(input [5:0] addr, output [31:0] data);
  s_axil_arvalid = 1; s_axil_araddr = addr;
  @(posedge clk);
  while (!s_axil_rvalid) @(posedge clk);
  data = s_axil_rdata;
  s_axil_arvalid = 0; s_axil_rready = 1;
  @(posedge clk);
  s_axil_rready = 0;
endtask

task axil_write(input [5:0] addr, input [31:0] data);
  @(posedge clk);
  s_axil_awvalid = 1; s_axil_awaddr = addr; s_axil_wdata = data; s_axil_wvalid = 1;
  @(posedge clk);
  while (!s_axil_bvalid) @(posedge clk);
  s_axil_awvalid = 0; s_axil_wvalid = 0; s_axil_bready = 1;
  @(posedge clk);
  s_axil_bready = 0;
endtask

task stream_mat(input int mat[4][4], input bit last_set = 1);
  automatic logic [AXI_IN_W-1:0] row_data;
  for (int i = 0; i < SIZE; i++) begin
    row_data = 0;
    for (int c = 0; c < SIZE; c++)
      row_data[c*DWI+:DWI] = 8'(mat[i][c]);
    wait (s_axis_B_tready);
    @(posedge clk);
    #1;
    s_axis_B_tdata = row_data;
    s_axis_B_tvalid = 1;
    s_axis_B_tlast = last_set && (i == SIZE - 1);
  end
  wait (s_axis_B_tready);
  @(posedge clk);
  #1;
  s_axis_B_tvalid = 0;
  s_axis_B_tlast = 0;
endtask

task stream_mat_n(input int mat[4][4], input int nrows, input bit last_set = 1);
  automatic logic [AXI_IN_W-1:0] row_data;
  for (int i = 0; i < nrows; i++) begin
    row_data = 0;
    for (int c = 0; c < SIZE; c++)
      row_data[c*DWI+:DWI] = 8'(mat[i][c]);
    wait (s_axis_B_tready);
    @(posedge clk);
    #1;
    s_axis_B_tdata = row_data;
    s_axis_B_tvalid = 1;
    s_axis_B_tlast = last_set && (i == nrows - 1);
  end
  wait (s_axis_B_tready);
  @(posedge clk);
  #1;
  s_axis_B_tvalid = 0;
  s_axis_B_tlast = 0;
endtask

task load_A();
  axil_write(5'h1C, SIZE - 1);
  axil_write(5'h10, 0);
  stream_mat(A);
endtask

task reset_test();
  rst_n = 0;
  repeat (5) @(posedge clk);
  rst_n = 1;
  @(posedge clk);
endtask

task wait_output_done();
  while (out_count == 0) @(posedge clk);
  while (!result_tlast[out_count-1]) @(posedge clk);
endtask

task soft_rst_via_axil();
  axil_write(6'h2C, 0);
endtask

task check_row_str(string label, int idx, int expected[4]);
  for (int c = 0; c < SIZE; c++)
    if (result[idx][c] !== expected[c]) begin
      $display("  %s [%0d]: got %0d, exp %0d", label, c, result[idx][c], expected[c]);
      errors++;
    end
endtask
`endif
