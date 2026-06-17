"""Minimal test: 8x8 first, then 16x16, then 8x8 again."""
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

def wait_state(target=0):
    for _ in range(10000):
        if (mmio.read(SaReg.STATE) & 0x3) == target:
            return True
    return False

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
    if cb is not None:
        mmio.write(SaReg.C_LOAD, 1)
        mmio_ch.write(ChlastReg.BYPASS, 1)
        dma.sendchannel.start(); dma.sendchannel.transfer(cb); dma.sendchannel.wait()
        mmio_ch.write(ChlastReg.BYPASS, 0)

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    dma.recvchannel.start(); dma.sendchannel.start()
    dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)

    r = ob.reshape(N, Co).copy()
    ab.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return r

rng = np.random.default_rng(42)

# Test 0: 8×8 eye-like (from step 7)
A0 = np.zeros((8,8), dtype=np.int8)
A0[0,:4] = [1,2,3,4]; A0[1,:4] = [5,6,7,8]
B0 = np.zeros((8,8), dtype=np.int8)
B0[0,0] = 1; B0[1,1] = 1
hw0 = run(A0, B0, 0, 7, 7, 8, 1)  # c_le=7 (Co-1) matching bench
sw0 = (B0.astype(np.int32) @ A0.astype(np.int32).T).clip(-128,127).astype(np.int8)
d0 = np.max(np.abs(hw0.astype(np.int16)-sw0.astype(np.int16)))
print(f"Test 0: 8x8 eye-like:   maxdiff={d0}  {'PASS' if d0==0 else 'FAIL'}")
print(f"  hw[0,:4]={hw0[0,:4]}  (expect [1,5,0,0])")

# Test 1: 8×8, FIRST
A1 = rng.integers(-5, 5, (8, 8), dtype=np.int8)
B1 = rng.integers(-5, 5, (16, 8), dtype=np.int8)
hw1 = run(A1, B1, 0, 7, 7, 8, 1)  # c_le=7
sw1 = (B1.astype(np.int32) @ A1.astype(np.int32).T).clip(-128,127).astype(np.int8)
d1 = np.max(np.abs(hw1.astype(np.int16)-sw1.astype(np.int16)))
print(f"Test 1: 8x8 random:     maxdiff={d1}  {'PASS' if d1==0 else 'FAIL'}")
# Debug: show first row
acc1 = B1.astype(np.int32) @ A1.astype(np.int32).T
print(f"  hw[0,:]={hw1[0,:]}")
print(f"  sw[0,:]={sw1[0,:]}")
print(f"  acc[0,:]={acc1[0,:]}")

# Test 2: 16×16  
A2 = rng.integers(-5, 5, (16, 16), dtype=np.int8)
B2 = rng.integers(-5, 5, (32, 16), dtype=np.int8)
hw2 = run(A2, B2, 1, 31, 15, 16, 2)  # c_le=15 (Co-1)
sw2 = (B2.astype(np.int32) @ A2.astype(np.int32).T).clip(-128,127).astype(np.int8)
d2 = np.max(np.abs(hw2.astype(np.int16)-sw2.astype(np.int16)))
print(f"Test 2: 16x16 (second):  maxdiff={d2}  {'PASS' if d2==0 else 'FAIL'}")

# Test 3: 8×8 AGAIN
A3 = rng.integers(-5, 5, (8, 8), dtype=np.int8)
B3 = rng.integers(-5, 5, (16, 8), dtype=np.int8)
hw3 = run(A3, B3, 0, 7, 7, 8, 1)  # c_le=7
sw3 = (B3.astype(np.int32) @ A3.astype(np.int32).T).clip(-128,127).astype(np.int8)
d3 = np.max(np.abs(hw3.astype(np.int16)-sw3.astype(np.int16)))
print(f"Test 3: 8x8 (third):     maxdiff={d3}  {'PASS' if d3==0 else 'FAIL'}")
