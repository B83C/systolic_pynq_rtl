"""Benchmark: SA tile throughput (excludes A_LOAD overhead)."""
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

T = 8
Cout, Cin = 16, 16
rng = np.random.default_rng(42)
weight = rng.integers(-5, 5, (Cout, Cin, 1, 1), dtype=np.int8)
bias = rng.integers(-100, 100, Cout, dtype=np.int32)

from yunet_pynq import to_block_major

# Pre-load A (weight) and C (bias) once
def init_sa():
    Cp = ((Cout + T - 1) // T) * T
    Kp = ((Cin  + T - 1) // T) * T
    w_2d = weight.reshape(Cout, Cin)
    w_pad = np.zeros((Cp, Kp), dtype=np.int8)
    w_pad[:Cout, :Cin] = w_2d
    A_bm = to_block_major(w_pad, T, T)
    ab = allocate(shape=(Cp*Kp,), dtype=np.int8)
    ab[:] = np.ascontiguousarray(A_bm).reshape(-1)
    ab.sync_to_device()

    fb_cnt = Kp // T - 1
    mmio.write(SaReg.FB_CNT, fb_cnt)
    mmio.write(SaReg.A_LOOP_END, Cp * Kp // T - 1)
    mmio.write(SaReg.C_LOOP_END, Cp - 1)
    mmio.write(SaReg.ZP_IN, 0)
    m.configure_chlast(mmio_ch, cfg_channels=Kp, repeat_cnt=Kp//T, bypass=0)
    m.configure_quantizer(mmio_q, mul_q=1, shift=0, zp_out=0)
    m.configure_tiled(mmio_tc, cfg_channels=Cp)

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    cb = allocate(shape=(Cp,), dtype=np.int64)
    c32 = cb.view(np.int32).reshape(-1,2); c32[:,:] = 0
    for i in range(Cout): c32[i,0] = np.int32(bias[i])
    cb.sync_to_device()
    mmio.write(SaReg.C_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(cb); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    bb = allocate(shape=(T*Kp,), dtype=np.int8)
    ob = allocate(shape=(T*Cp,), dtype=np.int8)
    ab.freebuffer(); cb.freebuffer()
    return bb, ob

bb, ob = init_sa()

def run_tile(act_tile):
    """Stream one spatial tile (T positions × Cin). act_tile: (T, Cin)."""
    bb[:] = act_tile.reshape(-1)
    bb.sync_to_device()
    dma.recvchannel.start(); dma.sendchannel.start()
    dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)
    return ob.reshape(T, -1).copy()

# Benchmark: measure only the tile streaming (no A/C load)
act_tile = rng.integers(-128, 127, (T, Cin), dtype=np.int8)

# Warmup
hw = run_tile(act_tile)

times = []
for i in range(10):
    t0 = time.perf_counter()
    hw = run_tile(act_tile)
    times.append(time.perf_counter() - t0)

avg = np.mean(times) * 1000
print(f"Tile throughput: {avg:.3f}ms per tile  ({1000/avg:.0f} tiles/sec)")
print()

# Estimate for full pipeline
for H, W in [(8,8), (40,40), (80,80), (160,160), (320,320)]:
    n = (H * W) // T
    est = n * avg / 1000
    print(f"  {H:4d}x{W:4d} ({n:5d} tiles): ~{est:.2f}s per pw layer")

bb.freebuffer(); ob.freebuffer()
