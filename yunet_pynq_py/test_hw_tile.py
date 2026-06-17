"""Test HW pointwise with spatial tiling."""
import numpy as np
from pynq import Overlay, allocate, MMIO
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

ol = Overlay("systolic.bit")
dma = ol.axi_dma_0
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)

from yunet_pynq import hw_pointwise

rng = np.random.default_rng(42)

# Test 8×8, N=16 (2 spatial groups)
Cout, Cin, H, W = 8, 8, 4, 4
weight = rng.integers(-5, 5, (Cout, Cin, 1, 1), dtype=np.int8)
bias = rng.integers(-100, 100, Cout, dtype=np.int32)
act = rng.integers(-5, 5, (H, W, Cin), dtype=np.int8)

hw = hw_pointwise(mmio, mmio_ch, mmio_q, mmio_tc, dma,
                   act, weight, bias, zp_in=0, zp_w=0, mul_float=0.01, zp_out=42)

# SW reference
from yunet_pynq import sw_pointwise
sw = sw_pointwise(act, weight, bias, zp_in=0, zp_w=0, mul=0.01, zp_out=42)

d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))
print(f"8×8 N=16: maxdiff={d}  {'PASS' if d<=1 else 'FAIL'}")

# Test 16×16, N=32 (4 spatial groups)
Cout2, Cin2, H2, W2 = 16, 16, 4, 8
weight2 = rng.integers(-5, 5, (Cout2, Cin2, 1, 1), dtype=np.int8)
bias2 = rng.integers(-100, 100, Cout2, dtype=np.int32)
act2 = rng.integers(-5, 5, (H2, W2, Cin2), dtype=np.int8)

hw2 = hw_pointwise(mmio, mmio_ch, mmio_q, mmio_tc, dma,
                    act2, weight2, bias2, zp_in=0, zp_w=0, mul_float=0.005, zp_out=14)
sw2 = sw_pointwise(act2, weight2, bias2, zp_in=0, zp_w=0, mul=0.005, zp_out=14)
d2 = np.max(np.abs(hw2.astype(np.int16)-sw2.astype(np.int16)))
print(f"16×16 N=32: maxdiff={d2}  {'PASS' if d2<=1 else 'FAIL'}")
