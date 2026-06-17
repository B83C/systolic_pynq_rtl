"""Test NEON conv3x3 vs SW."""
import numpy as np, time, subprocess
from pynq import allocate
from yunet_sw import conv3x3

H, W, C_in, C_out = 640, 640, 3, 16
rng = np.random.default_rng(42)
inp = rng.integers(-128, 127, (H, W, C_in), dtype=np.int8)
wt = rng.integers(-5, 5, (C_out, C_in, 3, 3), dtype=np.int8)
bias = rng.integers(-1000, 1000, C_out, dtype=np.int32)

zp_in = -128; zp_w = 0; mul = 1.0; zp_out = 0

ib = allocate(shape=(H*W*C_in,), dtype=np.int8); ib[:] = inp.reshape(-1)
# Rearrange weight from (C_out, C_in, 3,3) to (C_in, 3,3, C_out) for NEON
w_re = np.zeros((C_in, 3, 3, C_out), dtype=np.int8)
for ci in range(C_in):
    for ky in range(3):
        for kx in range(3):
            w_re[ci, ky, kx, :] = wt[:, ci, ky, kx]
wb = allocate(shape=(C_in*3*3*C_out,), dtype=np.int8); wb[:] = w_re.reshape(-1)
bb = allocate(shape=(C_out,), dtype=np.int32); bb[:] = bias
ob = allocate(shape=(H//2*W//2*C_out,), dtype=np.int8)

r = subprocess.run(["./conv3x3",
    hex(ib.device_address), hex(wb.device_address), hex(bb.device_address), hex(ob.device_address),
    str(H), str(W), str(C_in), str(C_out),
    str(zp_in), str(zp_w), str(1), str(0), str(zp_out)],
    capture_output=True, text=True, timeout=60)

ts = []
for _ in range(3):
    t0 = time.perf_counter()
    subprocess.run(["./conv3x3",
        hex(ib.device_address), hex(wb.device_address), hex(bb.device_address), hex(ob.device_address),
        str(H), str(W), str(C_in), str(C_out),
        str(zp_in), str(zp_w), str(1), str(0), str(zp_out)],
        capture_output=True, timeout=60)
    ts.append(time.perf_counter() - t0)
t_cpp = np.median(ts)

hw = ob.view(np.int8).reshape(H//2, W//2, C_out).copy()
sw = conv3x3(inp, wt, bias, zp_in, zp_w, mul, zp_out, stride=2)
d = np.max(np.abs(hw.astype(np.int16)-sw.astype(np.int16)))

t0 = time.perf_counter()
for _ in range(3):
    conv3x3(inp, wt, bias, zp_in, zp_w, mul, zp_out, stride=2)
t_sw = (time.perf_counter() - t0) / 3

print(f"NEON conv3x3: {t_cpp*1000:.1f}ms  SW: {t_sw*1000:.1f}ms  speedup: {t_sw/t_cpp:.1f}x  maxdiff={d}")
ib.freebuffer(); wb.freebuffer(); bb.freebuffer(); ob.freebuffer()
