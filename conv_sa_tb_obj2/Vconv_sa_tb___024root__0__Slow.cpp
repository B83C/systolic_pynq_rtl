// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vconv_sa_tb.h for the primary calling header

#include "Vconv_sa_tb__pch.h"

void Vconv_sa_tb___024root___timing_ready(Vconv_sa_tb___024root* vlSelf);

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_static(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_static\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0 
        = vlSelfRef.conv_sa_tb__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__rst_n__0 
        = vlSelfRef.conv_sa_tb__DOT__rst_n;
    Vconv_sa_tb___024root___timing_ready(vlSelf);
    do {
        vlSelfRef.__VactTriggeredAcc[vlSelfRef.__Vi] 
            = vlSelfRef.__VactTriggered[vlSelfRef.__Vi];
        vlSelfRef.__Vi = ((IData)(1U) + vlSelfRef.__Vi);
    } while ((0U >= vlSelfRef.__Vi));
}

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_initial__TOP(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_initial__TOP\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][4U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][5U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][6U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][7U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][8U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][9U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][10U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][11U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][4U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][5U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][6U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][7U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][8U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][9U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][10U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][11U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][4U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][5U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][6U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][7U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][8U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][9U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][10U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][11U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][0U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][1U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][2U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][4U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][5U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][6U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][7U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][8U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][9U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][10U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][11U] = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
}

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_final(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_final\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vconv_sa_tb___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vconv_sa_tb___024root___eval_phase__stl(Vconv_sa_tb___024root* vlSelf);

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_settle(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_settle\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vconv_sa_tb___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("conv_sa_tb.sv", 3, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vconv_sa_tb___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_triggers_vec__stl(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_triggers_vec__stl\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vconv_sa_tb___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vconv_sa_tb___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vconv_sa_tb___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vconv_sa_tb___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___trigger_anySet__stl\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

extern const VlUnpacked<CData/*2:0*/, 1024> Vconv_sa_tb__ConstPool__TABLE_hc4f99468_0;

VL_ATTR_COLD void Vconv_sa_tb___024root___stl_sequent__TOP__0(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___stl_sequent__TOP__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row 
        = (0x0000000fU & ((- (IData)((2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) 
                          & ((IData)(1U) << (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle))));
    if (((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
         & (0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)))) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[0U] 
            = vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[0U];
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[1U] 
            = vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[1U];
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[2U] 
            = vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[2U];
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[3U] 
            = vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[3U];
    } else {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[0U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[1U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[2U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[3U] = 0U;
    }
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[0U] 
        = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_reg
           [vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle] 
           & (- (IData)(((2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                         & VL_GTS_III(32, 4U, (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle))))));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a 
        = ((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
           | (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0 = ((4U 
                                                 == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                                & (0U 
                                                   == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][0U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][1U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][2U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][3U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall 
        = ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0) 
           & ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__res_tready)) 
              & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v)));
    vlSelfRef.conv_sa_tb__DOT__pix_tready = ((1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                             | ((2U 
                                                 == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0) 
                                                   | (3U 
                                                      == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_valid = 
        ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
         & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window 
        = ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tvalid) 
           & ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready) 
              & ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame) 
                 & ((1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx)) 
                    & (1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col))))));
    __Vtableidx1 = ((((((IData)(vlSelfRef.conv_sa_tb__DOT__kernel_tvalid) 
                        << 4U) | ((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)) 
                                  << 3U)) | (((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window))) 
                     << 5U) | (((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall) 
                                << 4U) | ((VL_LTES_III(32, 9U, (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt)) 
                                           << 3U) | (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__state_nxt 
        = Vconv_sa_tb__ConstPool__TABLE_hc4f99468_0
        [__Vtableidx1];
}

VL_ATTR_COLD void Vconv_sa_tb___024root____Vm_traceActivitySetAll(Vconv_sa_tb___024root* vlSelf);

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_stl(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_stl\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vconv_sa_tb___024root___stl_sequent__TOP__0(vlSelf);
        Vconv_sa_tb___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

VL_ATTR_COLD bool Vconv_sa_tb___024root___eval_phase__stl(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_phase__stl\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vconv_sa_tb___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vconv_sa_tb___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vconv_sa_tb___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vconv_sa_tb___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vconv_sa_tb___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vconv_sa_tb___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vconv_sa_tb___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge conv_sa_tb.clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge conv_sa_tb.rst_n)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vconv_sa_tb___024root____Vm_traceActivitySetAll(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root____Vm_traceActivitySetAll\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    vlSelfRef.__Vm_traceActivity[3U] = 1U;
}

VL_ATTR_COLD void Vconv_sa_tb___024root___ctor_var_reset(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___ctor_var_reset\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->conv_sa_tb__DOT__clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3796818663737414986ull);
    vlSelf->conv_sa_tb__DOT__rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2521604019605759403ull);
    vlSelf->conv_sa_tb__DOT__pix_tdata = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8595111733380235886ull);
    vlSelf->conv_sa_tb__DOT__pix_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12521546421108979677ull);
    vlSelf->conv_sa_tb__DOT__pix_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9007591837004657643ull);
    vlSelf->conv_sa_tb__DOT__pix_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7115061356303148948ull);
    vlSelf->conv_sa_tb__DOT__pix_tuser = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1436308135410256341ull);
    vlSelf->conv_sa_tb__DOT__kernel_tdata = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7156123308999095184ull);
    vlSelf->conv_sa_tb__DOT__kernel_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17246252561167247013ull);
    vlSelf->conv_sa_tb__DOT__kernel_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6378478989510013681ull);
    vlSelf->conv_sa_tb__DOT__res_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5485273357343893769ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__state = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 2634440933638706627ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__state_nxt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15803038171719486428ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__kernel_reg[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 7213839479161969212ull);
    }
    vlSelf->conv_sa_tb__DOT__dut__DOT__kernel_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6062898577678238858ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__b_cycle = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 9893685274628962787ull);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->conv_sa_tb__DOT__dut__DOT__line_buf[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11921843927655147017ull);
        }
    }
    vlSelf->conv_sa_tb__DOT__dut__DOT__wr_row = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13808778017416914834ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__wr_col = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 5469024403450638629ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__row_rx = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5979067603875935584ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__in_frame = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10291602092971397275ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__window_cap[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 312258949525670619ull);
    }
    vlSelf->conv_sa_tb__DOT__dut__DOT__new_window = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5720346324953115650ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__step = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 2135136195056151945ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__win_cnt = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 649793053314332182ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__sa_a_row[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16727831324541178292ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__sa_b_row[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14437040099191424836ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__sa_c_row[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 180988192736919105ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->conv_sa_tb__DOT__dut__DOT__sa_result_row[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5878464238894846032ull);
    }
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_cur_row = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17542946619826238750ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_load_a = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15918965662797654799ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15060439272323403035ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__out_stall = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8942561099687068188ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__result_v = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2339388648727897823ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14209420940423348855ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 12; ++__Vi1) {
            vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 18207714014631862810ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12048117910297720845ull);
        }
    }
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7159792777792392233ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17717053250267075375ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 749601071489101709ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9559003256285611722ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12396088892933438447ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7964125347198644623ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8472133523465542637ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13277361860692532405ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3867610638486396828ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1695663407065887318ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7862254244936912665ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7904943489296668391ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8435666072552300725ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10596911591300930234ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18327116214416496058ull);
    vlSelf->conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11833692951571541751ull);
    vlSelf->__VdfgRegularize_h6e95ff9d_0_0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggeredAcc[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__conv_sa_tb__DOT__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
    vlSelf->__Vi = 0;
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
