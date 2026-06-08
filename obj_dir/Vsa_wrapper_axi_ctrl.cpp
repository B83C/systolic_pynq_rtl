// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vsa_wrapper_axi_ctrl__pch.h"

//============================================================
// Constructors

Vsa_wrapper_axi_ctrl::Vsa_wrapper_axi_ctrl(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vsa_wrapper_axi_ctrl__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst_n{vlSymsp->TOP.rst_n}
    , s_axis_A_tvalid{vlSymsp->TOP.s_axis_A_tvalid}
    , s_axis_A_tready{vlSymsp->TOP.s_axis_A_tready}
    , s_axis_A_tlast{vlSymsp->TOP.s_axis_A_tlast}
    , s_axis_B_tvalid{vlSymsp->TOP.s_axis_B_tvalid}
    , s_axis_B_tready{vlSymsp->TOP.s_axis_B_tready}
    , s_axis_B_tlast{vlSymsp->TOP.s_axis_B_tlast}
    , m_axis_tvalid{vlSymsp->TOP.m_axis_tvalid}
    , m_axis_tready{vlSymsp->TOP.m_axis_tready}
    , m_axis_tlast{vlSymsp->TOP.m_axis_tlast}
    , s_axil_awvalid{vlSymsp->TOP.s_axil_awvalid}
    , s_axil_awready{vlSymsp->TOP.s_axil_awready}
    , s_axil_awaddr{vlSymsp->TOP.s_axil_awaddr}
    , s_axil_wvalid{vlSymsp->TOP.s_axil_wvalid}
    , s_axil_wready{vlSymsp->TOP.s_axil_wready}
    , s_axil_bresp{vlSymsp->TOP.s_axil_bresp}
    , s_axil_bvalid{vlSymsp->TOP.s_axil_bvalid}
    , s_axil_bready{vlSymsp->TOP.s_axil_bready}
    , s_axil_arvalid{vlSymsp->TOP.s_axil_arvalid}
    , s_axil_arready{vlSymsp->TOP.s_axil_arready}
    , s_axil_araddr{vlSymsp->TOP.s_axil_araddr}
    , s_axil_rresp{vlSymsp->TOP.s_axil_rresp}
    , s_axil_rvalid{vlSymsp->TOP.s_axil_rvalid}
    , s_axil_rready{vlSymsp->TOP.s_axil_rready}
    , s_axis_A_tdata{vlSymsp->TOP.s_axis_A_tdata}
    , s_axis_B_tdata{vlSymsp->TOP.s_axis_B_tdata}
    , m_axis_tdata{vlSymsp->TOP.m_axis_tdata}
    , s_axil_wdata{vlSymsp->TOP.s_axil_wdata}
    , s_axil_rdata{vlSymsp->TOP.s_axil_rdata}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vsa_wrapper_axi_ctrl::Vsa_wrapper_axi_ctrl(const char* _vcname__)
    : Vsa_wrapper_axi_ctrl(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vsa_wrapper_axi_ctrl::~Vsa_wrapper_axi_ctrl() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vsa_wrapper_axi_ctrl___024root___eval_debug_assertions(Vsa_wrapper_axi_ctrl___024root* vlSelf);
#endif  // VL_DEBUG
void Vsa_wrapper_axi_ctrl___024root___eval_static(Vsa_wrapper_axi_ctrl___024root* vlSelf);
void Vsa_wrapper_axi_ctrl___024root___eval_initial(Vsa_wrapper_axi_ctrl___024root* vlSelf);
void Vsa_wrapper_axi_ctrl___024root___eval_settle(Vsa_wrapper_axi_ctrl___024root* vlSelf);
void Vsa_wrapper_axi_ctrl___024root___eval(Vsa_wrapper_axi_ctrl___024root* vlSelf);

void Vsa_wrapper_axi_ctrl::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vsa_wrapper_axi_ctrl::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vsa_wrapper_axi_ctrl___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vsa_wrapper_axi_ctrl___024root___eval_static(&(vlSymsp->TOP));
        Vsa_wrapper_axi_ctrl___024root___eval_initial(&(vlSymsp->TOP));
        Vsa_wrapper_axi_ctrl___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vsa_wrapper_axi_ctrl___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vsa_wrapper_axi_ctrl::eventsPending() { return false; }

uint64_t Vsa_wrapper_axi_ctrl::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vsa_wrapper_axi_ctrl::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vsa_wrapper_axi_ctrl___024root___eval_final(Vsa_wrapper_axi_ctrl___024root* vlSelf);

VL_ATTR_COLD void Vsa_wrapper_axi_ctrl::final() {
    contextp()->executingFinal(true);
    Vsa_wrapper_axi_ctrl___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vsa_wrapper_axi_ctrl::hierName() const { return vlSymsp->name(); }
const char* Vsa_wrapper_axi_ctrl::modelName() const { return "Vsa_wrapper_axi_ctrl"; }
unsigned Vsa_wrapper_axi_ctrl::threads() const { return 1; }
void Vsa_wrapper_axi_ctrl::prepareClone() const { contextp()->prepareClone(); }
void Vsa_wrapper_axi_ctrl::atClone() const {
    contextp()->threadPoolpOnClone();
}
