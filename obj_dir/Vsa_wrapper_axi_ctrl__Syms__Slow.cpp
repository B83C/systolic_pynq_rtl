// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "Vsa_wrapper_axi_ctrl__pch.h"

Vsa_wrapper_axi_ctrl__Syms::Vsa_wrapper_axi_ctrl__Syms(VerilatedContext* contextp, const char* namep, Vsa_wrapper_axi_ctrl* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp{modelp}
    // Setup top module instance
    , TOP{this, namep}
{
    // Check resources
    Verilated::stackCheck(414);
    // Setup sub module instances
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-9);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(true);
    // Setup scopes
}

Vsa_wrapper_axi_ctrl__Syms::~Vsa_wrapper_axi_ctrl__Syms() {
    // Tear down scopes
    // Tear down sub module instances
}
