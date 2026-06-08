import sys, time
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
import numpy as np
from pynq import allocate

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

n = 4
A = np.ones((n, n), dtype=np.uint8)
B = np.eye(n, dtype=np.uint8) * 2

wpbi = sa._input_words_per_beat
wpbo = sa._output_words_per_beat

in_a = allocate(shape=(n * wpbi,), dtype=np.uint32)
in_b = allocate(shape=(n * wpbi,), dtype=np.uint32)
out = allocate(shape=(n * wpbo,), dtype=np.uint32)

for i in range(n):
    col = np.uint32(A[i, 0])
    for j in range(1, n):
        col |= np.uint32(A[i, j]) << (8 * j)
    in_a[i * wpbi:(i+1)*wpbi] = col

for i in range(n):
    col = np.uint32(B[i, 0])
    for j in range(1, n):
        col |= np.uint32(B[i, j]) << (8 * j)
    in_b[i * wpbi:(i+1)*wpbi] = col

sa.set_mode(False, False)
sa.set_loop_len(n)

# Manual transfer like the old test_full
sa.dma_a.recvchannel.transfer(out)
sa.dma_a.sendchannel.transfer(in_a)
sa.dma_b.sendchannel.transfer(in_b)

sa.start()

time.sleep(0.05)
print("send A running:", sa.dma_a.sendchannel.running)
print("send B running:", sa.dma_b.sendchannel.running)
print("recv running:", sa.dma_a.recvchannel.running)

try:
    sa.dma_a.sendchannel.wait()
    print("sendA done")
except Exception as e:
    print("sendA err:", e)

try:
    sa.dma_b.sendchannel.wait()
    print("sendB done")
except Exception as e:
    print("sendB err:", e)

try:
    sa.dma_a.recvchannel.wait()
    print("recv done")
except Exception as e:
    print("recv err:", e)

sa.wait_done()

result = np.zeros((n, sa.size), dtype=np.uint32)
for i in range(n):
    for j in range(sa.size):
        result[i, j] = out[i * wpbo + j]
print("Result:")
print(result)

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
