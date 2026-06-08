// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vconv_sa_tb.h for the primary calling header

#include "Vconv_sa_tb__pch.h"

VL_ATTR_COLD void Vconv_sa_tb___024root___eval_initial__TOP(Vconv_sa_tb___024root* vlSelf);
VlCoroutine Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__0(Vconv_sa_tb___024root* vlSelf);
VlCoroutine Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__1(Vconv_sa_tb___024root* vlSelf);

void Vconv_sa_tb___024root___eval_initial(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_initial\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vconv_sa_tb___024root___eval_initial__TOP(vlSelf);
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
    Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

void Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(Vconv_sa_tb___024root* vlSelf, const char* __VeventDescription);

VlCoroutine Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__0(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0;
    conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1;
    conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__0__d;
    __Vtask_conv_sa_tb__DOT__send_kernel__0__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__0__last;
    __Vtask_conv_sa_tb__DOT__send_kernel__0__last = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__1__d;
    __Vtask_conv_sa_tb__DOT__send_kernel__1__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__1__last;
    __Vtask_conv_sa_tb__DOT__send_kernel__1__last = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__2__d;
    __Vtask_conv_sa_tb__DOT__send_kernel__2__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__2__last;
    __Vtask_conv_sa_tb__DOT__send_kernel__2__last = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__3__d;
    __Vtask_conv_sa_tb__DOT__send_kernel__3__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_kernel__3__last;
    __Vtask_conv_sa_tb__DOT__send_kernel__3__last = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__4__d;
    __Vtask_conv_sa_tb__DOT__send_pix__4__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__4__last;
    __Vtask_conv_sa_tb__DOT__send_pix__4__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__4__user;
    __Vtask_conv_sa_tb__DOT__send_pix__4__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__5__d;
    __Vtask_conv_sa_tb__DOT__send_pix__5__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__5__last;
    __Vtask_conv_sa_tb__DOT__send_pix__5__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__5__user;
    __Vtask_conv_sa_tb__DOT__send_pix__5__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__6__d;
    __Vtask_conv_sa_tb__DOT__send_pix__6__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__6__last;
    __Vtask_conv_sa_tb__DOT__send_pix__6__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__6__user;
    __Vtask_conv_sa_tb__DOT__send_pix__6__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__7__d;
    __Vtask_conv_sa_tb__DOT__send_pix__7__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__7__last;
    __Vtask_conv_sa_tb__DOT__send_pix__7__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__7__user;
    __Vtask_conv_sa_tb__DOT__send_pix__7__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__8__d;
    __Vtask_conv_sa_tb__DOT__send_pix__8__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__8__last;
    __Vtask_conv_sa_tb__DOT__send_pix__8__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__8__user;
    __Vtask_conv_sa_tb__DOT__send_pix__8__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__9__d;
    __Vtask_conv_sa_tb__DOT__send_pix__9__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__9__last;
    __Vtask_conv_sa_tb__DOT__send_pix__9__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__9__user;
    __Vtask_conv_sa_tb__DOT__send_pix__9__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__10__d;
    __Vtask_conv_sa_tb__DOT__send_pix__10__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__10__last;
    __Vtask_conv_sa_tb__DOT__send_pix__10__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__10__user;
    __Vtask_conv_sa_tb__DOT__send_pix__10__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__11__d;
    __Vtask_conv_sa_tb__DOT__send_pix__11__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__11__last;
    __Vtask_conv_sa_tb__DOT__send_pix__11__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__11__user;
    __Vtask_conv_sa_tb__DOT__send_pix__11__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__12__d;
    __Vtask_conv_sa_tb__DOT__send_pix__12__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__12__last;
    __Vtask_conv_sa_tb__DOT__send_pix__12__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__12__user;
    __Vtask_conv_sa_tb__DOT__send_pix__12__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__13__d;
    __Vtask_conv_sa_tb__DOT__send_pix__13__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__13__last;
    __Vtask_conv_sa_tb__DOT__send_pix__13__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__13__user;
    __Vtask_conv_sa_tb__DOT__send_pix__13__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__14__d;
    __Vtask_conv_sa_tb__DOT__send_pix__14__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__14__last;
    __Vtask_conv_sa_tb__DOT__send_pix__14__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__14__user;
    __Vtask_conv_sa_tb__DOT__send_pix__14__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__15__d;
    __Vtask_conv_sa_tb__DOT__send_pix__15__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__15__last;
    __Vtask_conv_sa_tb__DOT__send_pix__15__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__15__user;
    __Vtask_conv_sa_tb__DOT__send_pix__15__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__16__d;
    __Vtask_conv_sa_tb__DOT__send_pix__16__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__16__last;
    __Vtask_conv_sa_tb__DOT__send_pix__16__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__16__user;
    __Vtask_conv_sa_tb__DOT__send_pix__16__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__17__d;
    __Vtask_conv_sa_tb__DOT__send_pix__17__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__17__last;
    __Vtask_conv_sa_tb__DOT__send_pix__17__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__17__user;
    __Vtask_conv_sa_tb__DOT__send_pix__17__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__18__d;
    __Vtask_conv_sa_tb__DOT__send_pix__18__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__18__last;
    __Vtask_conv_sa_tb__DOT__send_pix__18__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__18__user;
    __Vtask_conv_sa_tb__DOT__send_pix__18__user = 0;
    CData/*7:0*/ __Vtask_conv_sa_tb__DOT__send_pix__19__d;
    __Vtask_conv_sa_tb__DOT__send_pix__19__d = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__19__last;
    __Vtask_conv_sa_tb__DOT__send_pix__19__last = 0;
    CData/*0:0*/ __Vtask_conv_sa_tb__DOT__send_pix__19__user;
    __Vtask_conv_sa_tb__DOT__send_pix__19__user = 0;
    // Body
    VL_WRITEF_NX("=== conv_sa test ===\n",0);
    vlSymsp->_vm_contextp__->dumpfile("waveform.fst"s);
    vlSymsp->_traceDumpOpen();
    vlSelfRef.conv_sa_tb__DOT__clk = 0U;
    vlSelfRef.conv_sa_tb__DOT__rst_n = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__res_tready = 1U;
    co_await vlSelfRef.__VdlySched.delay(0x0000000000000fa0ULL, 
                                         nullptr, "conv_sa_tb.sv", 
                                         87);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    vlSelfRef.conv_sa_tb__DOT__rst_n = 1U;
    VL_WRITEF_NX("reset done\n",0);
    __Vtask_conv_sa_tb__DOT__send_kernel__0__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__0__d = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tdata = __Vtask_conv_sa_tb__DOT__send_kernel__0__d;
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tlast = __Vtask_conv_sa_tb__DOT__send_kernel__0__last;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         59);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    VL_WRITEF_NX("  kernel loaded val=%0d cnt=%0d done=%0d state=%0d\n",4
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_kernel__0__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)
                 , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state));
    while ((1U & (~ ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
                     & ((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                        | (1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))))))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             61);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__1__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__1__d = 0U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tdata = __Vtask_conv_sa_tb__DOT__send_kernel__1__d;
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tlast = __Vtask_conv_sa_tb__DOT__send_kernel__1__last;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         59);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    VL_WRITEF_NX("  kernel loaded val=%0d cnt=%0d done=%0d state=%0d\n",4
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_kernel__1__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)
                 , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state));
    while ((1U & (~ ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
                     & ((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                        | (1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))))))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             61);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__2__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__2__d = 0U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tdata = __Vtask_conv_sa_tb__DOT__send_kernel__2__d;
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tlast = __Vtask_conv_sa_tb__DOT__send_kernel__2__last;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         59);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    VL_WRITEF_NX("  kernel loaded val=%0d cnt=%0d done=%0d state=%0d\n",4
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_kernel__2__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)
                 , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state));
    while ((1U & (~ ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
                     & ((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                        | (1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))))))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             61);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 0U;
    __Vtask_conv_sa_tb__DOT__send_kernel__3__last = 1U;
    __Vtask_conv_sa_tb__DOT__send_kernel__3__d = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tdata = __Vtask_conv_sa_tb__DOT__send_kernel__3__d;
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__kernel_tlast = __Vtask_conv_sa_tb__DOT__send_kernel__3__last;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         59);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    VL_WRITEF_NX("  kernel loaded val=%0d cnt=%0d done=%0d state=%0d\n",4
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_kernel__3__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)
                 , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state));
    while ((1U & (~ ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
                     & ((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                        | (1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))))))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             61);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    vlSelfRef.conv_sa_tb__DOT__kernel_tvalid = 0U;
    VL_WRITEF_NX("kernel loaded (kernel_done=%0d kernel_cnt=%0d)\n",2
                 , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt));
    conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0x0000000aU;
    while (VL_LTS_III(32, 0U, conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             98);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
        VL_WRITEF_NX("  state=%0d state_nxt=%0d kernel_done=%0d kernel_cnt=%0d\n",4
                     , '#',3,vlSelfRef.conv_sa_tb__DOT__dut__DOT__state
                     , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state_nxt)
                     , '#',1,(4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))
                     , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt));
        conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (conv_sa_tb__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    VL_WRITEF_NX("fill done, state=%0d\n",1, '#',3,vlSelfRef.conv_sa_tb__DOT__dut__DOT__state);
    __Vtask_conv_sa_tb__DOT__send_pix__4__user = 1U;
    __Vtask_conv_sa_tb__DOT__send_pix__4__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__4__d = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__4__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__4__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__4__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__4__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__5__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__5__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__5__d = 2U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__5__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__5__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__5__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__5__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__6__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__6__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__6__d = 3U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__6__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__6__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__6__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__6__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__7__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__7__last = 1U;
    __Vtask_conv_sa_tb__DOT__send_pix__7__d = 4U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__7__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__7__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__7__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__7__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__8__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__8__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__8__d = 5U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__8__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__8__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__8__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__8__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__9__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__9__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__9__d = 6U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__9__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__9__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__9__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__9__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__10__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__10__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__10__d = 7U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__10__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__10__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__10__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__10__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__11__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__11__last = 1U;
    __Vtask_conv_sa_tb__DOT__send_pix__11__d = 8U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__11__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__11__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__11__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__11__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__12__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__12__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__12__d = 9U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__12__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__12__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__12__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__12__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__13__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__13__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__13__d = 8U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__13__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__13__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__13__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__13__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__14__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__14__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__14__d = 7U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__14__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__14__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__14__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__14__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__15__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__15__last = 1U;
    __Vtask_conv_sa_tb__DOT__send_pix__15__d = 6U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__15__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__15__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__15__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__15__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__16__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__16__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__16__d = 5U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__16__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__16__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__16__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__16__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__17__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__17__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__17__d = 4U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__17__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__17__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__17__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__17__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__18__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__18__last = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__18__d = 3U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__18__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__18__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__18__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__18__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__19__user = 0U;
    __Vtask_conv_sa_tb__DOT__send_pix__19__last = 1U;
    __Vtask_conv_sa_tb__DOT__send_pix__19__d = 2U;
    vlSelfRef.conv_sa_tb__DOT__pix_tdata = __Vtask_conv_sa_tb__DOT__send_pix__19__d;
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 1U;
    vlSelfRef.conv_sa_tb__DOT__pix_tlast = __Vtask_conv_sa_tb__DOT__send_pix__19__last;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = __Vtask_conv_sa_tb__DOT__send_pix__19__user;
    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                      "@(posedge conv_sa_tb.clk)");
    co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                         nullptr, 
                                                         "@(posedge conv_sa_tb.clk)", 
                                                         "conv_sa_tb.sv", 
                                                         69);
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    while ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready)))) {
        Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                          "@(posedge conv_sa_tb.clk)");
        co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge conv_sa_tb.clk)", 
                                                             "conv_sa_tb.sv", 
                                                             70);
        vlSelfRef.__Vm_traceActivity[2U] = 1U;
    }
    VL_WRITEF_NX("pix %0d accepted (state=%0d win_cnt=%0d)\n",3
                 , '#',8,__Vtask_conv_sa_tb__DOT__send_pix__19__d
                 , '#',3,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    vlSelfRef.conv_sa_tb__DOT__pix_tvalid = 0U;
    vlSelfRef.conv_sa_tb__DOT__pix_tuser = 0U;
    VL_WRITEF_NX("all 16 pixels sent, state=%0d step=%0d win_cnt=%0d\nwaiting for result...\n",3
                 , '#',3,vlSelfRef.conv_sa_tb__DOT__dut__DOT__state
                 , '#',2,(IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)
                 , '#',4,vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt);
    conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0x000000c8U;
    {
        while (VL_LTS_III(32, 0U, conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                              "@(posedge conv_sa_tb.clk)");
            co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge conv_sa_tb.clk)", 
                                                                 "conv_sa_tb.sv", 
                                                                 124);
            vlSelfRef.__Vm_traceActivity[2U] = 1U;
            if (VL_UNLIKELY((vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v))) {
                VL_WRITEF_NX("GOT result: %0d\n",1, '#',32,vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[0U]);
                Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(vlSelf, 
                                                                  "@(posedge conv_sa_tb.clk)");
                co_await vlSelfRef.__VtrigSched_h822c2670__0.trigger(0U, 
                                                                     nullptr, 
                                                                     "@(posedge conv_sa_tb.clk)", 
                                                                     "conv_sa_tb.sv", 
                                                                     127);
                vlSelfRef.__Vm_traceActivity[2U] = 1U;
                VL_WRITEF_NX("PASS\n",0);
                VL_FINISH_MT("conv_sa_tb.sv", 129, "");
                goto __Vlabel0;
            }
            conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (conv_sa_tb__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        __Vlabel0: ;
    }
    VL_WRITEF_NX("TIMEOUT - no result seen\n",0);
    VL_FINISH_MT("conv_sa_tb.sv", 133, "");
    vlSelfRef.__Vm_traceActivity[2U] = 1U;
    co_return;
}

VlCoroutine Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__1(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(0x00000000000003e8ULL, 
                                             nullptr, 
                                             "conv_sa_tb.sv", 
                                             54);
        vlSelfRef.conv_sa_tb__DOT__clk = (1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__clk)));
    }
    co_return;
}

void Vconv_sa_tb___024root___eval_triggers_vec__act(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_triggers_vec__act\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                      << 2U) 
                                                     | ((((~ (IData)(vlSelfRef.conv_sa_tb__DOT__rst_n)) 
                                                          & (IData)(vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__rst_n__0)) 
                                                         << 1U) 
                                                        | ((IData)(vlSelfRef.conv_sa_tb__DOT__clk) 
                                                           & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0)))))));
    vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0 
        = vlSelfRef.conv_sa_tb__DOT__clk;
    vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__rst_n__0 
        = vlSelfRef.conv_sa_tb__DOT__rst_n;
}

bool Vconv_sa_tb___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___trigger_anySet__act\n"); );
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

void Vconv_sa_tb___024root___act_comb__TOP__0(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___act_comb__TOP__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window 
        = ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tvalid) 
           & ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready) 
              & ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame) 
                 & ((1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx)) 
                    & (1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col))))));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall 
        = ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0) 
           & ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__res_tready)) 
              & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v)));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_valid = 
        ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
         & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a));
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

void Vconv_sa_tb___024root___eval_act(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_act\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((5ULL & vlSelfRef.__VactTriggered[0U])) {
        Vconv_sa_tb___024root___act_comb__TOP__0(vlSelf);
    }
}

void Vconv_sa_tb___024root___nba_sequent__TOP__0(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___nba_sequent__TOP__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row;
    __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row = 0;
    CData/*1:0*/ __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col;
    __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col = 0;
    CData/*3:0*/ __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx;
    __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx = 0;
    CData/*1:0*/ __Vdly__conv_sa_tb__DOT__dut__DOT__step;
    __Vdly__conv_sa_tb__DOT__dut__DOT__step = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 = 0;
    CData/*1:0*/ __VdlyDim0__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0;
    __VdlyDim0__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__line_buf__v0;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 0;
    CData/*1:0*/ __VdlyDim0__conv_sa_tb__DOT__dut__DOT__line_buf__v0;
    __VdlyDim0__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 0;
    CData/*0:0*/ __VdlyDim1__conv_sa_tb__DOT__dut__DOT__line_buf__v0;
    __VdlyDim1__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__line_buf__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v0 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v4;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v4 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v4;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v4 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v5;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v5 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v5;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v5 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v6;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v6 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v6;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v6 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v7;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v7 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v7;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v7 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v8;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v8 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v1;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v1 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v2;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v2 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v3;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v3 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v5;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v5 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v6;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v6 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v7;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v7 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v9;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v9 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v10;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v10 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v11;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v11 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v13;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v13 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v14;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v14 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v15;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v15 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v16;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v16 = 0;
    IData/*31:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0 = 0;
    IData/*31:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v1;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v1 = 0;
    IData/*31:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v2;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v2 = 0;
    IData/*31:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v3;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v3 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v12;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v12 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v24;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v24 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v36;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v36 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46 = 0;
    CData/*7:0*/ __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47;
    __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47 = 0;
    CData/*0:0*/ __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v48;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v48 = 0;
    // Body
    __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx = vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx;
    __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row = vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 0U;
    __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col = vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 = 0U;
    __Vdly__conv_sa_tb__DOT__dut__DOT__step = vlSelfRef.conv_sa_tb__DOT__dut__DOT__step;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v0 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v4 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v5 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v6 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v7 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v8 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v16 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47 = 0U;
    __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v48 = 0U;
    if (vlSelfRef.conv_sa_tb__DOT__rst_n) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v 
            = ((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
               & (3U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)));
        if (vlSelfRef.conv_sa_tb__DOT__pix_tuser) {
            __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row = 0U;
            __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col = 0U;
            __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx = 0U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame = 1U;
        }
        if (((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tvalid) 
             & (IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready))) {
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__line_buf__v0 
                = vlSelfRef.conv_sa_tb__DOT__pix_tdata;
            __VdlyDim0__conv_sa_tb__DOT__dut__DOT__line_buf__v0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col;
            __VdlyDim1__conv_sa_tb__DOT__dut__DOT__line_buf__v0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__line_buf__v0 = 1U;
            if (vlSelfRef.conv_sa_tb__DOT__pix_tlast) {
                __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row 
                    = ((2U <= ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row)))
                        ? 0U : (1U & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row))));
                __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col = 0U;
                __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx 
                    = (0x0000000fU & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx)));
                if ((3U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx))) {
                    vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame = 0U;
                }
            } else {
                __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col 
                    = (3U & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col)));
            }
        }
        if (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_valid) {
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[0U];
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0 = 1U;
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v1 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[1U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v2 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[2U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v3 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row[3U];
            if (((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row) 
                 & (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[0U];
                __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0 = 1U;
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v1 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[1U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v2 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[2U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v3 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[3U];
            }
            if ((((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row) 
                  >> 1U) & (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[0U];
                __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4 = 1U;
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v5 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[1U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v6 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[2U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v7 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[3U];
            }
            if ((((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row) 
                  >> 2U) & (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[0U];
                __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8 = 1U;
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v9 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[1U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v10 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[2U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v11 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[3U];
            }
            if ((((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row) 
                  >> 3U) & (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[0U];
                __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12 = 1U;
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v13 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[1U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v14 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[2U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v15 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row[3U];
            }
            if (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[0U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v12 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[1U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v24 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[2U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v36 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row[3U];
            } else {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][3U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v12 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][3U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v24 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][3U];
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v36 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U];
            }
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][1U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][2U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][3U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][4U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][5U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][6U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][7U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][8U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][9U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][10U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][0U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][2U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][3U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][4U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][5U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][6U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][7U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][8U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][9U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][10U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][0U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][1U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][3U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][4U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][5U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][6U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][7U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][8U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][9U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][10U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][0U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][1U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][2U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][4U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][5U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][6U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][7U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][8U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][9U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][10U];
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT____Vlvbound_h44549618__0;
            __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][0U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][1U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][2U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][3U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][0U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][1U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][2U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][3U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][0U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][1U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][2U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][3U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][0U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][1U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][2U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum 
                = (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][3U] 
                   + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U] 
                      * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]));
        }
        if ((1U & (~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)))) {
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle 
                = ((2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))
                    ? (3U & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle)))
                    : 0U);
            if ((((1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                  | (0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))) 
                 & (IData)(vlSelfRef.conv_sa_tb__DOT__kernel_tvalid))) {
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 
                    = vlSelfRef.conv_sa_tb__DOT__kernel_tdata;
                __VdlyDim0__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 
                    = (3U & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt));
                __VdlySet__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0 = 1U;
                vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt)));
            }
            if (((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                 & (3U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)))) {
                vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt 
                    = (0x0000000fU & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt)));
            } else if ((4U != (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))) {
                vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt = 0U;
            }
            __Vdly__conv_sa_tb__DOT__dut__DOT__step 
                = ((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))
                    ? ((3U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step))
                        ? 0U : (3U & ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step))))
                    : 0U);
        }
        if (((1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
             | (2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))) {
            __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v0 = 1U;
        } else if (vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window) {
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r 
                = VL_MODDIVS_III(32, ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row)), (IData)(2U));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c 
                = ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window)
                    ? ((IData)(1U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U)))
                    : ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                       - (IData)(2U)));
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v4 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf
                [(1U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r)]
                [(3U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c)];
            __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v4 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r 
                = VL_MODDIVS_III(32, ((IData)(1U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row)), (IData)(2U));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c 
                = ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window)
                    ? ((IData)(2U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U)))
                    : ((IData)(1U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U))));
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v5 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf
                [(1U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r)]
                [(3U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c)];
            __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v5 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r 
                = VL_MODDIVS_III(32, ((IData)(2U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row)), (IData)(2U));
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c 
                = ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window)
                    ? ((IData)(1U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U)))
                    : ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                       - (IData)(2U)));
            __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v6 
                = vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf
                [(1U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r)]
                [(3U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c)];
            __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v6 = 1U;
            vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r 
                = VL_MODDIVS_III(32, ((IData)(2U) + (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row)), (IData)(2U));
            if (vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window) {
                vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c 
                    = ((IData)(2U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U)));
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v7 
                    = vlSelfRef.conv_sa_tb__DOT__pix_tdata;
            } else {
                vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c 
                    = ((IData)(1U) + ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col) 
                                      - (IData)(2U)));
                __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v7 
                    = vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf
                    [(1U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__r)]
                    [(3U & vlSelfRef.conv_sa_tb__DOT__dut__DOT__unnamedblk5__DOT__unnamedblk6__DOT__unnamedblk7__DOT__c)];
            }
            __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v7 = 1U;
        }
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__state 
            = vlSelfRef.conv_sa_tb__DOT__dut__DOT__state_nxt;
    } else {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v = 0U;
        __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row = 0U;
        __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt = 0U;
        __Vdly__conv_sa_tb__DOT__dut__DOT__step = 0U;
        __VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v8 = 1U;
        __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v16 = 1U;
        __VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v48 = 1U;
        __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame = 0U;
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
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__state = 0U;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_reg[__VdlyDim0__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__kernel_reg__v0;
    }
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row = __Vdly__conv_sa_tb__DOT__dut__DOT__wr_row;
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__line_buf__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf[__VdlyDim1__conv_sa_tb__DOT__dut__DOT__line_buf__v0][__VdlyDim0__conv_sa_tb__DOT__dut__DOT__line_buf__v0] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__line_buf__v0;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[0U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[1U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[2U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[3U] = 0U;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v4) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v4;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v5) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v5;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v6) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v6;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v7) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__window_cap__v7;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__window_cap__v8) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[0U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[1U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[2U] = 0U;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap[3U] = 0U;
    }
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col = __Vdly__conv_sa_tb__DOT__dut__DOT__wr_col;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx = __Vdly__conv_sa_tb__DOT__dut__DOT__row_rx;
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__step = __Vdly__conv_sa_tb__DOT__dut__DOT__step;
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v0;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v1;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v2;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v3;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v4;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v5;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v6;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v7;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v8;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v9;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v10;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v11;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v12;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v13;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v14;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v15;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner__v16) {
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
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v0;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v1;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v2;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v3;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][4U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v4;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][5U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v5;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][6U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v6;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][7U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v7;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][8U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v8;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][9U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v9;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][10U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v10;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][11U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v11;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v12;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v13;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v14;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v15;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][4U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v16;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][5U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v17;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][6U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v18;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][7U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v19;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][8U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v20;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][9U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v21;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][10U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v22;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][11U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v23;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v24;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v25;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v26;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v27;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][4U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v28;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][5U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v29;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][6U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v30;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][7U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v31;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][8U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v32;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][9U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v33;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][10U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v34;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][11U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v35;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v36;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v37;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v38;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v39;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][4U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v40;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][5U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v41;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][6U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v42;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][7U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v43;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][8U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v44;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][9U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v45;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][10U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v46;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][11U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v47;
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop__v48) {
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
    }
    if (__VdlySet__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0) {
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][0U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v0;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][1U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v1;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][2U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v2;
        vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][3U] 
            = __VdlyVal__conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum__v3;
    }
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
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[0U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][0U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[1U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][1U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[2U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][2U];
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[3U] 
        = vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][3U];
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
    vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0 = ((4U 
                                                 == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                                & (0U 
                                                   == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step)));
    vlSelfRef.conv_sa_tb__DOT__pix_tready = ((1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                             | ((2U 
                                                 == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0) 
                                                   | (3U 
                                                      == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)))));
}

void Vconv_sa_tb___024root___nba_comb__TOP__0(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___nba_comb__TOP__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*9:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall 
        = ((IData)(vlSelfRef.__VdfgRegularize_h6e95ff9d_0_0) 
           & ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__res_tready)) 
              & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v)));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window 
        = ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tvalid) 
           & ((IData)(vlSelfRef.conv_sa_tb__DOT__pix_tready) 
              & ((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame) 
                 & ((1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx)) 
                    & (1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col))))));
    vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_valid = 
        ((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
         & (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a));
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

void Vconv_sa_tb___024root___eval_nba(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_nba\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vconv_sa_tb___024root___nba_sequent__TOP__0(vlSelf);
        vlSelfRef.__Vm_traceActivity[3U] = 1U;
    }
    if ((7ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vconv_sa_tb___024root___nba_comb__TOP__0(vlSelf);
    }
}

void Vconv_sa_tb___024root___timing_ready(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___timing_ready\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_h822c2670__0.ready("@(posedge conv_sa_tb.clk)");
    }
}

void Vconv_sa_tb___024root___timing_resume(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___timing_resume\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VtrigSched_h822c2670__0.moveToResumeQueue(
                                                          "@(posedge conv_sa_tb.clk)");
    vlSelfRef.__VtrigSched_h822c2670__0.resume("@(posedge conv_sa_tb.clk)");
    if ((4ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vconv_sa_tb___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___trigger_orInto__act_vec_vec\n"); );
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
VL_ATTR_COLD void Vconv_sa_tb___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vconv_sa_tb___024root___eval_phase__act(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_phase__act\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vconv_sa_tb___024root___eval_triggers_vec__act(vlSelf);
    Vconv_sa_tb___024root___timing_ready(vlSelf);
    Vconv_sa_tb___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VactTriggered, vlSelfRef.__VactTriggeredAcc);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vconv_sa_tb___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vconv_sa_tb___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vconv_sa_tb___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        vlSelfRef.__VactTriggeredAcc.fill(0ULL);
        Vconv_sa_tb___024root___timing_resume(vlSelf);
        Vconv_sa_tb___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vconv_sa_tb___024root___eval_phase__inact(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_phase__inact\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VinactExecute;
    // Body
    __VinactExecute = vlSelfRef.__VdlySched.awaitingZeroDelay();
    if (__VinactExecute) {
        VL_FATAL_MT("conv_sa_tb.sv", 3, "", "ZERODLY: Design Verilated with '--no-sched-zero-delay', but #0 delay executed at runtime");
    }
    return (__VinactExecute);
}

void Vconv_sa_tb___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vconv_sa_tb___024root___eval_phase__nba(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_phase__nba\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vconv_sa_tb___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vconv_sa_tb___024root___eval_nba(vlSelf);
        Vconv_sa_tb___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vconv_sa_tb___024root___eval(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vconv_sa_tb___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("conv_sa_tb.sv", 3, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VinactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VinactIterCount)))) {
                VL_FATAL_MT("conv_sa_tb.sv", 3, "", "DIDNOTCONVERGE: Inactive region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VinactIterCount = ((IData)(1U) 
                                           + vlSelfRef.__VinactIterCount);
            vlSelfRef.__VactIterCount = 0U;
            do {
                if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                    Vconv_sa_tb___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                    VL_FATAL_MT("conv_sa_tb.sv", 3, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
                }
                vlSelfRef.__VactIterCount = ((IData)(1U) 
                                             + vlSelfRef.__VactIterCount);
                vlSelfRef.__VactPhaseResult = Vconv_sa_tb___024root___eval_phase__act(vlSelf);
            } while (vlSelfRef.__VactPhaseResult);
            vlSelfRef.__VinactPhaseResult = Vconv_sa_tb___024root___eval_phase__inact(vlSelf);
        } while (vlSelfRef.__VinactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vconv_sa_tb___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

void Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0(Vconv_sa_tb___024root* vlSelf, const char* __VeventDescription) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root____VbeforeTrig_h822c2670__0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<QData/*63:0*/, 1> __VTmp;
    // Body
    __VTmp[0U] = (QData)((IData)(((IData)(vlSelfRef.conv_sa_tb__DOT__clk) 
                                  & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__conv_sa_tb__DOT__clk__0 
        = vlSelfRef.conv_sa_tb__DOT__clk;
    if ((1ULL & __VTmp[0U])) {
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
        vlSelfRef.__VtrigSched_h822c2670__0.ready(__VeventDescription);
    }
    vlSelfRef.__VactTriggeredAcc[0U] = (vlSelfRef.__VactTriggeredAcc[0U] 
                                        | __VTmp[0U]);
}

#ifdef VL_DEBUG
void Vconv_sa_tb___024root___eval_debug_assertions(Vconv_sa_tb___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root___eval_debug_assertions\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
