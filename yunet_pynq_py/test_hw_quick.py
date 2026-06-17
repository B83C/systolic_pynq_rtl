"""Quick HW test: bench_sa.py-style pointwise."""
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

def run_one(A, B, mul_q=1, shift=0, zp_out=0, bias=None):
    """SA compute: B @ A^T, quantized. transfer() is blocking — no .wait() needed."""
    Co, Ci = A.shape
    N = B.shape[0]
    ab = allocate(shape=(Co*Ci,), dtype=np.int8)
    bb = allocate(shape=( N*Ci,), dtype=np.int8)
    ob = allocate(shape=( N*Co,), dtype=np.int8)
    ab[:] = tbm(A).reshape(-1)
    bb[:] = B.reshape(-1)

    mmio.write(SaReg.FB_CNT,      Ci//8 - 1)
    mmio.write(SaReg.A_LOOP_END,  Co*Ci//8 - 1)
    mmio.write(SaReg.C_LOOP_END,  Co - 1 if bias is not None else 0)
    m.configure_chlast(mmio_ch,   cfg_channels=Ci, repeat_cnt=Ci//8, bypass=0)
    m.configure_quantizer(mmio_q,  mul_q=mul_q, shift=shift, zp_out=zp_out)
    m.configure_tiled(mmio_tc,    cfg_channels=Co)

    if bias is not None:
        cb = allocate(shape=(Co,), dtype=np.int64)
        c32 = cb.view(np.int32).reshape(-1, 2)
        c32[:,:] = 0
        for i in range(Co):
            c32[i,0] = np.int32(bias[i])
        mmio.write(SaReg.C_LOAD, 1)
        mmio_ch.write(ChlastReg.BYPASS, 1)
        dma.sendchannel.start(); dma.sendchannel.transfer(cb)
        mmio_ch.write(ChlastReg.BYPASS, 0)
        cb.freebuffer()

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(ab)
    mmio_ch.write(ChlastReg.BYPASS, 0)

    dma.recvchannel.start(); dma.sendchannel.start()
    dma.recvchannel.transfer(ob); dma.sendchannel.transfer(bb)

    out = ob.reshape(N, Co).copy()
    ab.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return out

rng = np.random.default_rng(42)

# Test 1: 16x16 × 64x16, no bias
A = rng.integers(-5, 5, (16, 16), dtype=np.int8)
B = rng.integers(-5, 5, (64, 16), dtype=np.int8)
hw = run_one(A, B)
sw = (B.astype(np.int32) @ A.astype(np.int32).T).clip(-128,127).astype(np.int8)
d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))
print(f'  16×16 × 64×16:           maxdiff={d}  {"PASS" if d==0 else "FAIL"}')

# Test 2: 8×8 × 32×8, with bias
A2 = rng.integers(-5, 5, (8, 8), dtype=np.int8)
B2 = rng.integers(-5, 5, (32, 8), dtype=np.int8)
bias = rng.integers(-100, 100, 8, dtype=np.int32)
hw2 = run_one(A2, B2, bias=bias)
sw2 = (B2.astype(np.int32) @ A2.astype(np.int32).T + bias).clip(-128,127).astype(np.int8)
d2 = np.max(np.abs(hw2.astype(np.int16)-sw2.astype(np.int16)))
print(f'  8×8 × 32×8 + bias:       maxdiff={d2}  {"PASS" if d2==0 else "FAIL"}')

# Test 3: 16×16 × 32×16, with quantizer
A3 = rng.integers(-5, 5, (16, 16), dtype=np.int8)
B3 = rng.integers(-5, 5, (32, 16), dtype=np.int8)
hw3 = run_one(A3, B3, mul_q=35995, shift=23, zp_out=14)
sw3 = np.clip(np.floor(B3.astype(np.int32) @ A3.astype(np.int32).T * 35995 / (1<<23)).astype(np.int32) + 14, -128, 127).astype(np.int8)
d3 = np.max(np.abs(hw3.astype(np.int16)-sw3.astype(np.int16)))
print(f'  16×16 × 32×16 + quant:   maxdiff={d3}  {"PASS" if d3==0 else "FAIL"}')

print()
print("All tests done.")
