`timescale 1ns / 1ps

module sa_tb;
  reg clk;
  reg rst_n;

  reg tick;

  localparam unsigned Size = 4;
  localparam unsigned DWI = 8;
  localparam unsigned DWO = 32;
  reg [Size - 1:0] load_row;

  reg [DWI-1:0] a_col[Size];
  reg [DWI-1:0] b_row[Size];
  reg [DWI-1:0] c_row[Size] = '{default: '0};

  wire [DWO-1:0] result_row[Size];

  SA #(
      .SIZE(Size),
      .DATA_WIDTH_IN(DWI),
      .DATA_WIDTH_OUT(DWO)
  ) sa_top (
      .clk  (clk),
      .rst_n(rst_n),
      .tick (tick),

      .load_row(load_row),
      .a_col(a_col),
      .b_row(b_row),
      .c_row(c_row),

      .result_row(result_row)
  );

  always #1 clk = ~clk;

  function automatic void load(logic [DWI-1:0] a_c[Size], logic [DWI-1:0] b_r[Size]);
    tick = 1;
    load_row = {load_row[Size-2:0], load_row[Size-1]};
    if (load_row == 0) begin
      load_row = 1;
    end
    a_col = a_c;
    b_row = b_r;
    @(negedge clk);
    tick = 0;
    @(negedge clk);
  endfunction

  initial begin
    $dumpfile("waveform.fst");
    $dumpvars(0, sa_tb);

    rst_n = 0;
    repeat (2) @(negedge clk);
    rst_n = 1;
    repeat (2) @(negedge clk);

    load('{1, 2, 3, 4}, '{4, 3, 2, 1});

    load('{8, 8, 8, 8}, '{9, 9, 9, 9});

    load('{5, 5, 5, 5}, '{6, 6, 6, 6});

    load('{6, 7, 8, 9}, '{9, 8, 7, 6});

    load('{1, 2, 3, 4}, '{4, 3, 2, 1});

    load('{8, 8, 8, 8}, '{9, 9, 9, 9});

    load('{5, 5, 5, 5}, '{6, 6, 6, 6});

    load('{6, 7, 8, 9}, '{9, 8, 7, 6});

    load('{1, 2, 3, 4}, '{4, 3, 2, 1});

    load('{8, 8, 8, 8}, '{9, 9, 9, 9});

    load('{5, 5, 5, 5}, '{6, 6, 6, 6});

    load('{6, 7, 8, 9}, '{9, 8, 7, 6});

    #500 $finish();

  end


endmodule
