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
localparam REG_ACC_CNT      = 6'h14;
localparam REG_A_LOOP_START = 6'h18;
localparam REG_A_LOOP_END   = 6'h1C;
localparam REG_C_LOOP_START = 6'h20;
localparam REG_C_LOOP_END   = 6'h24;
localparam REG_SIZE         = 6'h28;
localparam REG_RST_INDEX    = 6'h2C;
localparam REG_MUL_Q        = 6'h30;
localparam REG_SHIFT        = 6'h34;
localparam REG_ZP_OUT       = 6'h38;
localparam REG_ZP_IN        = 6'h3C;
localparam REG_OUT_CH       = 6'h3D;
localparam REG_AXIS_BYPASS  = 6'h3E;
localparam REG_REPEAT_CNT   = 6'h3F;

`endif
