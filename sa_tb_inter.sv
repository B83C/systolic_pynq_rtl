`timescale 1ns / 1ps

module sa_tb;
  reg clk;
  reg rst_n;

  reg tick;
  reg load_a, load_b;

  localparam unsigned Size = 4;
  localparam unsigned DWI = 8;
  localparam unsigned DWO = 32;
  reg [Size - 1:0] load_row;

  reg [DWI-1:0] a_row[Size];
  reg [DWI-1:0] b_row[Size];
  reg [DWI-1:0] c_row[Size];
  // reg [DWI-1:0] c_row[Sie] = '{default: '0};

  wire [DWO-1:0] result_row[Size];

  SA #(
      .SIZE(Size),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO)
  ) sa_top (
      .clk  (clk),
      .rst_n(rst_n),
      .tick (tick),

      .current_row(load_row),
      .load_a(load_a),
      .load_b(load_b),
      .a_row(a_row),
      .b_row(b_row),
      .c_row(c_row),

      .result_row(result_row)
  );

  always #1 clk = ~clk;

  task load(logic [DWI-1:0] a_r[Size], logic [DWI-1:0] b_r[Size],
            logic [DWI-1:0] c_r[Size] = '{default: 0});
    tick = 1;
    load_row = {load_row[Size-2:0], load_row[Size-1]};
    if (load_row == 0) begin
      load_row = 1;
    end
    a_row = a_r;
    b_row = b_r;
    c_row = c_r;
    @(negedge clk);
    tick = 0;
    @(negedge clk);
  endtask

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_tb);

    rst_n = 0;
    repeat (2) @(negedge clk);
    rst_n = 1;
    repeat (2) @(negedge clk);

    load_a = 1;
    load_b = 1;

    load('{1, 8, 5, 6}, '{4, 3, 2, 1});
    load('{2, 8, 5, 7}, '{9, 9, 9, 9});
    load('{3, 8, 5, 8}, '{6, 6, 6, 6});
    load('{4, 8, 5, 9}, '{9, 8, 7, 6});

    load('{1, 8, 5, 6}, '{4, 3, 2, 1}, '{1, 1, 1, 1});
    load('{2, 8, 5, 7}, '{9, 9, 9, 9}, '{2, 2, 2, 2});
    load('{3, 8, 5, 8}, '{6, 6, 6, 6}, '{3, 3, 3, 3});
    load('{4, 8, 5, 9}, '{9, 8, 7, 6}, '{4, 4, 4, 4});

    load('{1, 8, 5, 6}, '{3, 2, 1, 0}, '{1, 1, 1, 1});
    load('{2, 8, 5, 7}, '{8, 8, 8, 8}, '{2, 2, 2, 2});
    load('{3, 8, 5, 8}, '{5, 5, 5, 5}, '{3, 3, 3, 3});
    load('{4, 8, 5, 9}, '{8, 7, 6, 5}, '{4, 4, 4, 4});

    load_b = 0;
    load('{1, 8, 5, 6}, '{3, 2, 1, 0}, '{1, 1, 1, 1});
    load('{2, 8, 5, 7}, '{8, 8, 8, 8}, '{2, 2, 2, 2});
    load('{3, 8, 5, 8}, '{5, 5, 5, 5}, '{3, 3, 3, 3});
    load('{4, 8, 5, 9}, '{8, 7, 6, 5}, '{4, 4, 4, 4});

    load('{1, 8, 5, 6}, '{4, 3, 2, 1});
    load('{2, 8, 5, 7}, '{9, 9, 9, 9});
    load('{3, 8, 5, 8}, '{6, 6, 6, 6});
    load('{4, 8, 5, 9}, '{9, 8, 7, 6});

    #500 $finish();

  end


endmodule
