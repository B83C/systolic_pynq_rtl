"""Register map for the per-module AXI-Lite config slaves on the
systolic-array design.  Each slave is a separate 4 KB address range
mapped by Vivado:

  sa_wrapper_axi_ctrl_0  @ 0x4000_0000
  chlast_to_tiled_0      @ 0x4000_1000
  quantizer_wrapper_0    @ 0x4000_2000
  tiled_to_chlast_0      @ 0x4000_3000
"""

from enum import IntEnum
from dataclasses import dataclass

# AXI-Lite address range for each slave (4 KB)
AXI_RANGE = 0x1000

# -----------------------------------------------------------------------
# quantizer_wrapper_0  (offsets are 4-byte aligned, 3-bit address space)
# -----------------------------------------------------------------------
class QuantReg(IntEnum):
    MUL_Q  = 0x0   # [15:0] UINT16 quantized multiplier
    SHIFT  = 0x4   # [4:0]  UINT5  right-shift amount
    ZP_OUT = 0x8   # [7:0]  INT8   output zero-point (INT8 = -128..127)

# -----------------------------------------------------------------------
# chlast_to_tiled_0
# -----------------------------------------------------------------------
class ChlastReg(IntEnum):
    CFG_CH  = 0x0   # [$clog2(MAX_CHANNELS+1)-1:0]  number of channels
    RPT     = 0x4   # [$clog2(MAX_REPLAY_CNT+1)-1:0] replay count
    BYPASS  = 0x8   # [0]   0=normal, 1=passthrough

# -----------------------------------------------------------------------
# tiled_to_chlast_0
# -----------------------------------------------------------------------
class TiledReg(IntEnum):
    CFG_CH  = 0x0   # [$clog2(MAX_CHANNELS+1)-1:0]  number of channels
    BYPASS  = 0x4   # [0]   0=normal, 1=passthrough

# -----------------------------------------------------------------------
# Base addresses (override these if your block design differs)
# -----------------------------------------------------------------------
@dataclass
class SystolicMap:
    sa_wrapper_base: int = 0x4000_0000
    chlast_base:     int = 0x4000_1000
    quant_base:      int = 0x4000_2000
    tiled_base:      int = 0x4000_3000
    range:           int = AXI_RANGE

    def configure_quantizer(self, mmio, mul_q=1, shift=0, zp_out=0, zp_in=None):
        """Write quantizer config.  zp_in lives in the SA wrapper (separate)."""
        mmio.write(QuantReg.MUL_Q,  int(mul_q)  & 0xFFFF)
        mmio.write(QuantReg.SHIFT,  int(shift)  & 0x1F)
        mmio.write(QuantReg.ZP_OUT, int(zp_out) & 0xFF)
        if zp_in is not None:
            # SA wrapper's REG_ZP_IN at 0x3C
            from pynq import MMIO
            sa = MMIO(self.sa_wrapper_base, self.range)
            sa.write(0x3C, int(zp_in) & 0xFF)

    def configure_chlast(self, mmio, cfg_channels, repeat_cnt=1, bypass=0):
        mmio.write(ChlastReg.CFG_CH,  int(cfg_channels) & 0x7F)
        mmio.write(ChlastReg.RPT,     int(repeat_cnt)  & 0x1F)
        mmio.write(ChlastReg.BYPASS,  int(bypass)      & 0x1)

    def configure_tiled(self, mmio, cfg_channels, bypass=0):
        mmio.write(TiledReg.CFG_CH,  int(cfg_channels) & 0x7F)
        mmio.write(TiledReg.BYPASS,  int(bypass)      & 0x1)
