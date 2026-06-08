// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vconv_sa_tb.h for the primary calling header

#include "Vconv_sa_tb__pch.h"

void Vconv_sa_tb___024root___ctor_var_reset(Vconv_sa_tb___024root* vlSelf);

Vconv_sa_tb___024root::Vconv_sa_tb___024root(Vconv_sa_tb__Syms* symsp, const char* namep)
    : __VdlySched{*symsp->_vm_contextp__}
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vconv_sa_tb___024root___ctor_var_reset(this);
}

void Vconv_sa_tb___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vconv_sa_tb___024root::~Vconv_sa_tb___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
