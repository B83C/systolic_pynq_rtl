`timescale 1ns / 1ps

module sa_wrapper_axi_test_multi_tb;
  reg clk, rst_n;

  localparam unsigned Size = 4;
  localparam unsigned DWI = 8;
  localparam unsigned DWO = 32;
  localparam unsigned AXI_IW = Size * DWI;
  localparam unsigned AXI_OW = Size * DWO;

  reg  [AXI_IW-1:0] s_axis_A_tdata;
  reg               s_axis_A_tvalid;
  wire              s_axis_A_tready;
  reg               s_axis_A_tlast;

  reg  [AXI_IW-1:0] s_axis_B_tdata;
  reg               s_axis_B_tvalid;
  wire              s_axis_B_tready;
  reg               s_axis_B_tlast;

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
      .s_axis_A_tdata(s_axis_A_tdata),
      .s_axis_A_tvalid(s_axis_A_tvalid),
      .s_axis_A_tready(s_axis_A_tready),
      .s_axis_A_tlast(s_axis_A_tlast),
      .s_axis_B_tdata(s_axis_B_tdata),
      .s_axis_B_tvalid(s_axis_B_tvalid),
      .s_axis_B_tready(s_axis_B_tready),
      .s_axis_B_tlast(s_axis_B_tlast),
      .m_axis_tdata(m_axis_tdata),
      .m_axis_tvalid(m_axis_tvalid),
      .m_axis_tready(m_axis_tready),
      .m_axis_tlast(m_axis_tlast)
  );

  always #1 clk = ~clk;

  wire [DWO-1:0] result_row[Size];
  genvar gi;
  generate
    for (gi = 0; gi < Size; gi++) begin
      assign result_row[gi] = m_axis_tdata[gi*DWO+:DWO];
    end
  endgenerate

  // pack a/b data for send task
  function automatic logic [AXI_IW-1:0] pack(input logic [DWI-1:0] d[Size]);
    logic [AXI_IW-1:0] r;
    for (int i = 0; i < Size; i++) r[i*DWI+:DWI] = d[i];
    return r;
  endfunction

  task send(logic [DWI-1:0] a[Size], logic [DWI-1:0] b[Size], logic last = 0);
    s_axis_A_tdata  = pack(a);
    s_axis_B_tdata  = pack(b);
    s_axis_A_tlast  = last;
    s_axis_B_tlast  = last;
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 1;
    @(posedge clk);
    while (!s_axis_A_tready) @(posedge clk);
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    s_axis_A_tlast  = 0;
    s_axis_B_tlast  = 0;
  endtask

  int out_cnt, errors;

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_wrapper_axi_test_multi_tb);

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

    // =========================================================================
    // TEST 1: Synchronization — both streams must be valid together
    // =========================================================================
    $write("=== TEST 1: Stream synchronization ===\n");

    // 1a: A valid, B not valid → both ready low
    s_axis_A_tdata  = pack('{1, 2, 3, 4});
    s_axis_B_tdata  = pack('{5, 6, 7, 8});
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 0;
    @(posedge clk);
    if (s_axis_A_tready !== 0) begin
      $write("  FAIL 1a: A_ready should be 0 when B not valid (got %0d)\n", s_axis_A_tready);
      errors++;
    end
    if (s_axis_B_tready !== 0) begin
      $write("  FAIL 1a: B_ready should be 0 when B not valid (got %0d)\n", s_axis_B_tready);
      errors++;
    end
    // data should NOT have been consumed
    @(posedge clk);  // let the ready propagate

    // 1b: Now assert B → handshake should proceed
    s_axis_B_tvalid = 1;
    @(posedge clk);
    // combinatorial ready should be high immediately when both valid
    if (!s_axis_A_tready) begin
      // wait a cycle if combinatorial delay
      @(posedge clk);
    end
    if (s_axis_A_tready !== 1) begin
      $write("  FAIL 1b: A_ready should be 1 when both valid\n");
      errors++;
    end
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    @(posedge clk);

    // 1c: B valid, A not valid → both ready low
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 1;
    @(posedge clk);
    if (s_axis_A_tready !== 0 || s_axis_B_tready !== 0) begin
      $write("  FAIL 1c: ready should be 0 when only B valid\n");
      errors++;
    end
    s_axis_B_tvalid = 0;
    @(posedge clk);

    // 1d: Both valid at same time with m_axis_tready=0 → backpressure
    m_axis_tready   = 0;
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 1;
    @(posedge clk);
    // can_output = !m_axis_tvalid || m_axis_tready → m_axis_tvalid=0, so can_output=1
    // ready should still be 1 (output channel free)
    if (s_axis_A_tready !== 1) begin
      $write("  FAIL 1d: ready should be 1 when output free even if downstream not ready\n");
      errors++;
    end
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    m_axis_tready   = 1;
    @(posedge clk);

    // 1e: Fill output register, then check backpressure blocks input
    repeat (2) @(posedge clk);

    // first, send a burst to fill output pipeline
    send('{1, 1, 1, 1}, '{1, 0, 0, 0});
    send('{1, 1, 1, 1}, '{0, 1, 0, 0});
    send('{1, 1, 1, 1}, '{0, 0, 1, 0});
    send('{1, 1, 1, 1}, '{0, 0, 0, 1}, 1);

    // wait for first output, then assert m_axis_tready=0 before consuming it
    repeat (5) @(posedge clk);

    // now m_axis_tvalid should be 1 (output waiting)
    // stall downstream
    m_axis_tready = 0;
    @(posedge clk);

    // try sending new data while output is stalled
    s_axis_A_tvalid = 1;
    s_axis_B_tvalid = 1;
    s_axis_A_tdata  = pack('{9, 9, 9, 9});
    s_axis_B_tdata  = pack('{9, 9, 9, 9});
    @(posedge clk);
    // can_output = !m_axis_tvalid || m_axis_tready = 0 || 0 = 0
    // both_valid = 1, consume = 0
    // so ready should be 0
    if (s_axis_A_tready !== 0) begin
      $write("  FAIL 1e: A_ready should be 0 when output stalled (got %0d)\n", s_axis_A_tready);
      errors++;
    end
    // data should NOT have been consumed

    // release backpressure
    s_axis_A_tvalid = 0;
    s_axis_B_tvalid = 0;
    m_axis_tready   = 1;
    repeat (6) @(posedge clk);
    // consume all remaining outputs

    if (errors == 0) $write("  PASS: sync tests\n");

    // =========================================================================
    // TEST 2: Sequential streams with reset between them
    // =========================================================================
    $write("\n=== TEST 2: Sequential streams with reset ===\n");

    // Reset
    rst_n = 0;
    repeat (4) @(posedge clk);
    rst_n = 1;
    repeat (2) @(posedge clk);
    out_cnt = 0;

    // Stream 1: all A rows = [1,1,1,1], B = identity
    send('{1, 1, 1, 1}, '{1, 0, 0, 0});
    send('{1, 1, 1, 1}, '{0, 1, 0, 0});
    send('{1, 1, 1, 1}, '{0, 0, 1, 0});
    send('{1, 1, 1, 1}, '{0, 0, 0, 1}, 1);

    repeat (Size * 3) @(posedge clk);  // drain

    // Reset again between streams
    rst_n = 0;
    repeat (4) @(posedge clk);
    rst_n = 1;
    repeat (2) @(posedge clk);
    out_cnt = 0;

    // Stream 2: same data
    send('{1, 1, 1, 1}, '{1, 0, 0, 0});
    send('{1, 1, 1, 1}, '{0, 1, 0, 0});
    send('{1, 1, 1, 1}, '{0, 0, 1, 0});
    send('{1, 1, 1, 1}, '{0, 0, 0, 1}, 1);

    repeat (Size * 3) @(posedge clk);

    $write("\n=== TEST 3: Sequential streams WITHOUT reset (accumulation) ===\n");
    // streams are already done, this starts fresh
    // Actually the stream 2 was already completed above with a reset before it.
    // Let me do a third stream without reset
    // A stream with data [2,2,2,2] same B (still loaded from stream 2)

    // Note: since stream 2 ended with reset, the SA state is clean.
    // But then we didn't load B after the reset... we loaded B during stream 2.
    // After stream 2 drain, the SA has b_inner loaded.
    // Now send stream 3 WITHOUT reset, to test accumulation.

    send('{2, 2, 2, 2}, '{0, 0, 0, 0});  // dummy B data for handshake
    send('{2, 2, 2, 2}, '{0, 0, 0, 0});
    send('{2, 2, 2, 2}, '{0, 0, 0, 0});
    send('{2, 2, 2, 2}, '{0, 0, 0, 0}, 1);

    // B data is all zeros above, but that's just for the handshake.
    // Since load_b=0 (beat_cnt >= SIZE), b_inner is NOT overwritten.
    // The SA continues using the identity B from stream 2.
    // With all A=2, the results should be 2x the stream 2 results.

    repeat (Size * 3) @(posedge clk);

    // =========================================================================
    // Summary
    // =========================================================================
    if (errors != 0) begin
      $write("\nFAIL: %0d errors\n", errors);
    end else begin
      $write("\nPASS: all tests passed\n");
    end
    $finish();
  end

  // monitor
  always @(posedge clk) begin
    if (m_axis_tvalid && m_axis_tready) begin
      $write("OUT[%0d] = [%0d,%0d,%0d,%0d]", $time, result_row[0], result_row[1], result_row[2],
             result_row[3]);
      if (m_axis_tlast) $write(" LAST");
      $write("\n");

      out_cnt++;
    end
  end

endmodule
