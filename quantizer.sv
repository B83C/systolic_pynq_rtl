`timescale 1ns / 1ps

// Quantizer:  q_out = clamp(((acc * mul_q) >>> shift) + zp_out, MIN, MAX)
// 4 pipeline stages: S0 = input reg, S1 = DSP+window → reg,
//                    S2 = barrel → reg, S3 = add_zp+clamp → reg
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

    // 16-way mux: select q_prod[15+shift_lo : shift_lo] from a 32-bit window.
    // This avoids the sign-extension bug of `>>>` on truncated data: we pick
    // the exact 16-bit slice from q_prod based on the manual shift lower bits.
    function automatic logic signed [MUL_Q_W-1:0] barrel_16(
        input logic signed [31:0] w,
        input logic [3:0] sh
    );
        case (sh)
            4'd0:  barrel_16 = w[15:0];
            4'd1:  barrel_16 = w[16:1];
            4'd2:  barrel_16 = w[17:2];
            4'd3:  barrel_16 = w[18:3];
            4'd4:  barrel_16 = w[19:4];
            4'd5:  barrel_16 = w[20:5];
            4'd6:  barrel_16 = w[21:6];
            4'd7:  barrel_16 = w[22:7];
            4'd8:  barrel_16 = w[23:8];
            4'd9:  barrel_16 = w[24:9];
            4'd10: barrel_16 = w[25:10];
            4'd11: barrel_16 = w[26:11];
            4'd12: barrel_16 = w[27:12];
            4'd13: barrel_16 = w[28:13];
            4'd14: barrel_16 = w[29:14];
            4'd15: barrel_16 = w[30:15];
        endcase
    endfunction

    // -- stage 0 registers (raw accumulator input) --
    logic signed [ACCUM_WIDTH-1:0] acc_r[SIZE];
    logic s0_valid, s0_last;

    // -- stage 1 registers (after DSP + 32-bit window mux) --
    logic signed [31:0] q_prod_window_r[SIZE];
    logic s1_valid, s1_last;

    // -- stage 2 registers (after barrel_16 mux) --
    logic signed [MUL_Q_W-1:0] q_shifted_r[SIZE];
    logic s2_valid, s2_last;

    // -- stage 3 registers (after add_zp+clamp) --
    logic signed [MUL_Q_W-1:0] q_out_r[SIZE];
    logic s3_valid, s3_last;

    wire signed [ACCUM_WIDTH+MUL_Q_W-1:0] q_prod[SIZE];
    wire signed [31:0]                     q_prod_window[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_dsp
            (* use_dsp = "yes" *)
            assign q_prod[qi] = $signed(acc_r[qi]) * $signed({{(ACCUM_WIDTH-MUL_Q_W){1'b0}}, mul_q});
            // Stage-1 mux: extract a 32-bit window of q_prod.
            //   shift[4]=0 → q_prod[30:0]  (covers shift lower 0..15)
            //   shift[4]=1 → q_prod[46:16] (covers shift lower 0..15 after +16)
            assign q_prod_window[qi] = shift[4] ? q_prod[qi][47:16] : q_prod[qi][31:0];
        end
    endgenerate

    wire signed [MUL_Q_W-1:0] q_shifted[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_shift
            // Stage-2: barrel_16 picks q_prod[15+shift[3:0] : shift[3:0]]
            // from the registered window (q_prod_window_r already accounts for shift[4] offset).
            // No sign-extension issue: this is a raw mux, not an arithmetic shift.
            assign q_shifted[qi] = barrel_16(q_prod_window_r[qi], shift[3:0]);
        end
    endgenerate

    wire signed [MUL_Q_W-1:0] q_with_zp[SIZE];
    wire signed [MUL_Q_W-1:0] q_out[SIZE];
    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_clamp
            assign q_with_zp[qi] = MUL_Q_W'(
                $signed(q_shifted_r[qi])
                + $signed({zp_out[ZP_OUT_W-1], zp_out})
            );
            assign q_out[qi]     = (q_with_zp[qi] > MUL_Q_W'(ZP_OUT_CLAMP_HI)) ? MUL_Q_W'(ZP_OUT_CLAMP_HI) :
                                   (q_with_zp[qi] < MUL_Q_W'(ZP_OUT_CLAMP_LO)) ? MUL_Q_W'(ZP_OUT_CLAMP_LO) :
                                                                                q_with_zp[qi];
        end
    endgenerate

    wire stall = s3_valid && !m_axis_tready;
    wire pipe_shift = !stall;

    assign s_axis_tready = !stall;
    assign m_axis_tvalid = s3_valid;
    assign m_axis_tlast  = s3_valid && s3_last;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            acc_r           <= '{default: 0};
            q_prod_window_r <= '{default: 0};
            q_shifted_r     <= '{default: 0};
            q_out_r         <= '{default: 0};
            s0_valid <= 0; s0_last <= 0;
            s1_valid <= 0; s1_last <= 0;
            s2_valid <= 0; s2_last <= 0;
            s3_valid <= 0; s3_last <= 0;
        end else if (pipe_shift) begin
            // S0: register raw input
            acc_r    <= acc;
            s0_valid <= s_axis_tvalid;
            s0_last  <= s_axis_tvalid && s_axis_tlast;

            // S1: register after DSP + 32-bit window mux
            q_prod_window_r <= q_prod_window;
            s1_valid <= s0_valid;
            s1_last  <= s0_last;

            // S2: register after barrel_16 mux
            q_shifted_r <= q_shifted;
            s2_valid <= s1_valid;
            s2_last  <= s1_last;

            // S3: register after add_zp+clamp
            q_out_r  <= q_out;
            s3_valid <= s2_valid;
            s3_last  <= s2_last;
        end
    end

    generate
        for (genvar qi = 0; qi < SIZE; qi++) begin : gen_out
            assign m_axis_tdata[qi*DATA_WIDTH_OUT+:DATA_WIDTH_OUT]
                 = {{(DATA_WIDTH_OUT - ZP_OUT_W){q_out_r[qi][ZP_OUT_W-1]}}, q_out_r[qi][ZP_OUT_W-1:0]};
        end
    endgenerate

endmodule
