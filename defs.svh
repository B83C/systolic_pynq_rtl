`ifndef HEADER
`define HEADER

typedef enum {
  LOAD_A,
  LOAD_B,
  LOAD_C
} state_t;

// AXI-Lite register offsets (7-bit addresses, 0x00..0x7F)
localparam REG_STATE = 7'h00;
localparam REG_STATUS = 7'h04;
localparam REG_C_LOAD = 7'h08;
localparam REG_FB_CNT = 7'h0C;
localparam REG_A_LOAD = 7'h10;
localparam REG_ACC_CNT = 7'h14;
localparam REG_A_LOOP_START = 7'h18;
localparam REG_A_LOOP_END = 7'h1C;
localparam REG_C_LOOP_START = 7'h20;
localparam REG_C_LOOP_END = 7'h24;
localparam REG_SIZE = 7'h28;
localparam REG_RST_INDEX = 7'h2C;
localparam REG_ZP_IN = 7'h3C;
localparam REG_A_RD_PTR = 7'h40;
localparam REG_C_RD_PTR = 7'h44;


`endif
