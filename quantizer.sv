`timescale 1ns / 1ps

// Quantizer:  q_out = clamp(((acc * mul_q) >>> shift) + zp_out, MIN, MAX)
// 3 pipeline stages: S0 = input reg, S1 = DSP+shift → reg, S2 = add_zp+clamp → reg
// AXI-Lite slave: REG_MUL_Q, REG_SHIFT, REG_ZP_OUT (3-bit address space)

module quantizer #(
    parameter unsigned SIZE           = 4,
    parameter unsigned DATA_WIDTH_IN  = 32,
    parameter unsigned DATA_WIDTH_OUT = 8,
    parameter unsigned ACCUM_WIDTH    = 32,
    parameter unsigned MAX_MUL_Q       = 65535,
    parameter unsigned MAX_SHIFT       = 31,
    parameter integer  MAX_ZP_OUT      = 127,
    parameter integer  MIN_ZP_OUT      = -128,
    localparam unsigned MUL_Q_W       = $clog2(MAX_MUL_Q + 1),
    localparam unsigned SHIFT_W       = $clog2(MAX_SHIFT + 1),
    localparam unsigned ZP_OUT_W      = $clog2(MAX_ZP_OUT - MIN_ZP_OUT + 1),
    localparam integer  ZP_OUT_CLAMP_HI = MAX_ZP_OUT,
    localparam integer  ZP_OUT_CLAMP_LO = MIN_ZP_OUT,
    localparam integer  ZP_OUT_CLAMP_W  = MAX_ZP_OUT - MIN_ZP_OUT + 1,
    localparam unsigned AXI_ADDR_W    = 4
) (
    input  logic clk,
    input  logic rst_n,

    // AXI4-Lite: config / status
    input  wire                  s_axil_awvalid,
    output wire                  s_axil_awready,
    input  wire [AXI_ADDR_W-1:0] s_axil_awaddr,
    input  wire [          31:0] s_axil_wdata,
    input  wire                  s_axil_wvalid,
    output wire                  s_axil_wready,
    output wire [           1:0] s_axil_bresp,
    output reg                   s_axil_bvalid,
    input  wire                  s_axil_bready,

    input  wire                  s_axil_arvalid,
    output wire                  s_axil_arready,
    input  wire [AXI_ADDR_W-1:0] s_axil_araddr,
    output reg  [          31:0] s_axil_rdata,
    output wire [           1:0] s_axil_rresp,
    output reg                   s_axil_rvalid,
    input  wire                  s_axil_rready,

    input  logic [SIZE*DATA_WIDTH_IN -1:0] s_axis_tdata,
    input  logic                           s_axis_tvalid,
    output logic                           s_axis_tready,
    input  logic                           s_axis_tlast,

    output logic [SIZE*DATA_WIDTH_OUT-1:0] m_axis_tdata,
    output logic                           m_axis_tvalid,
    input  logic                           m_axis_tready,
    output logic                           m_axis_tlast
);

    // AXI-Lite register offsets
    localparam REG_Q_MUL_Q  = 4'h0;
    localparam REG_Q_SHIFT  = 4'h4;
    localparam REG_Q_ZP_OUT = 4'h8;

    wire signed [ACCUM_WIDTH-1:0] acc[SIZE];
    generate
        for (genvar i = 0; i < SIZE; i++) begin : gen_acc_slice
            assign acc[i] = s_axis_tdata[i*DATA_WIDTH_IN+:DATA_WIDTH_IN];
        end
    endgenerate

    logic [MUL_Q_W-1:0]  mul_q;
    logic [SHIFT_W-1:0]  shift;
    logic [ZP_OUT_W-1:0] zp_out;

    wire axil_wr_en = s_axil_awvalid && s_axil_wvalid && !s_axil_bvalid;
    wire axil_rd_en = s_axil_arvalid && !s_axil_rvalid;

    assign s_axil_awready = axil_wr_en;
    assign s_axil_wready  = axil_wr_en;
    assign s_axil_bresp   = 2'b00;
    assign s_axil_arready = axil_rd_en;
    assign s_axil_rresp   = 2'b00;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            mul_q          <= 0;
            shift          <= 0;
            zp_out         <= 0;
            s_axil_bvalid  <= 0;
            s_axil_rvalid  <= 0;
            s_axil_rdata   <= 0;
        end else begin
            if (axil_wr_en) begin
                /* verilator lint_off CASEOVERLAP */
                case (s_axil_awaddr)
                    REG_Q_MUL_Q:  mul_q  <= s_axil_wdata[MUL_Q_W-1:0];
                    REG_Q_SHIFT:  shift  <= s_axil_wdata[SHIFT_W-1:0];
                    REG_Q_ZP_OUT: zp_out <= s_axil_wdata[ZP_OUT_W-1:0];
                    default: ;
                endcase
                /* verilator lint_on CASEOVERLAP */
                s_axil_bvalid <= 1;
            end else if (s_axil_bready) begin
                s_axil_bvalid <= 0;
            end

            if (axil_rd_en) begin
                /* verilator lint_off CASEOVERLAP */
                case (s_axil_araddr)
                    REG_Q_MUL_Q:  s_axil_rdata <= {{32-MUL_Q_W{1'b0}},  mul_q};
                    REG_Q_SHIFT:  s_axil_rdata <= {{32-SHIFT_W{1'b0}},  shift};
                    REG_Q_ZP_OUT: s_axil_rdata <= {{32-ZP_OUT_W{1'b0}}, zp_out};
                    default:      s_axil_rdata <= 0;
                endcase
                /* verilator lint_on CASEOVERLAP */
                s_axil_rvalid <= 1;
            end else if (s_axil_rready) begin
                s_axil_rvalid <= 0;
            end
        end
    end

    // -- stage 0 registers (raw accumulator input) --
    logic signed [ACCUM_WIDTH-1:0] acc_r[SIZE];
    logic s0_valid, s0_last;

    // -- stage 1 registers (after DSP+shift) --
    logic signed [ACCUM_WIDTH+MUL_Q_W-1:0] q_shifted_r[SIZE];
    logic s1_valid, s1_last;

    // -- stage 2 registers (after add_zp+clamp) --
    logic signed [MUL_Q_W-1:0] q_out_r[SIZE];
    logic s2_valid, s2_last;

    wire signed [ACCUM_WIDTH+MUL_Q_W-1:0] q_prod[SIZE];
    wire signed [ACCUM_WIDTH+MUL_Q_W-1:0] q_shifted[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_quant
            (* use_dsp = "yes" *)
            assign q_prod[qi]   = $signed(acc_r[qi]) * $signed({{(ACCUM_WIDTH-MUL_Q_W){1'b0}}, mul_q});
            assign q_shifted[qi] = $signed(q_prod[qi]) >>> shift;
        end
    endgenerate

    wire signed [MUL_Q_W-1:0] q_with_zp[SIZE];
    wire signed [MUL_Q_W-1:0] q_out[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_clamp
            assign q_with_zp[qi] = MUL_Q_W'(
                $signed(q_shifted_r[qi][MUL_Q_W-1:0])
                + $signed({zp_out[ZP_OUT_W-1], zp_out})
            );
            assign q_out[qi]     = (q_with_zp[qi] > MUL_Q_W'(ZP_OUT_CLAMP_HI)) ? MUL_Q_W'(ZP_OUT_CLAMP_HI) :
                                   (q_with_zp[qi] < MUL_Q_W'(ZP_OUT_CLAMP_LO)) ? MUL_Q_W'(ZP_OUT_CLAMP_LO) :
                                                                                q_with_zp[qi];
        end
    endgenerate

    wire stall = s2_valid && !m_axis_tready;
    wire pipe_shift = !stall;

    assign s_axis_tready = !stall;
    assign m_axis_tvalid = s2_valid;
    assign m_axis_tlast  = s2_valid && s2_last;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            acc_r       <= '{default: 0};
            q_shifted_r <= '{default: 0};
            q_out_r     <= '{default: 0};
            s0_valid <= 0; s0_last <= 0;
            s1_valid <= 0; s1_last <= 0;
            s2_valid <= 0; s2_last <= 0;
        end else if (pipe_shift) begin
            // stage 0: register raw input
            acc_r    <= acc;
            s0_valid <= s_axis_tvalid;
            s0_last  <= s_axis_tvalid && s_axis_tlast;

            // stage 1: register after DSP+shift
            q_shifted_r <= q_shifted;
            s1_valid <= s0_valid;
            s1_last  <= s0_last;

            // stage 2: register after add_zp+clamp
            q_out_r  <= q_out;
            s2_valid <= s1_valid;
            s2_last  <= s1_last;
        end
    end

    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_out
            assign m_axis_tdata[qi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT]
                 = {{(DATA_WIDTH_OUT - ZP_OUT_W){q_out_r[qi][ZP_OUT_W-1]}}, q_out_r[qi][ZP_OUT_W-1:0]};
        end
    endgenerate

endmodule
