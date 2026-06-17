"""Benchmark NEON depthwise vs SW."""
import numpy as np, time, subprocess
from pynq import allocate
from yunet_sw import depthwise3x3

H, W, C = 160, 160, 64
rng = np.random.default_rng(42)
inp = rng.integers(-128, 127, (H, W, C), dtype=np.int8)
wt = rng.integers(-5, 5, (C, 1, 3, 3), dtype=np.int8)
bias = rng.integers(-1000, 1000, C, dtype=np.int32)

zp_in = -128; zp_w = 0; mul_q = 1; shift = 0; zp_out = 0

ib = allocate(shape=(H*W*C,), dtype=np.int8); ib[:] = inp.reshape(-1)
# Rearrange weights from (C,3,3) to (3,3,C) for NEON-friendly access
w_rearr = np.zeros((3,3,C), dtype=np.int8)
for ky in range(3):
    for kx in range(3):
        w_rearr[ky,kx,:] = wt[:,0,ky,kx]
wb = allocate(shape=(3*3*C,), dtype=np.int8); wb[:] = w_rearr.reshape(-1)
bb = allocate(shape=(C,), dtype=np.int32); bb[:] = bias
ob = allocate(shape=(H*W*C,), dtype=np.int8)

r = subprocess.run(["./dw3x3",
    hex(ib.device_address), hex(wb.device_address), hex(bb.device_address), hex(ob.device_address),
    str(H), str(W), str(C), str(zp_in), str(zp_w), str(mul_q), str(shift), str(zp_out)],
    capture_output=True, text=True, timeout=30)

# Timing: 3 runs
ts = []
for _ in range(3):
    t0 = time.perf_counter()
    subprocess.run(["./dw3x3",
        hex(ib.device_address), hex(wb.device_address), hex(bb.device_address), hex(ob.device_address),
        str(H), str(W), str(C), str(zp_in), str(zp_w), str(mul_q), str(shift), str(zp_out)],
        capture_output=True, timeout=30)
    ts.append(time.perf_counter() - t0)
t_cpp = np.median(ts)

hw = ob.view(np.int8).reshape(H, W, C).copy()
sw = depthwise3x3(inp, wt, bias, zp_in, zp_w, 1.0, zp_out)
d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))

t0 = time.perf_counter()
for _ in range(3):
    depthwise3x3(inp, wt, bias, zp_in, zp_w, 1.0, zp_out)
t_sw = (time.perf_counter() - t0) / 3

print(f"NEON DW: {t_cpp*1000:.1f}ms  SW: {t_sw*1000:.1f}ms  speedup: {t_sw/t_cpp:.1f}x  maxdiff={d}")
ib.freebuffer(); wb.freebuffer(); bb.freebuffer(); ob.freebuffer()
