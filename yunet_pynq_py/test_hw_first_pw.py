"""
Quick HW pointwise test: run first pw layer (16×16) on systolic array.
"""
import numpy as np
from pynq import allocate, Overlay, MMIO

print("Loading systolic array...")
ol = Overlay('systolic.bit', download=True)
from systolic import SystolicArray
sa = SystolicArray('systolic.bit', ctrl_base_addr=0x40000000, size=8)
print(f'  size={sa.size}, a_depth={sa.a_depth}, c_depth={sa.c_depth}')

# Simple test: 16×16 weight, 4×4 activation (16 spatial positions)
Cin, Cout = 16, 16
H, W = 4, 4

rng = np.random.default_rng(42)
weight = rng.integers(-128, 127, (Cout, Cin), dtype=np.int8)
bias = rng.integers(-1000, 1000, Cout, dtype=np.int32)
act = rng.integers(-128, 127, (H, W, Cin), dtype=np.int8)

# Software reference
sw_out = (act.reshape(-1, Cin).astype(np.float32) @ weight.astype(np.float32).T
          + bias.astype(np.float32))
sw_out = np.clip(np.round(sw_out * 0.01).astype(np.int32) + 42, -128, 127).astype(np.int8)

# HW call
from yunet_pynq import hw_pointwise
hw_out = hw_pointwise(sa, act, weight, bias,
                       zp_in=0, zp_w=0, mul_float=0.01, zp_out=42)

diff = np.abs(hw_out.astype(np.int16) - sw_out.astype(np.int16))
print(f'Max diff: {diff.max()}')
print(f'HW range: [{hw_out.min()}, {hw_out.max()}]')
print(f'SW range: [{sw_out.min()}, {sw_out.max()}]')

if diff.max() <= 1:
    print('PASS: HW matches SW')
else:
    print('FAIL: HW differs from SW')
    bad = np.where(diff > 1)
    for idx in range(min(5, len(bad[0]))):
        i, j, k = bad[0][idx], bad[1][idx], bad[2][idx]
        print(f'  [{i},{j},{k}] HW={hw_out[i,j,k]} SW={sw_out[i,j,k]}')
