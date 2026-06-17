"""Benchmark: optimized batching — send all positions in one DMA call."""
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
rng = np.random.default_rng(42)

def bench(Cout, Cin, N):
    weight = rng.integers(-5, 5, (Cout, Cin, 1, 1), dtype=np.int8)
    bias = rng.integers(-100, 100, Cout, dtype=np.int32)
    act = rng.integers(-128, 127, (1, N, Cin), dtype=np.int8)

    Cp = ((Cout + T - 1) // T) * T
    Kp = ((Cin  + T - 1) // T) * T

    # A
    w_2d = weight.reshape(Cout, Cin)
    w_pad = np.zeros((Cp, Kp), dtype=np.int8)
    w_pad[:Cout, :Cin] = w_2d
    A_bm = np.ascontiguousarray(
        w_pad.reshape(Cp//T, T, Kp//T, T).transpose(0, 2, 1, 3))
    ab = allocate(shape=(Cp*Kp,), dtype=np.int8)
    ab[:] = A_bm.reshape(-1)

    # B: all positions in one buffer
    Np = ((N + T - 1) // T) * T
    b_pad = np.zeros((Np, Kp), dtype=np.int8)
    b_pad[:N, :Cin] = act.reshape(N, Cin)
    bb = allocate(shape=(Np*Kp,), dtype=np.int8)
    bb[:] = b_pad.reshape(-1)

    ob = allocate(shape=(Np*Cp,), dtype=np.int8)

    fb_cnt = Kp // T - 1
    mmio.write(SaReg.FB_CNT, fb_cnt)
    mmio.write(SaReg.A_LOOP_END, Cp * Kp // T - 1)
    mmio.write(SaReg.C_LOOP_END, Cp - 1)
    m.configure_chlast(mmio_ch, cfg_channels=Kp, repeat_cnt=Kp//T, bypass=0)
    m.configure_quantizer(mmio_q, mul_q=1, shift=0, zp_out=0)
    m.configure_tiled(mmio_tc, cfg_channels=Cp)

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    # B stream all at once
    dma.recvchannel.start(); dma.sendchannel.start()
    t0 = time.perf_counter()
    dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)
    dt = time.perf_counter() - t0

    hw = ob.reshape(Np, Cp)[:N, :Cout].copy()
    sw = (act.reshape(N,Cin).astype(np.int32) @ weight.reshape(Cout,Cin).astype(np.int32).T
          + bias).clip(-128,127).astype(np.int8)
    d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))

    ab.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return dt, d

for (Co, Ci, N) in [(16,16,8), (16,16,64), (16,16,128), (16,16,256), (16,16,512)]:
    dt, d = bench(Co, Ci, N)
    print(f"  {Co}x{Ci} N={N:4d}: {dt*1000:.3f}ms  maxdiff={d}  {'PASS' if d<=1 else 'FAIL'}")
