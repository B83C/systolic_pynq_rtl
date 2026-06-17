"""Benchmark first pw layer: HW vs SW."""
import numpy as np, time
from pynq import Overlay, allocate, MMIO
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

ol = Overlay("systolic.bit")
dma = ol.axi_dma_0
m = SystolicMap()
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)

from yunet_pynq import hw_pointwise, sw_pointwise

# First pw layer params (from build_stage_table)
H, W = 320, 320
Cin, Cout = 16, 16
rng = np.random.default_rng(123)
weight = rng.integers(-5, 5, (Cout, Cin, 1, 1), dtype=np.int8)
bias = rng.integers(-1000, 1000, Cout, dtype=np.int32)

act = rng.integers(-128, 127, (H, W, Cin), dtype=np.int8)

zp_in = -128; zp_w = 0; zp_out = 14; mul = 0.004290928

print(f"Pointwise: {Cin}→{Cout}, {H}x{W}")
print()

# Warmup
hw0 = hw_pointwise(mmio, mmio_ch, mmio_q, mmio_tc, dma,
                   act, weight, bias, zp_in, zp_w, mul, zp_out)
sw0 = sw_pointwise(act, weight, bias, zp_in, zp_w, mul, zp_out)
d = np.max(np.abs(hw0.astype(np.int16)-sw0.astype(np.int16)))
print(f"Warmup: maxdiff={d} {'PASS' if d<=1 else 'FAIL'}")

# Benchmark HW (3 runs)
t0 = time.perf_counter()
for _ in range(3):
    hw = hw_pointwise(mmio, mmio_ch, mmio_q, mmio_tc, dma,
                      act, weight, bias, zp_in, zp_w, mul, zp_out)
t_hw = (time.perf_counter() - t0) / 3

# Benchmark SW (1 run: slow on ARM)
t0 = time.perf_counter()
sw = sw_pointwise(act, weight, bias, zp_in, zp_w, mul, zp_out)
t_sw = time.perf_counter() - t0

d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))
spd = t_sw / t_hw if t_hw > 0 else float('inf')
ops = 2 * H * W * Cin * Cout
print(f"HW: {t_hw*1000:.2f}ms ({ops/t_hw/1e6:.1f} MOPS)")
print(f"SW: {t_sw*1000:.2f}ms ({ops/t_sw/1e6:.1f} MOPS)")
print(f"Speedup: {spd:.1f}x")
print(f"Maxdiff: {d} (ideally <=1)")
