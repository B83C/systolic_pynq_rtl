// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing declarations

#include "verilated_fst_c.h"


void Vconv_sa_tb___024root__traceDeclTypesSub0(VerilatedFst* tracep) {
    {
        const char* __VenumItemNames[]
        = {"IDLE", "KERNEL", "BLOAD", "FILL", "COMPUTE", 
                                "FINISH"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101"};
        tracep->declDTypeEnum(1, "conv_sa.state_t", 6, 3, __VenumItemNames, __VenumItemValues);
    }
}

void Vconv_sa_tb___024root__trace_decl_types(VerilatedFst* tracep) {
    Vconv_sa_tb___024root__traceDeclTypesSub0(tracep);
}
