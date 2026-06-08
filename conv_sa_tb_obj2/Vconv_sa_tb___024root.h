// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vconv_sa_tb.h for the primary calling header

#ifndef VERILATED_VCONV_SA_TB___024ROOT_H_
#define VERILATED_VCONV_SA_TB___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vconv_sa_tb__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vconv_sa_tb___024root final {
  public:

    // DESIGN SPECIFIC STATE
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ conv_sa_tb__DOT__clk;
        CData/*0:0*/ conv_sa_tb__DOT__rst_n;
        CData/*7:0*/ conv_sa_tb__DOT__pix_tdata;
        CData/*0:0*/ conv_sa_tb__DOT__pix_tvalid;
        CData/*0:0*/ conv_sa_tb__DOT__pix_tready;
        CData/*0:0*/ conv_sa_tb__DOT__pix_tlast;
        CData/*0:0*/ conv_sa_tb__DOT__pix_tuser;
        CData/*7:0*/ conv_sa_tb__DOT__kernel_tdata;
        CData/*0:0*/ conv_sa_tb__DOT__kernel_tvalid;
        CData/*0:0*/ conv_sa_tb__DOT__kernel_tlast;
        CData/*0:0*/ conv_sa_tb__DOT__res_tready;
        CData/*2:0*/ conv_sa_tb__DOT__dut__DOT__state;
        CData/*2:0*/ conv_sa_tb__DOT__dut__DOT__state_nxt;
        CData/*2:0*/ conv_sa_tb__DOT__dut__DOT__kernel_cnt;
        CData/*1:0*/ conv_sa_tb__DOT__dut__DOT__b_cycle;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__wr_row;
        CData/*1:0*/ conv_sa_tb__DOT__dut__DOT__wr_col;
        CData/*3:0*/ conv_sa_tb__DOT__dut__DOT__row_rx;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__in_frame;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__new_window;
        CData/*1:0*/ conv_sa_tb__DOT__dut__DOT__step;
        CData/*3:0*/ conv_sa_tb__DOT__dut__DOT__win_cnt;
        CData/*3:0*/ conv_sa_tb__DOT__dut__DOT__sa_cur_row;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__sa_load_a;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__sa_valid;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__out_stall;
        CData/*0:0*/ conv_sa_tb__DOT__dut__DOT__result_v;
        CData/*7:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
        CData/*0:0*/ __VdfgRegularize_h6e95ff9d_0_0;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __VstlPhaseResult;
        CData/*0:0*/ __Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__conv_sa_tb__DOT__rst_n__0;
        CData/*0:0*/ __VactPhaseResult;
        CData/*0:0*/ __VinactPhaseResult;
        CData/*0:0*/ __VnbaPhaseResult;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
        IData/*31:0*/ __VactIterCount;
        IData/*31:0*/ __VinactIterCount;
        IData/*31:0*/ __Vi;
        VlUnpacked<CData/*7:0*/, 4> conv_sa_tb__DOT__dut__DOT__kernel_reg;
        VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 2> conv_sa_tb__DOT__dut__DOT__line_buf;
        VlUnpacked<CData/*7:0*/, 4> conv_sa_tb__DOT__dut__DOT__window_cap;
        VlUnpacked<CData/*7:0*/, 4> conv_sa_tb__DOT__dut__DOT__sa_a_row;
        VlUnpacked<CData/*7:0*/, 4> conv_sa_tb__DOT__dut__DOT__sa_b_row;
        VlUnpacked<IData/*31:0*/, 4> conv_sa_tb__DOT__dut__DOT__sa_c_row;
        VlUnpacked<IData/*31:0*/, 4> conv_sa_tb__DOT__dut__DOT__sa_result_row;
    };
    struct {
        VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 4> conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner;
        VlUnpacked<VlUnpacked<CData/*7:0*/, 12>, 4> conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop;
        VlUnpacked<VlUnpacked<IData/*31:0*/, 4>, 5> conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggeredAcc;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
        VlUnpacked<CData/*0:0*/, 4> __Vm_traceActivity;
    };
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h822c2670__0;

    // INTERNAL VARIABLES
    Vconv_sa_tb__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vconv_sa_tb___024root(Vconv_sa_tb__Syms* symsp, const char* namep);
    ~Vconv_sa_tb___024root();
    VL_UNCOPYABLE(Vconv_sa_tb___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
