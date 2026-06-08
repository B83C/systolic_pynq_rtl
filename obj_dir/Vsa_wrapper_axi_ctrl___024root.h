// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vsa_wrapper_axi_ctrl.h for the primary calling header

#ifndef VERILATED_VSA_WRAPPER_AXI_CTRL___024ROOT_H_
#define VERILATED_VSA_WRAPPER_AXI_CTRL___024ROOT_H_  // guard

#include "verilated.h"


class Vsa_wrapper_axi_ctrl__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vsa_wrapper_axi_ctrl___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        VL_IN8(clk,0,0);
        VL_IN8(rst_n,0,0);
        VL_IN8(s_axis_A_tvalid,0,0);
        VL_OUT8(s_axis_A_tready,0,0);
        VL_IN8(s_axis_A_tlast,0,0);
        VL_IN8(s_axis_B_tvalid,0,0);
        VL_OUT8(s_axis_B_tready,0,0);
        VL_IN8(s_axis_B_tlast,0,0);
        VL_OUT8(m_axis_tvalid,0,0);
        VL_IN8(m_axis_tready,0,0);
        VL_OUT8(m_axis_tlast,0,0);
        VL_IN8(s_axil_awvalid,0,0);
        VL_OUT8(s_axil_awready,0,0);
        VL_IN8(s_axil_awaddr,3,0);
        VL_IN8(s_axil_wvalid,0,0);
        VL_OUT8(s_axil_wready,0,0);
        VL_OUT8(s_axil_bresp,1,0);
        VL_OUT8(s_axil_bvalid,0,0);
        VL_IN8(s_axil_bready,0,0);
        VL_IN8(s_axil_arvalid,0,0);
        VL_OUT8(s_axil_arready,0,0);
        VL_IN8(s_axil_araddr,3,0);
        VL_OUT8(s_axil_rresp,1,0);
        VL_OUT8(s_axil_rvalid,0,0);
        VL_IN8(s_axil_rready,0,0);
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__ctrl_start;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__ctrl_done;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__mode_reuse_a;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__mode_reuse_b;
        CData/*7:0*/ sa_wrapper_axi_ctrl__DOT__loop_len_a_q;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__state;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__state_nxt;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__a_load_cnt;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__b_load_cnt;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__beat_cnt;
        CData/*7:0*/ sa_wrapper_axi_ctrl__DOT__compute_cnt;
        CData/*2:0*/ sa_wrapper_axi_ctrl__DOT__drain_cnt;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__a_loading_done;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__b_loading_done;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__state_running;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__tlast_seen;
        CData/*3:0*/ sa_wrapper_axi_ctrl__DOT__cur_row_r;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__consume;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__drain_active;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__valid;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__load_a;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__load_b;
        CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__output_available;
        CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_1;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __VicoFirstIteration;
        CData/*0:0*/ __VicoPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__rst_n__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        VL_IN(s_axis_A_tdata,31,0);
        VL_IN(s_axis_B_tdata,31,0);
        VL_OUTW(m_axis_tdata,127,0,4);
        VL_IN(s_axil_wdata,31,0);
        VL_OUT(s_axil_rdata,31,0);
        VlWide<4>/*127:0*/ sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    };
    struct {
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<CData/*7:0*/, 4> sa_wrapper_axi_ctrl__DOT__a_row;
        VlUnpacked<CData/*7:0*/, 4> sa_wrapper_axi_ctrl__DOT__b_row;
        VlUnpacked<IData/*31:0*/, 4> sa_wrapper_axi_ctrl__DOT__c_row;
        VlUnpacked<IData/*31:0*/, 4> sa_wrapper_axi_ctrl__DOT__result_row;
        VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 4> sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner;
        VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 4> sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop;
        VlUnpacked<VlUnpacked<IData/*31:0*/, 4>, 5> sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VicoTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };

    // INTERNAL VARIABLES
    Vsa_wrapper_axi_ctrl__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vsa_wrapper_axi_ctrl___024root(Vsa_wrapper_axi_ctrl__Syms* symsp, const char* namep);
    ~Vsa_wrapper_axi_ctrl___024root();
    VL_UNCOPYABLE(Vsa_wrapper_axi_ctrl___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
