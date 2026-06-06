`timescale 1ns / 1ps

module SA_wrapper #(
    parameter unsigned SIZE = 8,
    parameter unsigned DATA_WIDTH_IN = 8,
    parameter unsigned DATA_WIDTH_OUT = 32,
    parameter unsigned C_S_AXIS_DATA_WIDTH = 64,
    parameter unsigned C_M_AXIS_DATA_WIDTH = C_S_AXIS_DATA_WIDTH * (DATA_WIDTH_OUT / DATA_WIDTH_IN)
) (
    input wire aclk,
    input wire aresetn,

    input wire [C_S_AXIS_DATA_WIDTH-1 : 0] s_axis_A_tdata,
    input wire s_axis_A_tvalid,
    output wire s_axis_A_tready,
    input wire s_axis_A_tlast,

    input wire [C_S_AXIS_DATA_WIDTH-1 : 0] s_axis_B_tdata,
    input wire s_axis_B_tvalid,
    output wire s_axis_B_tready,
    input wire s_axis_B_tlast,

    output [C_M_AXIS_DATA_WIDTH-1 : 0] m_axis_tdata,
    output m_axis_tvalid,
    input wire m_axis_tready,
    output wire m_axis_tlast
);

  initial begin
    if (DATA_WIDTH_OUT % DATA_WIDTH_IN != 0)
      $fatal(
          1,
          "DATA_WIDTH_OUT (%0d) must be a multiple of DATA_WIDTH_IN (%0d)",
          DATA_WIDTH_OUT,
          DATA_WIDTH_IN
      );
    if (C_S_AXIS_DATA_WIDTH % DATA_WIDTH_IN != 0)
      $fatal(
          1,
          "C_S_AXIS_DATA_WIDTH (%0d) must be a multiple of DATA_WIDTH_IN (%0d)",
          C_S_AXIS_DATA_WIDTH,
          DATA_WIDTH_IN
      );
    if (C_M_AXIS_DATA_WIDTH % DATA_WIDTH_OUT != 0)
      $fatal(
          1,
          "C_M_AXIS_DATA_WIDTH (%0d) must be a multiple of DATA_WIDTH_OUT (%0d)",
          C_M_AXIS_DATA_WIDTH,
          DATA_WIDTH_OUT
      );
    if (C_M_AXIS_DATA_WIDTH / DATA_WIDTH_OUT != C_S_AXIS_DATA_WIDTH / DATA_WIDTH_IN)
      $fatal(
          1,
          "C_M_AXIS_DATA_WIDTH/DATA_WIDTH_OUT (%0d) must equal C_S_AXIS_DATA_WIDTH/DATA_WIDTH_IN (%0d)",
          C_M_AXIS_DATA_WIDTH / DATA_WIDTH_OUT,
          C_S_AXIS_DATA_WIDTH / DATA_WIDTH_IN
      );
  end

  reg draining;

  assign s_axis_A_tready = m_axis_tready;
  assign s_axis_B_tready = m_axis_tready;
  // assign s_axis_A_tready = m_axis_tready && !draining;
  // assign s_axis_B_tready = m_axis_tready && !draining;

  localparam unsigned ELEMS_PER_WORD = C_S_AXIS_DATA_WIDTH / DATA_WIDTH_IN;
  localparam unsigned Cols = ((SIZE + ELEMS_PER_WORD) / ELEMS_PER_WORD) - 1;
  localparam unsigned ColsW = Cols > 1 ? $clog2(Cols) : 1;

  logic [ColsW - 1:0] col;
  logic tick;
  logic tick_delayed;
  logic [SIZE-1:0] current_row_oh;
  logic [SIZE-1:0] output_row_oh;

  wire a_valid = s_axis_A_tvalid;
  wire b_valid = s_axis_B_tvalid;
  wire input_valid = a_valid || b_valid;
  wire input_last = (!s_axis_A_tvalid || s_axis_A_tlast) && (s_axis_B_tlast || !s_axis_B_tvalid);

  logic input_valid_delayed;
  logic input_last_delayed;

  always @(posedge aclk) begin
    input_valid_delayed <= input_valid;
    input_last_delayed  <= input_last;
  end

  logic [DATA_WIDTH_IN-1:0] a_col[SIZE];
  logic [DATA_WIDTH_IN-1:0] b_row[SIZE];
  logic [DATA_WIDTH_IN-1:0] c_row_zero[SIZE] = '{default: '0};
  logic [DATA_WIDTH_OUT - 1:0] result_row[SIZE];

  wire data_in_flight = (output_row_oh != 0);


  wire output_valid = (data_in_flight && (input_valid_delayed));
  // wire output_valid = (data_in_flight && (input_valid_delayed || draining));

  assign m_axis_tvalid = output_valid;
  assign m_axis_tlast  = (output_row_oh[SIZE-1] && ((input_last_delayed && input_valid_delayed)));
  // && (draining || (input_last_delayed && input_valid_delayed)));

  counter #(
      .MAX(Cols)
  ) row_cntr (
      .clk(aclk),
      .count(col),
      .en(input_valid),
      // .en(draining || input_valid),
      .ending(tick),
      .rstn(aresetn)
  );

  logic [ColsW -1:0] col_delayed;
  always @(posedge aclk) begin
    col_delayed <= col;
  end

  genvar i;
  generate
    for (i = 0; i < ELEMS_PER_WORD; i++) begin : gen_out
      assign m_axis_tdata[i*DATA_WIDTH_OUT+:DATA_WIDTH_OUT] = result_row[col_delayed*ELEMS_PER_WORD+i];
    end
    for (i = 0; i < ELEMS_PER_WORD; i++) begin : gen_input
      always @(posedge aclk, negedge aresetn) begin
        if (!aresetn) begin
          a_col[col*ELEMS_PER_WORD+i] <= 0;
          b_row[col*ELEMS_PER_WORD+i] <= 0;
        end else begin
          if (a_valid) begin
            a_col[col*ELEMS_PER_WORD+i] <= s_axis_A_tdata[i*DATA_WIDTH_IN+:DATA_WIDTH_IN];
          end
          if (b_valid) begin
            b_row[col*ELEMS_PER_WORD+i] <= s_axis_B_tdata[i*DATA_WIDTH_IN+:DATA_WIDTH_IN];
          end
        end
      end
    end
  endgenerate

  always @(posedge aclk or negedge aresetn) begin
    if (~aresetn) begin
      current_row_oh <= 1;
      output_row_oh <= 0;
      draining <= 0;
    end else begin
      if (tick_delayed) begin
        current_row_oh <= {
          current_row_oh[$bits(current_row_oh)-2:0], current_row_oh[$bits(current_row_oh)-1]
        };
        output_row_oh <= {
          output_row_oh[$bits(output_row_oh)-2:0], current_row_oh[$bits(current_row_oh)-1]
        };
      end

      if (draining && output_row_oh[SIZE-1] && tick_delayed) begin
        draining <= 0;
      end else if (input_last_delayed && input_valid_delayed) begin
        draining <= 1;
      end
    end
  end

  logic load_a_delayed;
  logic load_b_delayed;

  always @(posedge aclk) begin
    tick_delayed   <= tick && (input_valid);
    // tick_delayed   <= tick && (draining || input_valid);
    load_a_delayed <= a_valid;
    load_b_delayed <= b_valid;
  end


  SA #(
      .SIZE(SIZE),
      .DATA_WIDTH_IN(DATA_WIDTH_IN),
      .DATA_WIDTH_OUT(DATA_WIDTH_OUT)
  ) sa_core (
      .clk  (aclk),
      .rst_n(aresetn),
      .tick (tick_delayed),

      .current_row(current_row_oh),
      .load_a(load_a_delayed),
      .load_b(load_b_delayed),

      .a_col(a_col),
      .b_row(b_row),
      .c_row(c_row_zero),

      .result_row(result_row)
  );

endmodule
