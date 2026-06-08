// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsa_wrapper_axi_ctrl.h for the primary calling header

#include "Vsa_wrapper_axi_ctrl__pch.h"

void Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__ico(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__ico\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VicoTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VicoTriggered[0U]) 
                                     | (IData)((IData)(vlSelfRef.__VicoFirstIteration)));
}

bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__ico(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___trigger_anySet__ico\n"); );
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

void Vsa_wrapper_axi_ctrl___024root___ico_sequent__TOP__0(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___ico_sequent__TOP__0\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__both_valid;
    sa_wrapper_axi_ctrl__DOT__both_valid = 0;
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__can_output;
    sa_wrapper_axi_ctrl__DOT__can_output = 0;
    // Body
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
    sa_wrapper_axi_ctrl__DOT__both_valid = (((IData)(vlSelfRef.s_axis_B_tvalid) 
                                             | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)) 
                                            & ((IData)(vlSelfRef.s_axis_A_tvalid) 
                                               | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)));
    sa_wrapper_axi_ctrl__DOT__can_output = (1U & ((~ (IData)(vlSelfRef.m_axis_tvalid)) 
                                                  | (IData)(vlSelfRef.m_axis_tready)));
    vlSelfRef.s_axil_wready = vlSelfRef.s_axil_awready;
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

void Vsa_wrapper_axi_ctrl___024root___eval_ico(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_ico\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VicoTriggered[0U])) {
        Vsa_wrapper_axi_ctrl___024root___ico_sequent__TOP__0(vlSelf);
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__ico(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vsa_wrapper_axi_ctrl___024root___eval_phase__ico(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_phase__ico\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VicoExecute;
    // Body
    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__ico(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsa_wrapper_axi_ctrl___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
    }
#endif
    __VicoExecute = Vsa_wrapper_axi_ctrl___024root___trigger_anySet__ico(vlSelfRef.__VicoTriggered);
    if (__VicoExecute) {
        Vsa_wrapper_axi_ctrl___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__act(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__act\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((((~ (IData)(vlSelfRef.rst_n)) 
                                                       & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0)) 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst_n__0 = vlSelfRef.rst_n;
}

bool Vsa_wrapper_axi_ctrl___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___trigger_anySet__act\n"); );
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

void Vsa_wrapper_axi_ctrl___024root___nba_sequent__TOP__0(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___nba_sequent__TOP__0\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__both_valid;
    sa_wrapper_axi_ctrl__DOT__both_valid = 0;
    CData/*0:0*/ sa_wrapper_axi_ctrl__DOT__can_output;
    sa_wrapper_axi_ctrl__DOT__can_output = 0;
    CData/*3:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r;
    __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r = 0;
    CData/*2:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt = 0;
    CData/*0:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done;
    __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done = 0;
    CData/*2:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt = 0;
    CData/*0:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done;
    __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done = 0;
    CData/*2:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt = 0;
    CData/*7:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt = 0;
    CData/*2:0*/ __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt = 0;
    CData/*0:0*/ __Vdly__m_axis_tvalid;
    __Vdly__m_axis_tvalid = 0;
    IData/*31:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0 = 0;
    IData/*31:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v1;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v1 = 0;
    IData/*31:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v2;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v2 = 0;
    IData/*31:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v3;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v3 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v1;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v1 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v2;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v2 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v3;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v3 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v6;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v6 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v7;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v7 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v8;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v8 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v11;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v11 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v12;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v12 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v13;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v13 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v16;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v16 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v17;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v17 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v18;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v18 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v20;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v20 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v1;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v1 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v2;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v2 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v3;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v3 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v5;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v5 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v6;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v6 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v7;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v7 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v9;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v9 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v10;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v10 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v11;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v11 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v13;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v13 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v14;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v14 = 0;
    CData/*7:0*/ __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v15;
    __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v15 = 0;
    CData/*0:0*/ __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v16;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v16 = 0;
    // Body
    __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r = vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v16 = 0U;
    __Vdly__m_axis_tvalid = vlSelfRef.m_axis_tvalid;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19 = 0U;
    __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v20 = 0U;
    __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_load_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_load_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__beat_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__compute_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt = vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_cnt;
    __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done;
    __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done;
    if (vlSelfRef.rst_n) {
        if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r 
                = ((0x0000000eU & ((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                                   << 1U)) | (1U & 
                                              ((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                                               >> 3U)));
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[0U];
            __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0 = 1U;
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v1 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[1U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v2 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[2U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v3 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__c_row[3U];
            if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                 & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_b))) {
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[0U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v1 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[1U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v2 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[2U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v3 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[3U];
            }
            if ((((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                  >> 1U) & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_b))) {
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[0U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v5 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[1U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v6 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[2U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v7 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[3U];
            }
            if ((((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                  >> 2U) & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_b))) {
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[0U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v9 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[1U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v10 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[2U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v11 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[3U];
            }
            if ((((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r) 
                  >> 3U) & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_b))) {
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[0U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v13 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[1U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v14 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[2U];
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v15 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_row[3U];
            }
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U];
            __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0 = 1U;
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v1 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][1U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v2 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][2U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v3 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][3U];
            if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__load_a) {
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[0U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[1U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[2U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14 = 1U;
                __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19 
                    = vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_row[3U];
                __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19 = 1U;
            }
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][0U];
            __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5 = 1U;
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v6 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v7 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][2U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v8 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][3U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][0U];
            __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10 = 1U;
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v11 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][1U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v12 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v13 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][3U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][0U];
            __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15 = 1U;
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v16 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][1U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v17 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][2U];
            __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v18 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U];
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][0U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][0U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][1U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][1U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][2U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][2U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][3U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][3U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][0U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][0U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][1U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][1U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][2U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][2U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[1U][3U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][3U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][0U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][0U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][1U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][1U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][2U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][2U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[2U][3U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][3U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][0U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][0U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][1U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][1U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][2U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][2U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[3U][3U] 
                   + (vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][3U] 
                      * vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
        }
        if (vlSelfRef.s_axil_arready) {
            vlSelfRef.s_axil_rvalid = 1U;
            vlSelfRef.s_axil_rdata = ((0U == (IData)(vlSelfRef.s_axil_araddr))
                                       ? (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start) 
                                           << 2U) | 
                                          (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done) 
                                            << 1U) 
                                           | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_running)))
                                       : ((4U == (IData)(vlSelfRef.s_axil_araddr))
                                           ? (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b) 
                                               << 3U) 
                                              | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a))
                                           : ((8U == (IData)(vlSelfRef.s_axil_araddr))
                                               ? (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__loop_len_a_q)
                                               : 0U)));
        } else if (vlSelfRef.s_axil_rready) {
            vlSelfRef.s_axil_rvalid = 0U;
        }
        if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__output_available) 
             & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[0U] 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[0U];
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[1U] 
                = (IData)((((QData)((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[2U])) 
                            << 0x00000020U) | (QData)((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[1U]))));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[2U] 
                = (IData)(((((QData)((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[2U])) 
                             << 0x00000020U) | (QData)((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[1U]))) 
                           >> 0x00000020U));
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[3U] 
                = vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[3U];
            __Vdly__m_axis_tvalid = 1U;
            vlSelfRef.m_axis_tlast = ((6U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                      & (1U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_cnt)));
        } else if (((IData)(vlSelfRef.m_axis_tvalid) 
                    & (IData)(vlSelfRef.m_axis_tready))) {
            __Vdly__m_axis_tvalid = 0U;
            vlSelfRef.m_axis_tlast = 0U;
        }
        if (vlSelfRef.s_axil_awready) {
            vlSelfRef.s_axil_bvalid = 1U;
            if ((0U == (IData)(vlSelfRef.s_axil_awaddr))) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start 
                    = (1U & vlSelfRef.s_axil_wdata);
            }
        } else if (vlSelfRef.s_axil_bready) {
            vlSelfRef.s_axil_bvalid = 0U;
        }
        if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start = 0U;
        }
        if ((7U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done = 1U;
        }
        if ((0U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt = 0U;
            __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done = 0U;
        } else if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid) 
                    & (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done)))) {
            if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_load_cnt) 
                 == ((7U & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__loop_len_a_q)) 
                     - (IData)(1U)))) {
                __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_load_cnt)));
                __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done = 1U;
            } else {
                __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_load_cnt)));
            }
        }
        if (vlSelfRef.s_axil_awready) {
            if ((0U == (IData)(vlSelfRef.s_axil_awaddr))) {
                if ((1U & vlSelfRef.s_axil_wdata)) {
                    vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done = 0U;
                }
            }
            if ((0U != (IData)(vlSelfRef.s_axil_awaddr))) {
                if ((4U == (IData)(vlSelfRef.s_axil_awaddr))) {
                    vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b 
                        = (1U & (vlSelfRef.s_axil_wdata 
                                 >> 2U));
                    vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a 
                        = (1U & vlSelfRef.s_axil_wdata);
                }
                if ((4U != (IData)(vlSelfRef.s_axil_awaddr))) {
                    if ((8U == (IData)(vlSelfRef.s_axil_awaddr))) {
                        if (((1U <= (0x000000ffU & vlSelfRef.s_axil_wdata)) 
                             & (0x0cU >= (0x000000ffU 
                                          & vlSelfRef.s_axil_wdata)))) {
                            vlSelfRef.sa_wrapper_axi_ctrl__DOT__loop_len_a_q 
                                = (0x000000ffU & vlSelfRef.s_axil_wdata);
                        }
                    }
                }
            }
        }
        if ((0U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt = 0U;
            __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done = 0U;
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_running = 0U;
        } else {
            if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__valid) 
                 & (~ (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done)))) {
                if ((3U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_load_cnt))) {
                    __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt 
                        = (7U & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_load_cnt)));
                    __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done = 1U;
                } else {
                    __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt 
                        = (7U & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_load_cnt)));
                }
            }
            if ((7U != (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
                vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_running = 1U;
            }
        }
        if (((0U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
             | (7U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)))) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt = 0U;
            __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt = 0U;
        } else {
            if (vlSelfRef.sa_wrapper_axi_ctrl__DOT__consume) {
                __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__beat_cnt)));
            }
            if (((4U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                 & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__consume))) {
                __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt 
                    = (0x000000ffU & ((IData)(1U) + (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__compute_cnt)));
            }
        }
        if ((5U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state))) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt = 4U;
        } else if (((6U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                    & (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_active))) {
            __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt 
                = (7U & ((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_cnt) 
                         - (IData)(1U)));
        }
        if (((5U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
             | (0U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen = 0U;
        } else if (((IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__consume) 
                    & ((IData)(vlSelfRef.s_axis_A_tlast) 
                       | (IData)(vlSelfRef.s_axis_B_tlast)))) {
            vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen = 1U;
        }
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__state = vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_nxt;
    } else {
        __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r = 1U;
        __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v16 = 1U;
        __VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v20 = 1U;
        vlSelfRef.s_axil_rvalid = 0U;
        vlSelfRef.s_axil_bvalid = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[0U] = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[1U] = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[2U] = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[3U] = 0U;
        __Vdly__m_axis_tvalid = 0U;
        vlSelfRef.m_axis_tlast = 0U;
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
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_start = 0U;
        vlSelfRef.s_axil_rdata = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__ctrl_done = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done = 0U;
        __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__state_running = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__tlast_seen = 0U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__loop_len_a_q = 4U;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__state = 0U;
    }
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__cur_row_r = __Vdly__sa_wrapper_axi_ctrl__DOT__cur_row_r;
    vlSelfRef.m_axis_tvalid = __Vdly__m_axis_tvalid;
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v0;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v1;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v2;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[0U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v3;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v4;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v5;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v6;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[1U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v7;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v8;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v9;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v10;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[2U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v11;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v12;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v13;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v14;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner[3U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v15;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__b_inner__v16) {
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
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v0;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v1;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v2;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v3;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[0U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v4;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v5;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v6;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v7;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v8;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[1U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v9;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v10;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v11;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v12;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v13;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[2U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v14;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v15;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v16;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v17;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v18;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop[3U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v19;
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__a_inner_loop__v20) {
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
    }
    if (__VdlySet__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0) {
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][0U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v0;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][1U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v1;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][2U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v2;
        vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[0U][3U] 
            = __VdlyVal__sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum__v3;
    }
    vlSelfRef.s_axil_arready = ((~ (IData)(vlSelfRef.s_axil_rvalid)) 
                                & (IData)(vlSelfRef.s_axil_arvalid));
    vlSelfRef.m_axis_tdata[0U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[0U];
    vlSelfRef.m_axis_tdata[1U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[1U];
    vlSelfRef.m_axis_tdata[2U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[2U];
    vlSelfRef.m_axis_tdata[3U] = vlSelfRef.sa_wrapper_axi_ctrl__DOT__m_axis_tdata_reg[3U];
    sa_wrapper_axi_ctrl__DOT__can_output = (1U & ((~ (IData)(vlSelfRef.m_axis_tvalid)) 
                                                  | (IData)(vlSelfRef.m_axis_tready)));
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
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[0U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][0U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[1U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][1U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[2U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][2U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__result_row[3U] 
        = vlSelfRef.sa_wrapper_axi_ctrl__DOT__sa_core__DOT__partial_sum[4U][3U];
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_load_cnt 
        = __Vdly__sa_wrapper_axi_ctrl__DOT__a_load_cnt;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_load_cnt 
        = __Vdly__sa_wrapper_axi_ctrl__DOT__b_load_cnt;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__beat_cnt = __Vdly__sa_wrapper_axi_ctrl__DOT__beat_cnt;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__compute_cnt 
        = __Vdly__sa_wrapper_axi_ctrl__DOT__compute_cnt;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__drain_cnt = __Vdly__sa_wrapper_axi_ctrl__DOT__drain_cnt;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__a_loading_done 
        = __Vdly__sa_wrapper_axi_ctrl__DOT__a_loading_done;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__b_loading_done 
        = __Vdly__sa_wrapper_axi_ctrl__DOT__b_loading_done;
    sa_wrapper_axi_ctrl__DOT__both_valid = (((IData)(vlSelfRef.s_axis_B_tvalid) 
                                             | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_b)) 
                                            & ((IData)(vlSelfRef.s_axis_A_tvalid) 
                                               | (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__mode_reuse_a)));
    vlSelfRef.s_axil_awready = ((IData)(vlSelfRef.s_axil_awvalid) 
                                & ((~ (IData)(vlSelfRef.s_axil_bvalid)) 
                                   & (IData)(vlSelfRef.s_axil_wvalid)));
    vlSelfRef.s_axil_wready = vlSelfRef.s_axil_awready;
    vlSelfRef.sa_wrapper_axi_ctrl__DOT__output_available 
        = ((6U == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
           | (4U <= (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__beat_cnt)));
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
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_1 = (1U 
                                                & (~ 
                                                   ((6U 
                                                     == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)) 
                                                    | (5U 
                                                       == (IData)(vlSelfRef.sa_wrapper_axi_ctrl__DOT__state)))));
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

void Vsa_wrapper_axi_ctrl___024root___eval_nba(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_nba\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vsa_wrapper_axi_ctrl___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vsa_wrapper_axi_ctrl___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vsa_wrapper_axi_ctrl___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vsa_wrapper_axi_ctrl___024root___eval_phase__act(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_phase__act\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vsa_wrapper_axi_ctrl___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vsa_wrapper_axi_ctrl___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vsa_wrapper_axi_ctrl___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vsa_wrapper_axi_ctrl___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vsa_wrapper_axi_ctrl___024root___eval_phase__nba(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_phase__nba\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vsa_wrapper_axi_ctrl___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vsa_wrapper_axi_ctrl___024root___eval_nba(vlSelf);
        Vsa_wrapper_axi_ctrl___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vsa_wrapper_axi_ctrl___024root___eval(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VicoIterCount = 0U;
    vlSelfRef.__VicoFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VicoIterCount)))) {
#ifdef VL_DEBUG
            Vsa_wrapper_axi_ctrl___024root___dump_triggers__ico(vlSelfRef.__VicoTriggered, "ico"s);
#endif
            VL_FATAL_MT("sa_wrapper_axi_ctrl.sv", 3, "", "DIDNOTCONVERGE: Input combinational region did not converge after '--converge-limit' of 10000 tries");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        vlSelfRef.__VicoPhaseResult = Vsa_wrapper_axi_ctrl___024root___eval_phase__ico(vlSelf);
        vlSelfRef.__VicoFirstIteration = 0U;
    } while (vlSelfRef.__VicoPhaseResult);
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vsa_wrapper_axi_ctrl___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("sa_wrapper_axi_ctrl.sv", 3, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vsa_wrapper_axi_ctrl___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("sa_wrapper_axi_ctrl.sv", 3, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vsa_wrapper_axi_ctrl___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vsa_wrapper_axi_ctrl___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vsa_wrapper_axi_ctrl___024root___eval_debug_assertions(Vsa_wrapper_axi_ctrl___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vsa_wrapper_axi_ctrl___024root___eval_debug_assertions\n"); );
    Vsa_wrapper_axi_ctrl__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.rst_n & 0xfeU)))) {
        Verilated::overWidthError("rst_n");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axis_A_tvalid & 0xfeU)))) {
        Verilated::overWidthError("s_axis_A_tvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axis_A_tlast & 0xfeU)))) {
        Verilated::overWidthError("s_axis_A_tlast");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axis_B_tvalid & 0xfeU)))) {
        Verilated::overWidthError("s_axis_B_tvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axis_B_tlast & 0xfeU)))) {
        Verilated::overWidthError("s_axis_B_tlast");
    }
    if (VL_UNLIKELY(((vlSelfRef.m_axis_tready & 0xfeU)))) {
        Verilated::overWidthError("m_axis_tready");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_awvalid & 0xfeU)))) {
        Verilated::overWidthError("s_axil_awvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_awaddr & 0xf0U)))) {
        Verilated::overWidthError("s_axil_awaddr");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_wvalid & 0xfeU)))) {
        Verilated::overWidthError("s_axil_wvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_bready & 0xfeU)))) {
        Verilated::overWidthError("s_axil_bready");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_arvalid & 0xfeU)))) {
        Verilated::overWidthError("s_axil_arvalid");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_araddr & 0xf0U)))) {
        Verilated::overWidthError("s_axil_araddr");
    }
    if (VL_UNLIKELY(((vlSelfRef.s_axil_rready & 0xfeU)))) {
        Verilated::overWidthError("s_axil_rready");
    }
}
#endif  // VL_DEBUG
