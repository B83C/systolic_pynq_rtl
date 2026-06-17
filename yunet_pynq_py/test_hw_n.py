"""Test if N=8 vs N=16 makes the difference for 8x8."""
import numpy as np
from pynq import Overlay, allocate, MMIO
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

ol = Overlay("systolic.bit")
m = SystolicMap()
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)
dma = ol.axi_dma_0

def tbm(M):
    return np.ascontiguousarray(
        M.reshape(M.shape[0]//8, 8, M.shape[1]//8, 8).transpose(0, 2, 1, 3))

def run(A, B, fb_cnt, a_le, c_le, cfg, rpt, mq=1, sh=0, zo=0, cb=None):
    Co, Ci = A.shape; N = B.shape[0]
    ab = allocate(shape=(Co*Ci,), dtype=np.int8)
    bb = allocate(shape=( N*Ci,), dtype=np.int8)
    ob = allocate(shape=( N*Co,), dtype=np.int8)
    ab[:] = tbm(A).reshape(-1); bb[:] = B.reshape(-1)
    mmio.write(SaReg.FB_CNT, fb_cnt); mmio.write(SaReg.A_LOOP_END, a_le)
    mmio.write(SaReg.C_LOOP_END, c_le)
    m.configure_chlast(mmio_ch, cfg_channels=cfg, repeat_cnt=rpt, bypass=0)
    m.configure_quantizer(mmio_q, mul_q=mq, shift=sh, zp_out=zo)
    m.configure_tiled(mmio_tc, cfg_channels=cfg)

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    dma.recvchannel.start(); dma.sendchannel.start()
    dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)

    r = ob.reshape(N, Co).copy()
    ab.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return r

np.random.seed(1234)  # same as bench

# Test with N=8 (matches bench)
A = np.random.randint(-5,5,(8,8),dtype=np.int8)
B = np.random.randint(-5,5,(8,8),dtype=np.int8)
hw = run(A, B, 0, 7, 7, 8, 1)
sw = (B.astype(np.int32) @ A.astype(np.int32).T).clip(-128,127).astype(np.int8)
d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))
print(f"N=8: maxdiff={d} {'PASS' if d==0 else 'FAIL'}")

# Test with N=16
B2 = np.random.randint(-5,5,(16,8),dtype=np.int8)
hw2 = run(A, B2, 0, 7, 7, 8, 1)
sw2 = (B2.astype(np.int32) @ A.astype(np.int32).T).clip(-128,127).astype(np.int8)
d2 = np.max(np.abs(hw2.astype(np.int16)-sw2.astype(np.int16)))
print(f"N=16: maxdiff={d2} {'PASS' if d2==0 else 'FAIL'}")
