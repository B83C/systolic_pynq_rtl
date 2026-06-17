"""Step-by-step HW test — validates each stage independently."""
import sys, traceback

def step(n, msg):
    print(f"\n--- Step {n}: {msg} ---", flush=True)

# Step 1: Python + numpy
step(1, "Python + numpy")
import numpy as np
print(f"  numpy {np.__version__}")

# Step 2: Import pynq modules
step(2, "pynq imports")
from pynq import Overlay, allocate, MMIO
print("  pynq OK")

# Step 3: Import systolic_regs
step(3, "systolic_regs imports")
from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg
print(f"  SaReg: STATE=0x{SaReg.STATE:02X}")

# Step 4: Load overlay
step(4, "Load overlay")
ol = Overlay("systolic.bit")
print(f"  IPs: {list(ol.ip_dict.keys())}")

# Step 5: Create MMIO objects
step(5, "Create MMIO objects")
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)
print("  MMIO objects created")
state = mmio.read(SaReg.STATE)
size = mmio.read(SaReg.SIZE)
print(f"  SA state={state} size={size}")

# Step 6: DMA
step(6, "DMA objects")
dma = ol.axi_dma_0
print(f"  send methods: {[m for m in dir(dma.sendchannel) if not m.startswith('_')]}")
print(f"  recv methods: {[m for m in dir(dma.recvchannel) if not m.startswith('_')]}")

# Step 7: Simple SA compute (4×4 weight, 4×4 act, no bias, no quant)
step(7, "SA compute: 8×8 matmul (no bias)")
m = SystolicMap()

A = np.array([[1,2,3,4,0,0,0,0],
              [5,6,7,8,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0]], dtype=np.int8)
B = np.array([[1,0,0,0,0,0,0,0],
              [0,1,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0],
              [0,0,0,0,0,0,0,0]], dtype=np.int8)

def tbm(M):
    return np.ascontiguousarray(
        M.reshape(M.shape[0]//8, 8, M.shape[1]//8, 8).transpose(0, 2, 1, 3))

def read_state(msg=""):
    st = mmio.read(SaReg.STATE) & 0x3
    sts = mmio.read(SaReg.STATUS)
    s = {0:"IDLE",1:"LOAD_A",2:"LOAD_B",3:"LOAD_C"}.get(st, "?")
    if st != 0:
        print(f"  SA state={st}({s}) status=0x{sts:x} {msg}")

def wait_idle():
    for _ in range(1000):
        if (mmio.read(SaReg.STATE) & 0x3) == 0:
            return True
    return False

def sa_compute(a_buf, b_buf, o_buf, fb_cnt, a_loop_end, c_loop_end,
               cfg_ch, rpt, mul_q=1, shift=0, zp_out=0,
               c_buf=None):
    """Run SA: A_LOAD → (C_LOAD) → B stream."""
    m = SystolicMap()
    mmio.write(SaReg.FB_CNT,     fb_cnt)
    mmio.write(SaReg.A_LOOP_END, a_loop_end)
    mmio.write(SaReg.C_LOOP_END, c_loop_end)
    m.configure_chlast(mmio_ch,   cfg_channels=cfg_ch, repeat_cnt=rpt, bypass=0)
    m.configure_quantizer(mmio_q,  mul_q=mul_q, shift=shift, zp_out=zp_out)
    m.configure_tiled(mmio_tc,    cfg_channels=cfg_ch)

    if c_buf is not None:
        mmio.write(SaReg.C_LOAD, 1)
        mmio_ch.write(ChlastReg.BYPASS, 1)
        dma.sendchannel.start(); dma.sendchannel.transfer(c_buf); dma.sendchannel.wait()
        mmio_ch.write(ChlastReg.BYPASS, 0)

    mmio.write(SaReg.A_LOAD, 1)
    mmio_ch.write(ChlastReg.BYPASS, 1)
    dma.sendchannel.start(); dma.sendchannel.transfer(a_buf); dma.sendchannel.wait()
    mmio_ch.write(ChlastReg.BYPASS, 0)

    dma.recvchannel.start(); dma.sendchannel.start()
    dma.recvchannel.transfer(o_buf); dma.sendchannel.transfer(b_buf)

wait_idle(); read_state("pre-step7")

# ── Step 7 ──
ab = allocate(shape=(64,), dtype=np.int8)
bb = allocate(shape=(64,), dtype=np.int8)
ob = allocate(shape=(64,), dtype=np.int8)
ab[:] = tbm(A).reshape(-1)
bb[:] = B.reshape(-1)

sa_compute(ab, bb, ob, fb_cnt=0, a_loop_end=7, c_loop_end=0, cfg_ch=8, rpt=1)
wait_idle(); read_state("post-step7")

hw = ob.reshape(8, 8).copy()
# SA computes B @ A.T → row0 = B[0] @ A.T = [1,0,0,...] @ rows of A = A col 0 = [1,5,...]
print(f"  output top-left 2×2: [{hw[0,0]},{hw[0,1]}],[{hw[1,0]},{hw[1,1]}]")
expected_row0 = A[:,0]  # = [1,5,0,0,...]
ok7 = np.array_equal(hw[0,:4], expected_row0[:4])
print(f"  expected first 4 of row0: {expected_row0[:4]}")
print(f"  {'PASS' if ok7 else 'FAIL'}")

ab.freebuffer(); bb.freebuffer(); ob.freebuffer()

# Step 8: Full pointwise 16×16
step(8, "SA pointwise 16×16, no bias, shift=0")
A8 = np.random.default_rng(123).integers(-5, 5, (16, 16), dtype=np.int8)
B8 = np.random.default_rng(456).integers(-5, 5, (32, 16), dtype=np.int8)

wait_idle(); read_state("pre-step8")

ab8 = allocate(shape=(256,), dtype=np.int8)
bb8 = allocate(shape=(512,), dtype=np.int8)
ob8 = allocate(shape=(512,), dtype=np.int8)
ab8[:] = tbm(A8).reshape(-1)
bb8[:] = B8.reshape(-1)

sa_compute(ab8, bb8, ob8, fb_cnt=1, a_loop_end=31, c_loop_end=0, cfg_ch=16, rpt=2)
wait_idle(); read_state("post-step8")

hw8 = ob8.reshape(32, 16).copy()
sw8 = (B8.astype(np.int32) @ A8.astype(np.int32).T).clip(-128,127).astype(np.int8)
d8 = np.max(np.abs(hw8.astype(np.int16)-sw8.astype(np.int16)))
print(f"  maxdiff={d8}  {'PASS' if d8==0 else 'FAIL'}")

ab8.freebuffer(); bb8.freebuffer(); ob8.freebuffer()

# Step 9: With bias
step(9, "SA pointwise 8×8, with bias")
bias = np.array([10, -20, 30, -40, 50, -60, 70, -80], dtype=np.int32)
A9 = np.random.default_rng(789).integers(-5, 5, (8, 8), dtype=np.int8)
B9 = np.random.default_rng(101).integers(-5, 5, (16, 8), dtype=np.int8)

ab9 = allocate(shape=(64,), dtype=np.int8)
bb9 = allocate(shape=(128,), dtype=np.int8)
ob9 = allocate(shape=(128,), dtype=np.int8)
ab9[:] = tbm(A9).reshape(-1)
bb9[:] = B9.reshape(-1)
cb9 = allocate(shape=(8,), dtype=np.int64)
wait_idle(); read_state("pre-step9-no-bias")
# Read STATUS to clear underflow, then soft-reset
mmio.read(SaReg.STATUS)
mmio.write(SaReg.RST_INDEX, 0)

sa_compute(ab9, bb9, ob9, fb_cnt=0, a_loop_end=7, c_loop_end=0, cfg_ch=8, rpt=1)
wait_idle(); read_state("post-step9-no-bias")
hw9a = ob9.reshape(16, 8).copy()
sw9a = (B9.astype(np.int32) @ A9.astype(np.int32).T).clip(-128,127).astype(np.int8)
d9a = np.max(np.abs(hw9a.astype(np.int16)-sw9a.astype(np.int16)))
print(f"  8×8 no bias:           maxdiff={d9a}  {'PASS' if d9a==0 else 'FAIL'}")

# Then test WITH bias (same buffers reused)
c32 = cb9.view(np.int32).reshape(-1,2)
c32[:,:] = 0
for i in range(8): c32[i,0] = np.int32(bias[i])
cb9.sync_to_device()

wait_idle(); read_state("pre-step9-with-bias")

sa_compute(ab9, bb9, ob9, fb_cnt=0, a_loop_end=7, c_loop_end=7, cfg_ch=8, rpt=1, c_buf=cb9)
wait_idle(); read_state("post-step9-with-bias")

hw9b = ob9.reshape(16, 8).copy()
sw9b = (B9.astype(np.int32) @ A9.astype(np.int32).T + bias).clip(-128,127).astype(np.int8)
d9b = np.max(np.abs(hw9b.astype(np.int16)-sw9b.astype(np.int16)))
print(f"  8×8 + bias:            maxdiff={d9b}  {'PASS' if d9b==0 else 'FAIL'}")
print(f"  hw[0,:]={hw9b[0,:]}")
print(f"  sw[0,:]={sw9b[0,:]}")

ab9.freebuffer(); bb9.freebuffer(); ob9.freebuffer(); cb9.freebuffer()

# Step 10: With quantizer
step(10, "SA pointwise 16×16, with quantizer (mul_q=35995, shift=23, zp=14)")
A10 = np.random.default_rng(202).integers(-5, 5, (16, 16), dtype=np.int8)
B10 = np.random.default_rng(303).integers(-5, 5, (32, 16), dtype=np.int8)

ab10 = allocate(shape=(256,), dtype=np.int8)
bb10 = allocate(shape=(512,), dtype=np.int8)
ob10 = allocate(shape=(512,), dtype=np.int8)
ab10[:] = tbm(A10).reshape(-1)
bb10[:] = B10.reshape(-1)

wait_idle(); read_state("pre-step10")

sa_compute(ab10, bb10, ob10, fb_cnt=1, a_loop_end=31, c_loop_end=0, cfg_ch=16, rpt=2,
           mul_q=35995, shift=23, zp_out=14)
wait_idle(); read_state("post-step10")

hw10 = ob10.reshape(32, 16).copy()
acc10 = B10.astype(np.int64) @ A10.astype(np.int64).T
sw10 = ((acc10 * 35995) >> 23).astype(np.int32) + 14
sw10 = np.clip(sw10, -128, 127).astype(np.int8)
d10 = np.max(np.abs(hw10.astype(np.int16)-sw10.astype(np.int16)))
print(f"  maxdiff={d10}  {'PASS' if d10==0 else 'FAIL'}")

ab10.freebuffer(); bb10.freebuffer(); ob10.freebuffer()

print(f"\n{'='*40}")
print(f"Steps 1-7: basic SA matmul {'PASS' if ok7 else 'FAIL'}")
print(f"Step 8: 16×16 pointwise {'PASS' if d8==0 else 'FAIL'}")
print(f"Step 9a: 8×8 no bias      {'PASS' if d9a==0 else 'FAIL'}")
print(f"Step 9b: 8×8 + bias        {'PASS' if d9b==0 else 'FAIL'}")
print(f"Step 10: quant             {'PASS' if d10==0 else 'FAIL'}")
