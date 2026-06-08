`timescale 1ns / 1ps

module sa_wrapper_axi_test_tb;
  reg clk, rst_n;

  localparam unsigned Size = 4;
  localparam unsigned DWI = 8;
  localparam unsigned DWO = 32;
  localparam unsigned AXI_IW = Size * DWI;
  localparam unsigned AXI_OW = Size * DWO;

  reg [AXI_IW-1:0] s_axis_A_tdata;
  reg              s_axis_A_tvalid;
  wire             s_axis_A_tready;
  reg              s_axis_A_tlast;

  reg [AXI_IW-1:0] s_axis_B_tdata;
  reg              s_axis_B_tvalid;
  wire             s_axis_B_tready;
  reg              s_axis_B_tlast;

  wire [AXI_OW-1:0] m_axis_tdata;
  reg               m_axis_tready;
  wire              m_axis_tvalid;
  wire              m_axis_tlast;

  sa_wrapper_axi_test #(
      .SIZE(Size),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO)
  ) dut (
      .clk(clk),
      .rst_n(rst_n),
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

  wire [DWO-1:0] result_row[Size];
  genvar gi;
  generate
    for (gi = 0; gi < Size; gi++) begin
      assign result_row[gi] = m_axis_tdata[gi*DWO+:DWO];
    end
  endgenerate

  task send(logic [DWI-1:0] a[Size], logic [DWI-1:0] b[Size],
            logic last = 0);
    for (int i = 0; i < Size; i++) begin
      s_axis_A_tdata[i*DWI+:DWI] = a[i];
      s_axis_B_tdata[i*DWI+:DWI] = b[i];
    end
    s_axis_A_tlast = last;
    s_axis_B_tlast = last;
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 1;
    @(posedge clk);
    while (!s_axis_A_tready) @(posedge clk);
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    s_axis_A_tlast = 0;
    s_axis_B_tlast = 0;
  endtask

  int out_cnt;
  int errors;

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_wrapper_axi_test_tb);

    clk = 0;
    rst_n = 0;
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    m_axis_tready = 1;
    out_cnt = 0;
    errors = 0;

    repeat (4) @(posedge clk);
    rst_n = 1;
    repeat (2) @(posedge clk);

    // send B and A data (4 cycles)
    send('{1, 8, 5, 6}, '{4, 3, 2, 1});
    send('{2, 8, 5, 7}, '{9, 9, 9, 9});
    send('{3, 8, 5, 8}, '{6, 6, 6, 6});
    send('{4, 8, 5, 9}, '{9, 8, 7, 6}, 1);

    // drain is handled internally by the wrapper
    repeat (Size * 3) @(posedge clk);

    if (errors != 0) begin
      $write("FAIL: %0d mismatches\n", errors);
    end else begin
      $write("PASS: all %0d outputs match\n", out_cnt);
    end
    $finish();
  end

  // expected outputs (computed by manual trace)
  int expected[4][4] = '{
      '{160, 153, 146, 139},
      '{173, 164, 155, 146},
      '{186, 175, 164, 153},
      '{199, 186, 173, 160}
  };

  // monitor
  always @(posedge clk) begin
    if (m_axis_tvalid && m_axis_tready) begin
      $write("OUT[%0d] = [%0d,%0d,%0d,%0d]",
             $time, result_row[0], result_row[1], result_row[2], result_row[3]);
      if (m_axis_tlast) $write(" LAST");
      $write("\n");

      if (out_cnt < 4) begin
        for (int i = 0; i < Size; i++) begin
          if (result_row[i] !== expected[out_cnt][i]) begin
            $write("  MISMATCH col %0d: got %0d, expected %0d\n",
                   i, result_row[i], expected[out_cnt][i]);
            errors++;
          end
        end
      end
      if (!m_axis_tlast && out_cnt >= 3) begin
        $write("  Expected LAST on final output\n");
        errors++;
      end
      out_cnt++;
    end
  end

endmodule
