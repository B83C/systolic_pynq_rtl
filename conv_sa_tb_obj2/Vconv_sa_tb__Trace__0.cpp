// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals

#include "verilated_fst_c.h"
#include "Vconv_sa_tb__Syms.h"


void Vconv_sa_tb___024root__trace_chg_0_sub_0(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp);

void Vconv_sa_tb___024root__trace_chg_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_0\n"); );
    // Body
    Vconv_sa_tb___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vconv_sa_tb___024root*>(voidSelf);
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    Vconv_sa_tb___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vconv_sa_tb___024root__trace_chg_dtype____0(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____1(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____2(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 2>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____3(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____4(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____5(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<IData/*31:0*/, 4>& __VdtypeVar);
void Vconv_sa_tb___024root__trace_chg_dtype____6(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<IData/*31:0*/, 4>& __VdtypeVar);

void Vconv_sa_tb___024root__trace_chg_0_sub_0(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_0_sub_0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 0);
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[1U] 
                      | vlSelfRef.__Vm_traceActivity[2U])))) {
        bufp->chgBit(oldp+0,(vlSelfRef.conv_sa_tb__DOT__rst_n));
        bufp->chgCData(oldp+1,(vlSelfRef.conv_sa_tb__DOT__pix_tdata),8);
        bufp->chgBit(oldp+2,(vlSelfRef.conv_sa_tb__DOT__pix_tvalid));
        bufp->chgBit(oldp+3,(vlSelfRef.conv_sa_tb__DOT__pix_tlast));
        bufp->chgBit(oldp+4,(vlSelfRef.conv_sa_tb__DOT__pix_tuser));
        bufp->chgCData(oldp+5,(vlSelfRef.conv_sa_tb__DOT__kernel_tdata),8);
        bufp->chgBit(oldp+6,(vlSelfRef.conv_sa_tb__DOT__kernel_tvalid));
        bufp->chgBit(oldp+7,(vlSelfRef.conv_sa_tb__DOT__kernel_tlast));
        bufp->chgBit(oldp+8,(vlSelfRef.conv_sa_tb__DOT__res_tready));
    }
    if (VL_UNLIKELY(((vlSelfRef.__Vm_traceActivity[1U] 
                      | vlSelfRef.__Vm_traceActivity[3U])))) {
        Vconv_sa_tb___024root__trace_chg_dtype____0(vlSelf, bufp, 9, vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_b_row);
        bufp->chgCData(oldp+13,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U]),8);
        bufp->chgCData(oldp+14,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U]),8);
        bufp->chgCData(oldp+15,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U]),8);
        bufp->chgCData(oldp+16,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U]),8);
        bufp->chgCData(oldp+17,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U]),8);
        bufp->chgCData(oldp+18,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U]),8);
        bufp->chgCData(oldp+19,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U]),8);
        bufp->chgCData(oldp+20,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U]),8);
        bufp->chgCData(oldp+21,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U]),8);
        bufp->chgCData(oldp+22,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U]),8);
        bufp->chgCData(oldp+23,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U]),8);
        bufp->chgCData(oldp+24,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U]),8);
        bufp->chgCData(oldp+25,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U]),8);
        bufp->chgCData(oldp+26,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U]),8);
        bufp->chgCData(oldp+27,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U]),8);
        bufp->chgCData(oldp+28,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U]),8);
        bufp->chgCData(oldp+29,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U]),8);
        bufp->chgCData(oldp+30,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]),8);
        bufp->chgIData(oldp+31,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+32,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][0U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][0U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]))),32);
        bufp->chgCData(oldp+33,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U]),8);
        bufp->chgIData(oldp+34,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+35,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][1U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][1U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]))),32);
        bufp->chgCData(oldp+36,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U]),8);
        bufp->chgIData(oldp+37,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+38,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][2U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][2U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]))),32);
        bufp->chgCData(oldp+39,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U]),8);
        bufp->chgIData(oldp+40,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__0__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+41,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][3U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[0U][3U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[0U][0U]))),32);
        bufp->chgCData(oldp+42,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U]),8);
        bufp->chgCData(oldp+43,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]),8);
        bufp->chgIData(oldp+44,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+45,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][0U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][0U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]))),32);
        bufp->chgCData(oldp+46,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U]),8);
        bufp->chgIData(oldp+47,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+48,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][1U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][1U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]))),32);
        bufp->chgCData(oldp+49,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U]),8);
        bufp->chgIData(oldp+50,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+51,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][2U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][2U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]))),32);
        bufp->chgCData(oldp+52,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U]),8);
        bufp->chgIData(oldp+53,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__1__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+54,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][3U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[1U][3U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[1U][1U]))),32);
        bufp->chgCData(oldp+55,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U]),8);
        bufp->chgCData(oldp+56,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]),8);
        bufp->chgIData(oldp+57,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+58,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][0U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][0U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]))),32);
        bufp->chgCData(oldp+59,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U]),8);
        bufp->chgIData(oldp+60,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+61,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][1U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][1U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]))),32);
        bufp->chgCData(oldp+62,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U]),8);
        bufp->chgIData(oldp+63,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+64,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][2U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][2U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]))),32);
        bufp->chgCData(oldp+65,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U]),8);
        bufp->chgIData(oldp+66,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__2__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+67,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][3U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[2U][3U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[2U][2U]))),32);
        bufp->chgCData(oldp+68,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U]),8);
        bufp->chgCData(oldp+69,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]),8);
        bufp->chgIData(oldp+70,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__0__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+71,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][0U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][0U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]))),32);
        bufp->chgCData(oldp+72,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U]),8);
        bufp->chgIData(oldp+73,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__1__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+74,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][1U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][1U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]))),32);
        bufp->chgCData(oldp+75,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U]),8);
        bufp->chgIData(oldp+76,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__2__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+77,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][2U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][2U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]))),32);
        bufp->chgCData(oldp+78,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U]),8);
        bufp->chgIData(oldp+79,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__gen_row__BRA__3__KET____DOT__gen_col__BRA__3__KET____DOT__pe_unit__DOT__sum),32);
        bufp->chgIData(oldp+80,((vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][3U] 
                                 + (vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__b_inner[3U][3U] 
                                    * vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__a_inner_loop[3U][3U]))),32);
    }
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[3U]))) {
        bufp->chgBit(oldp+81,(vlSelfRef.conv_sa_tb__DOT__pix_tready));
        bufp->chgIData(oldp+82,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row[0U]),32);
        bufp->chgBit(oldp+83,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v));
        bufp->chgBit(oldp+84,(((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__result_v) 
                               & VL_LTES_III(32, 9U, (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt)))));
        bufp->chgCData(oldp+85,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state),3);
        Vconv_sa_tb___024root__trace_chg_dtype____1(vlSelf, bufp, 86, vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_reg);
        bufp->chgCData(oldp+90,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt),3);
        bufp->chgBit(oldp+91,((4U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__kernel_cnt))));
        bufp->chgCData(oldp+92,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__b_cycle),2);
        Vconv_sa_tb___024root__trace_chg_dtype____2(vlSelf, bufp, 93, vlSelfRef.conv_sa_tb__DOT__dut__DOT__line_buf);
        bufp->chgBit(oldp+101,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_row));
        bufp->chgCData(oldp+102,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col),2);
        bufp->chgCData(oldp+103,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx),4);
        bufp->chgBit(oldp+104,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame));
        Vconv_sa_tb___024root__trace_chg_dtype____3(vlSelf, bufp, 105, vlSelfRef.conv_sa_tb__DOT__dut__DOT__window_cap);
        bufp->chgBit(oldp+109,(((IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__in_frame) 
                                & ((1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__row_rx)) 
                                   & (1U <= (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__wr_col))))));
        bufp->chgCData(oldp+110,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step),2);
        bufp->chgBit(oldp+111,((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step))));
        bufp->chgBit(oldp+112,((3U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__step))));
        bufp->chgCData(oldp+113,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt),4);
        bufp->chgBit(oldp+114,(VL_LTES_III(32, 9U, (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__win_cnt))));
        Vconv_sa_tb___024root__trace_chg_dtype____4(vlSelf, bufp, 115, vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_a_row);
        Vconv_sa_tb___024root__trace_chg_dtype____5(vlSelf, bufp, 119, vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_result_row);
        bufp->chgCData(oldp+123,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_cur_row),4);
        bufp->chgBit(oldp+124,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_load_a));
        bufp->chgBit(oldp+125,((2U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))));
        bufp->chgIData(oldp+126,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][0U]),32);
        bufp->chgIData(oldp+127,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][1U]),32);
        bufp->chgIData(oldp+128,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][2U]),32);
        bufp->chgIData(oldp+129,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][3U]),32);
        bufp->chgIData(oldp+130,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][0U]),32);
        bufp->chgIData(oldp+131,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][1U]),32);
        bufp->chgIData(oldp+132,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][2U]),32);
        bufp->chgIData(oldp+133,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][3U]),32);
        bufp->chgIData(oldp+134,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][0U]),32);
        bufp->chgIData(oldp+135,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][1U]),32);
        bufp->chgIData(oldp+136,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][2U]),32);
        bufp->chgIData(oldp+137,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][3U]),32);
        bufp->chgIData(oldp+138,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][0U]),32);
        bufp->chgIData(oldp+139,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][1U]),32);
        bufp->chgIData(oldp+140,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][2U]),32);
        bufp->chgIData(oldp+141,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][3U]),32);
        bufp->chgIData(oldp+142,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][0U]),32);
        bufp->chgIData(oldp+143,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][1U]),32);
        bufp->chgIData(oldp+144,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][2U]),32);
        bufp->chgIData(oldp+145,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[4U][3U]),32);
        bufp->chgIData(oldp+146,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][0U]),32);
        bufp->chgIData(oldp+147,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][1U]),32);
        bufp->chgIData(oldp+148,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][2U]),32);
        bufp->chgIData(oldp+149,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[0U][3U]),32);
        bufp->chgIData(oldp+150,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][0U]),32);
        bufp->chgIData(oldp+151,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][1U]),32);
        bufp->chgIData(oldp+152,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][2U]),32);
        bufp->chgIData(oldp+153,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[1U][3U]),32);
        bufp->chgIData(oldp+154,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][0U]),32);
        bufp->chgIData(oldp+155,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][1U]),32);
        bufp->chgIData(oldp+156,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][2U]),32);
        bufp->chgIData(oldp+157,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[2U][3U]),32);
        bufp->chgIData(oldp+158,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][0U]),32);
        bufp->chgIData(oldp+159,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][1U]),32);
        bufp->chgIData(oldp+160,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][2U]),32);
        bufp->chgIData(oldp+161,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_core__DOT__partial_sum[3U][3U]),32);
    }
    bufp->chgBit(oldp+162,(vlSelfRef.conv_sa_tb__DOT__clk));
    bufp->chgBit(oldp+163,(((~ (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall)) 
                            & ((0U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state)) 
                               | (1U == (IData)(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state))))));
    bufp->chgCData(oldp+164,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__state_nxt),3);
    bufp->chgBit(oldp+165,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__new_window));
    Vconv_sa_tb___024root__trace_chg_dtype____6(vlSelf, bufp, 166, vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_c_row);
    bufp->chgBit(oldp+170,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__sa_valid));
    bufp->chgBit(oldp+171,(vlSelfRef.conv_sa_tb__DOT__dut__DOT__out_stall));
}

void Vconv_sa_tb___024root__trace_chg_dtype____0(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____0\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgCData(oldp+0,(__VdtypeVar[0]),8);
    bufp->chgCData(oldp+1,(__VdtypeVar[1]),8);
    bufp->chgCData(oldp+2,(__VdtypeVar[2]),8);
    bufp->chgCData(oldp+3,(__VdtypeVar[3]),8);
}

void Vconv_sa_tb___024root__trace_chg_dtype____1(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____1\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgCData(oldp+0,(__VdtypeVar[0]),8);
    bufp->chgCData(oldp+1,(__VdtypeVar[1]),8);
    bufp->chgCData(oldp+2,(__VdtypeVar[2]),8);
    bufp->chgCData(oldp+3,(__VdtypeVar[3]),8);
}

void Vconv_sa_tb___024root__trace_chg_dtype____2(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<VlUnpacked<CData/*7:0*/, 4>, 2>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____2\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgCData(oldp+0,(__VdtypeVar[0U][0U]),8);
    bufp->chgCData(oldp+1,(__VdtypeVar[0U][1U]),8);
    bufp->chgCData(oldp+2,(__VdtypeVar[0U][2U]),8);
    bufp->chgCData(oldp+3,(__VdtypeVar[0U][3U]),8);
    bufp->chgCData(oldp+4,(__VdtypeVar[1U][0U]),8);
    bufp->chgCData(oldp+5,(__VdtypeVar[1U][1U]),8);
    bufp->chgCData(oldp+6,(__VdtypeVar[1U][2U]),8);
    bufp->chgCData(oldp+7,(__VdtypeVar[1U][3U]),8);
}

void Vconv_sa_tb___024root__trace_chg_dtype____3(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____3\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgCData(oldp+0,(__VdtypeVar[0]),8);
    bufp->chgCData(oldp+1,(__VdtypeVar[1]),8);
    bufp->chgCData(oldp+2,(__VdtypeVar[2]),8);
    bufp->chgCData(oldp+3,(__VdtypeVar[3]),8);
}

void Vconv_sa_tb___024root__trace_chg_dtype____4(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<CData/*7:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____4\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgCData(oldp+0,(__VdtypeVar[0]),8);
    bufp->chgCData(oldp+1,(__VdtypeVar[1]),8);
    bufp->chgCData(oldp+2,(__VdtypeVar[2]),8);
    bufp->chgCData(oldp+3,(__VdtypeVar[3]),8);
}

void Vconv_sa_tb___024root__trace_chg_dtype____5(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<IData/*31:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____5\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgIData(oldp+0,(__VdtypeVar[0]),32);
    bufp->chgIData(oldp+1,(__VdtypeVar[1]),32);
    bufp->chgIData(oldp+2,(__VdtypeVar[2]),32);
    bufp->chgIData(oldp+3,(__VdtypeVar[3]),32);
}

void Vconv_sa_tb___024root__trace_chg_dtype____6(Vconv_sa_tb___024root* vlSelf, VerilatedFst::Buffer* bufp, uint32_t offset, const VlUnpacked<IData/*31:0*/, 4>& __VdtypeVar) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_chg_dtype____6\n"); );
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode +  offset);
    bufp->chgIData(oldp+0,(__VdtypeVar[0]),32);
    bufp->chgIData(oldp+1,(__VdtypeVar[1]),32);
    bufp->chgIData(oldp+2,(__VdtypeVar[2]),32);
    bufp->chgIData(oldp+3,(__VdtypeVar[3]),32);
}

void Vconv_sa_tb___024root__trace_cleanup(void* voidSelf, VerilatedFst* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vconv_sa_tb___024root__trace_cleanup\n"); );
    // Body
    Vconv_sa_tb___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vconv_sa_tb___024root*>(voidSelf);
    Vconv_sa_tb__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[2U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[3U] = 0U;
}
