`timescale 1ns / 1ps

module sa_wrapper_tb;
  reg clk;
  reg rst_n;

  localparam unsigned Size = 4;
  localparam unsigned DWI = 8;
  localparam unsigned DWO = 32;
  localparam unsigned SAxisW = 16;
  localparam unsigned MAxisW = SAxisW * (DWO / DWI);


  logic [SAxisW-1 : 0] s_axis_A_tdata;
  logic s_axis_A_tvalid;
  logic s_axis_A_tready;
  logic s_axis_A_tlast;

  logic [SAxisW-1 : 0] s_axis_B_tdata;
  logic s_axis_B_tvalid;
  logic s_axis_B_tready;
  logic s_axis_B_tlast;

  logic [MAxisW-1 : 0] m_axis_tdata;
  logic m_axis_tvalid;
  logic m_axis_tready;
  logic m_axis_tlast;

  SA_wrapper #(
      .SIZE(Size),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO),
      .C_S_AXIS_DATA_WIDTH(SAxisW)
  ) sa_wrapper_top (
      .aclk(clk),
      .aresetn(rst_n),

      .s_axis_A_tdata (s_axis_A_tdata),
      .s_axis_A_tvalid(s_axis_A_tvalid),
      .s_axis_A_tready(s_axis_A_tready),
      .s_axis_A_tlast (s_axis_A_tlast),

      .s_axis_B_tdata (s_axis_B_tdata),
      .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready),
      .s_axis_B_tlast (s_axis_B_tlast),

      .m_axis_tdata (m_axis_tdata),
      .m_axis_tvalid(m_axis_tvalid),
      .m_axis_tready(m_axis_tready),
      .m_axis_tlast (m_axis_tlast)
  );

  always #1 clk = ~clk;

  localparam unsigned ELEMS_PER_WORD = SAxisW / DWI;
  localparam unsigned Cols = ((Size + ELEMS_PER_WORD) / ELEMS_PER_WORD) - 1;
  localparam unsigned ColsW = Cols > 1 ? $clog2(Cols) : 1;

  task load(logic [DWI-1:0] a_c[Size], logic [DWI-1:0] b_r[Size], logic last = 0);
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;
    for (int c = 0; c < Cols; c++) begin
      for (int i = 0; i < ELEMS_PER_WORD; i++) begin
        s_axis_A_tdata[i*DWI+:DWI] = a_c[c*ELEMS_PER_WORD+i];
        s_axis_B_tdata[i*DWI+:DWI] = b_r[c*ELEMS_PER_WORD+i];
      end
      s_axis_A_tvalid = 1;
      s_axis_B_tvalid = 1;
      if (last && c == Cols - 1) begin
        s_axis_A_tlast = 1;
        s_axis_B_tlast = 1;
      end
      @(negedge clk);
    end
    s_axis_A_tlast  = 0;
    s_axis_B_tlast  = 0;
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
  endtask

  // logic [ELEMS_PER_WORD-1:0][DWO - 1:0] partial_result_row_packed;
  logic [DWO - 1:0] partial_result_row[ELEMS_PER_WORD];

  // assign partial_result_row_packed = {>>{m_axis_tdata}};
  genvar i;
  generate
    for (i = 0; i < ELEMS_PER_WORD; i++) begin
      assign partial_result_row[i] = m_axis_tdata[i*DWO+:DWO];
    end
  endgenerate

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_tb);

    rst_n = 0;
    repeat (2) @(negedge clk);
    rst_n = 1;
    repeat (2) @(negedge clk);

    m_axis_tready = 1;
    wait (s_axis_A_tready && s_axis_B_tready);
    @(negedge clk);

    load('{1, 2, 3, 4}, '{4, 3, 2, 1});
    repeat (10) @(negedge clk);
    load('{8, 8, 8, 8}, '{9, 9, 9, 9});
    load('{5, 5, 5, 5}, '{6, 6, 6, 6});
    repeat (20) @(negedge clk);
    load('{6, 7, 8, 9}, '{9, 8, 7, 6});

    load('{1, 2, 3, 4}, '{4, 3, 2, 1});
    load('{8, 8, 8, 8}, '{9, 9, 9, 9});
    load('{5, 5, 5, 5}, '{6, 6, 6, 6});
    load('{6, 7, 8, 9}, '{9, 8, 7, 6});

    load('{3, 4, 5, 6}, '{6, 5, 4, 3});
    load('{6, 6, 6, 6}, '{7, 7, 7, 7});
    load('{7, 7, 7, 7}, '{8, 8, 8, 8});
    load('{5, 6, 7, 8}, '{8, 7, 6, 5}, 1);

    #500 $finish();

  end


endmodule
