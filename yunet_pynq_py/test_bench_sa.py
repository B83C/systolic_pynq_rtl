"""Run bench_sa matmul_sa directly to verify 8x8 works."""
import sys, numpy as np
sys.path.insert(0, '.')
from bench_sa import matmul_sa, ref_mm

np.random.seed(1234)
for Co, Ci, N in [(8,8,8), (8,8,16), (8,8,32), (16,16,16)]:
    A = np.random.randint(-5,5,(Co,Ci),dtype=np.int8)
    B = np.random.randint(-5,5,(N,Ci),dtype=np.int8)
    r = matmul_sa(A, B, 0)
    e = ref_mm(A, B, 0)
    ok = np.array_equal(r, e)
    d = np.max(np.abs(r.astype(np.int16)-e.astype(np.int16))) if not ok else 0
    print(f"  {Co}x{Ci} N={N}: {'PASS' if ok else f'FAIL d={d}'}")
