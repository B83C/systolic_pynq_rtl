`ifndef HEADER
`define HEADER

typedef enum {
  IDLE,
  LOAD_A,
  LOAD_B,
  LOAD_C
} state_t;

// AXI-Lite register offsets (6-bit addresses)
localparam REG_STATE        = 6'h00;
localparam REG_STATUS       = 6'h04;
localparam REG_C_LOAD       = 6'h08;
localparam REG_FB_CNT       = 6'h0C;
localparam REG_A_LOAD       = 6'h10;
localparam REG_ACC_OUT      = 6'h14;
localparam REG_A_LOOP_START = 6'h18;
localparam REG_A_LOOP_END   = 6'h1C;
localparam REG_C_LOOP_START = 6'h20;
localparam REG_C_LOOP_END   = 6'h24;
localparam REG_RST_INDEX    = 6'h2C;

`endif
