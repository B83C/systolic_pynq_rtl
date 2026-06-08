// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VSA_WRAPPER_AXI_CTRL__SYMS_H_
#define VERILATED_VSA_WRAPPER_AXI_CTRL__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vsa_wrapper_axi_ctrl.h"

// INCLUDE MODULE CLASSES
#include "Vsa_wrapper_axi_ctrl___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vsa_wrapper_axi_ctrl__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vsa_wrapper_axi_ctrl* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vsa_wrapper_axi_ctrl___024root TOP;

    // CONSTRUCTORS
    Vsa_wrapper_axi_ctrl__Syms(VerilatedContext* contextp, const char* namep, Vsa_wrapper_axi_ctrl* modelp);
    ~Vsa_wrapper_axi_ctrl__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
