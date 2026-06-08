`timescale 1ns / 1ps

module sa_wrapper_axi_test #(
    parameter unsigned SIZE            = 4,
    parameter unsigned DATA_WIDTH_IN   = 8,
    parameter unsigned DATA_WIDTH_OUT  = 32,
    localparam unsigned AXI_IN_WIDTH   = SIZE * DATA_WIDTH_IN,
    localparam unsigned AXI_OUT_WIDTH  = SIZE * DATA_WIDTH_OUT
) (
    input wire clk,
    input wire rst_n,

    input  wire [AXI_IN_WIDTH-1:0]  s_axis_A_tdata,
    input  wire                     s_axis_A_tvalid,
    output wire                     s_axis_A_tready,
    input  wire                     s_axis_A_tlast,

    input  wire [AXI_IN_WIDTH-1:0]  s_axis_B_tdata,
    input  wire                     s_axis_B_tvalid,
    output wire                     s_axis_B_tready,
    input  wire                     s_axis_B_tlast,

    output wire [AXI_OUT_WIDTH-1:0] m_axis_tdata,
    output reg                      m_axis_tvalid,
    input  wire                     m_axis_tready,
    output reg                      m_axis_tlast
);

  localparam ROW_BITS = SIZE > 1 ? $clog2(SIZE) : 1;

  reg [SIZE-1:0]   cur_row;
  reg [ROW_BITS:0] beat_cnt;
  reg [ROW_BITS:0] drain_cnt;
  reg              draining;
  reg              first_drain;

  reg [AXI_OUT_WIDTH-1:0] m_axis_tdata_reg;

  reg                    a_full, b_full;
  reg [AXI_IN_WIDTH-1:0] a_data, b_data;
  reg                    a_last, b_last;

  wire can_output = !m_axis_tvalid || m_axis_tready;

  assign s_axis_A_tready = !a_full;
  assign s_axis_B_tready = !b_full;

  wire consume = a_full && b_full && can_output && !draining;
  wire drain_active = draining && (drain_cnt > 0);
  wire drain_valid = drain_active && can_output;
  wire valid = consume || drain_valid;

  wire load_b = (beat_cnt < SIZE);

  wire [DATA_WIDTH_IN-1:0] a_row[SIZE];
  wire [DATA_WIDTH_IN-1:0] b_row[SIZE];
  wire [DATA_WIDTH_OUT-1:0] c_row[SIZE] = '{default: '0};
  wire [DATA_WIDTH_OUT-1:0] result_row[SIZE];

  genvar gi;
  generate
    for (gi = 0; gi < SIZE; gi++) begin : gen_a_row
      assign a_row[gi] = drain_active ? '0 : a_data[gi*DATA_WIDTH_IN+:DATA_WIDTH_IN];
    end
    for (gi = 0; gi < SIZE; gi++) begin : gen_b_row
      assign b_row[gi] = drain_active ? '0 : b_data[gi*DATA_WIDTH_IN+:DATA_WIDTH_IN];
    end
  endgenerate

  SA #(
      .SIZE(SIZE),
      .MAX_LOOP(3 * SIZE),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) sa_core (
      .clk  (clk),
      .rst_n(rst_n),
      .valid(valid),
      .current_row(cur_row),
      .load_a(1'b1),
      .load_b(load_b),
      .loop_len_a(SIZE),
      .a_row(a_row),
      .b_row(b_row),
      .c_row(c_row),
      .result_row(result_row)
  );

  generate
    for (gi = 0; gi < SIZE; gi++) begin : gen_pack
      assign m_axis_tdata[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] =
             m_axis_tdata_reg[gi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT];
    end
  endgenerate

  wire output_available = drain_active && !first_drain;

  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      cur_row          <= 1;
      beat_cnt         <= 0;
      drain_cnt        <= 0;
      draining         <= 0;
      first_drain      <= 1;
      m_axis_tdata_reg <= 0;
      m_axis_tvalid    <= 0;
      m_axis_tlast     <= 0;
      a_full           <= 0;
      b_full           <= 0;
      a_data           <= 0;
      b_data           <= 0;
      a_last           <= 0;
      b_last           <= 0;
    end else begin
      if (s_axis_A_tvalid && !a_full) begin
        a_data <= s_axis_A_tdata;
        a_last <= s_axis_A_tlast;
      end
      if (s_axis_B_tvalid && !b_full) begin
        b_data <= s_axis_B_tdata;
        b_last <= s_axis_B_tlast;
      end

      if (consume) begin
        a_full <= s_axis_A_tvalid && !a_full;
        b_full <= s_axis_B_tvalid && !b_full;
      end else begin
        if (s_axis_A_tvalid && !a_full) a_full <= 1;
        if (s_axis_B_tvalid && !b_full) b_full <= 1;
      end

      if (valid) begin
        cur_row <= {cur_row[SIZE-2:0], cur_row[SIZE-1]};
      end

      if (consume) begin
        beat_cnt <= beat_cnt + 1;
      end

      if (consume && (a_last || b_last)) begin
        draining  <= 1;
        drain_cnt <= SIZE + 1;
      end

      if (drain_valid) begin
        drain_cnt   <= drain_cnt - 1;
        first_drain <= 0;
        if (drain_cnt == 1) begin
          draining <= 0;
        end
      end

      if (output_available && valid) begin
        for (int i = 0; i < SIZE; i++) begin
          m_axis_tdata_reg[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] <= result_row[i];
        end
        m_axis_tvalid <= 1;
        m_axis_tlast  <= drain_active && (drain_cnt == 1);
      end else if (m_axis_tvalid && m_axis_tready) begin
        m_axis_tvalid <= 0;
        m_axis_tlast  <= 0;
      end
    end
  end

endmodule