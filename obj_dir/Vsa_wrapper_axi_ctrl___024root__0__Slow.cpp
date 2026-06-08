// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsa_wrapper_axi_ctrl.h for the primary calling header

#include "Vsa_wrapper_axi_ctrl__pch.h"

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_static(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_static\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_initial__TOP(Vsa_wrapper_axi_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_initial(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_initial\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vsa_wrapper_axi_ctrl___024root___eval_initial__TOP(vlSelf);
}

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_initial__TOP(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_initial__TOP\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[3U] = 0U;
    vlSelfRef.s_axil_bresp = 0U;
    vlSelfRef.s_axil_rresp = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][0U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][1U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][2U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U] = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = 0U;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = 0U;
}

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_final(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_final\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vsa_wrapper_axi_ctrl___024root___eval_phase__stl(Vsa_wrapper_axi_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_settle(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_settle\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vsa_wrapper_axi_ctrl___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("sa_wrapper_axi_ctrl.sv", 3, "", "DIDNOTCONVERGE: Settle region did not converge after '--converge-limit' of 10000 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        vlSelfRef.__VstlPhaseResult = Vsa_wrapper_axi_ctrl___024root___eval_phase__stl(vlSelf);
        vlSelfRef.__VstlFirstIteration = 0U;
    } while (vlSelfRef.__VstlPhaseResult);
}

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__stl(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__stl\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
}

VL_ATTR_COLD bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vsa_wrapper_axi_ctrl___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___stl_sequent__TOP__0(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___stl_sequent__TOP__0\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__both_valid;
    sa_wrapper_axi_ctrl__DOT__both_valid = 0;
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__can_output;
    sa_wrapper_axi_ctrl__DOT__can_output = 0;
    // Body
    vlSelfRef.m_axis_tdata[0U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[0U];
    vlSelfRef.m_axis_tdata[1U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[1U];
    vlSelfRef.m_axis_tdata[2U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[2U];
    vlSelfRef.m_axis_tdata[3U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[3U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__output_available 
        = ((6U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
           | (4U <= (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__beat_cnt)));
    vlSelfRef.s_axil_arready = ((~ (IData)(vlSelfRef.s_axil_rvalid)) 
                                & (IData)(vlSelfRef.s_axil_arvalid));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[0U] = 
        (0x000000ffU & (vlSelfRef.s_axis_A_tdata & 
                        (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[1U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_A_tdata >> 8U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[2U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_A_tdata >> 0x00000010U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[3U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_A_tdata >> 0x00000018U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[0U] = 
        (0x000000ffU & (vlSelfRef.s_axis_B_tdata & 
                        (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[1U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_B_tdata >> 8U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[2U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_B_tdata >> 0x00000010U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[3U] = 
        (0x000000ffU & ((vlSelfRef.s_axis_B_tdata >> 0x00000018U) 
                        & (- (IData)((6U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))))));
    vlSelfRef.s_axil_awready = ((IData)(vlSelfRef.s_axil_awvalid) 
                                & ((~ (IData)(vlSelfRef.s_axil_bvalid)) 
                                   & (IData)(vlSelfRef.s_axil_wvalid)));
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = (1U 
                                                & (~ 
                                                   ((6U 
                                                     == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                                    | (5U 
                                                       == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum;
    sa_wrapper_axi_ctrl__DOT__both_valid = (((IData)(vlSelfRef.s_axis_B_tvalid) 
                                             | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)) 
                                            & ((IData)(vlSelfRef.s_axis_A_tvalid) 
                                               | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)));
    sa_wrapper_axi_ctrl__DOT__can_output = (1U & ((~ (IData)(vlSelfRef.m_axis_tvalid)) 
                                                  | (IData)(vlSelfRef.m_axis_tready)));
    vlSelfRef.s_axil_wready = vlSelfRef.s_axil_awready;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][0U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][1U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][2U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][3U];
    vlSelfRef.s_axis_A_tready = ((IData)(sa_wrapper_axi_ctrl__DOT__both_valid) 
                                 & ((IData)(sa_wrapper_axi_ctrl__DOT__can_output) 
                                    & ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1) 
                                       & ((0U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                          & ((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)) 
                                             | (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done)))))));
    vlSelfRef.s_axis_B_tready = ((IData)(sa_wrapper_axi_ctrl__DOT__both_valid) 
                                 & ((IData)(sa_wrapper_axi_ctrl__DOT__can_output) 
                                    & ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1) 
                                       & ((0U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                          & ((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)) 
                                             | (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done)))))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__state;
    if ((4U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
        if ((2U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            if ((1U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
                if ((1U & (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start)))) {
                    vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 0U;
                }
            } else if ((0U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_cnt))) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 7U;
            }
        } else if ((1U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 6U;
        } else if (sa_wrapper_axi_ctrl__DOT__can_output) {
            if ((((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a) 
                  & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)) 
                 & ((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__compute_cnt) 
                    >= (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__loop_len_a_q)))) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 5U;
            } else if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 5U;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
        if ((1U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
            } else if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
            }
        } else if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
        } else if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
        }
    } else if ((1U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
        if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
        } else if (((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done)) 
                    & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 2U;
        } else if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done) 
                    & (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done)))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 3U;
        } else if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done) 
                    & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt = 4U;
        }
    } else if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt 
            = (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a) 
                & (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)))
                ? 3U : (((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)) 
                         & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b))
                         ? 2U : 1U));
    }
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_active 
        = ((6U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
           & (IData)(sa_wrapper_axi_ctrl__DOT__can_output));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__consume = ((IData)(sa_wrapper_axi_ctrl__DOT__both_valid) 
                                                   & ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1) 
                                                      & ((IData)(sa_wrapper_axi_ctrl__DOT__can_output) 
                                                         & (0U 
                                                            != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)))));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid = ((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_active) 
                                                 | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__consume));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_a = ((
                                                   (6U 
                                                    == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                                   | ((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done)) 
                                                      | (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)))) 
                                                  & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid));
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_b = ((
                                                   (6U 
                                                    == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                                   | ((~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done)) 
                                                      | (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)))) 
                                                  & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid));
}

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___eval_stl(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_stl\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vsa_wrapper_axi_ctrl___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vsa_wrapper_axi_ctrl___024root___eval_phase__stl(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_phase__stl\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__stl(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsa_wrapper_axi_ctrl___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
    __VstlExecute = Vsa_wrapper_axi_ctrl___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vsa_wrapper_axi_ctrl___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(Vsa_wrapper_axi_ctrl___024root___trigger_anySet__ico(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vsa_wrapper_axi_ctrl___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(negedge rst_n)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___ctor_var_reset(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___ctor_var_reset\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst_n = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1638864771569018232ull);
    vlSelf->s_axis_A_tdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17781260350859062884ull);
    vlSelf->s_axis_A_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14940946007388297931ull);
    vlSelf->s_axis_A_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18265933941742247073ull);
    vlSelf->s_axis_A_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12369808444379646754ull);
    vlSelf->s_axis_B_tdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11283839095980026434ull);
    vlSelf->s_axis_B_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7203504858085649600ull);
    vlSelf->s_axis_B_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2024453030321390404ull);
    vlSelf->s_axis_B_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13557395602598198602ull);
    VL_SCOPED_RAND_RESET_W(128, vlSelf->m_axis_tdata, __VscopeHash, 8796747702141925029ull);
    vlSelf->m_axis_tvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11050073027672567459ull);
    vlSelf->m_axis_tready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5619517951140101778ull);
    vlSelf->m_axis_tlast = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3666489172703407315ull);
    vlSelf->s_axil_awvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5565460829454824723ull);
    vlSelf->s_axil_awready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16902639615143046160ull);
    vlSelf->s_axil_awaddr = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 13848725219975908421ull);
    vlSelf->s_axil_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1103157403480817957ull);
    vlSelf->s_axil_wvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7856252330729444323ull);
    vlSelf->s_axil_wready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10617832712156855404ull);
    vlSelf->s_axil_bresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 4619599597122007664ull);
    vlSelf->s_axil_bvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15275381318758420228ull);
    vlSelf->s_axil_bready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 341218635761165985ull);
    vlSelf->s_axil_arvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14086364826711030938ull);
    vlSelf->s_axil_arready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3912437489807458257ull);
    vlSelf->s_axil_araddr = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14530572992589720136ull);
    vlSelf->s_axil_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5485178797053085655ull);
    vlSelf->s_axil_rresp = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 7204267391845473835ull);
    vlSelf->s_axil_rvalid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13034476536657300307ull);
    vlSelf->s_axil_rready = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14687157439859267212ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__ctrl_start = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13675645228373148444ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__ctrl_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9631085325993605104ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__mode_reuse_a = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16212408165259519347ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__mode_reuse_b = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16236535930746250861ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__loop_len_a_q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 16708899082874747569ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__state = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 15972594527856826103ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__state_nxt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 8753539028072239645ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__a_load_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14923285876271860893ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__b_load_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9996115212122744137ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__beat_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 14818655556171407930ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__compute_cnt = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 9014782919699805966ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__drain_cnt = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 9815991649830491875ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__a_loading_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18441523471978136695ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__b_loading_done = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11216442523901201579ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__state_running = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15071146780679858041ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__tlast_seen = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14621051405676276846ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__cur_row_r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 8405073553597742802ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__consume = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14412798421257030709ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__drain_active = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7130502149086467754ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6369116665998060048ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__load_a = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11386712182739394015ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__load_b = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12706258660912105777ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sa_wrapper_axi_ctrl__DOT__a_row[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 523234491277353181ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sa_wrapper_axi_ctrl__DOT__b_row[__Vi0] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 17631693823317637153ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sa_wrapper_axi_ctrl__DOT__c_row[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7269094446174127781ull);
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        vlSelf->sa_wrapper_axi_ctrl__DOT__result_row[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15868421708840077187ull);
    }
    VL_SCOPED_RAND_RESET_W(128, vlSelf->sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg, __VscopeHash, 17223856288983702287ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__output_available = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5135748393119022913ull);
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2152037435920616201ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 4; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 634008676104165049ull);
        }
    }
    for (int __Vi0 = 0; __Vi0 < 5; ++__Vi0) {
        for (int __Vi1 = 0; __Vi1 < 4; ++__Vi1) {
            vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[__Vi0][__Vi1] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3424816567386638965ull);
        }
    }
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3485320150609995013ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3799147523387849201ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13880685107867942790ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4426253770043183187ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8436821802575830255ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16781536337384759474ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10347572617275568642ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 3716416046112766659ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15513544435914370705ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12527419718766117882ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15794026776298568300ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 787826391309570198ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10604323794118778735ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9372732916727996316ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1990640474526346662ull);
    vlSelf->sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 18264953148135267219ull);
    vlSelf->__VdfgRegularize_h6e95ff9d_0_1 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VicoTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__rst_n__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
