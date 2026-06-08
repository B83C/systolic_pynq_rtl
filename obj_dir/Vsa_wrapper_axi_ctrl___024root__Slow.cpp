// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vsa_wrapper_axi_ctrl.h for the primary calling header

#include "Vsa_wrapper_axi_ctrl__pch.h"

void Vsa_wrapper_axi_ctrl___024root___ctor_var_reset(Vsa_wrapper_axi_ctrl___024root* vlSelf);

Vsa_wrapper_axi_ctrl___024root::Vsa_wrapper_axi_ctrl___024root(Vsa_wrapper_axi_ctrl__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vsa_wrapper_axi_ctrl___024root___ctor_var_reset(this);
}

void Vsa_wrapper_axi_ctrl___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vsa_wrapper_axi_ctrl___024root::~Vsa_wrapper_axi_ctrl___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
