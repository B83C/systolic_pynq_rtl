import sys
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
import numpy as np
from pynq import allocate

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

A = np.ones((8, 8), dtype=np.uint8)
B = np.eye(8, dtype=np.uint8) * 2

n = 8
wpbi = sa._words_per_beat
wpbo = sa._out_words_per_beat

in_a = allocate(shape=(n * wpbi,), dtype=np.uint32)
in_b = allocate(shape=(n * wpbi,), dtype=np.uint32)
out = allocate(shape=(n * wpbo,), dtype=np.uint32)

for i in range(n):
    in_a[i * wpbi:(i + 1) * wpbi] = sa.pack_row(A[i], wpbi)
    in_b[i * wpbi:(i + 1) * wpbi] = sa.pack_row(B[i], wpbi)

sa.set_mode(False, False)
sa.set_loop_len(n)

sa.dma_a.recvchannel.transfer(out)
sa.dma_a.sendchannel.transfer(in_a)
sa.dma_b.sendchannel.transfer(in_b)

sa.start()

print("send r", sa.dma_a.sendchannel.running, "recv r", sa.dma_a.recvchannel.running)
sa.dma_a.sendchannel.wait()
print("A send done")
sa.dma_b.sendchannel.wait()
print("B send done")
sa.dma_a.recvchannel.wait()
print("recv done")
sa.wait_done()
print("SA done")

result = np.zeros((n, sa.size), dtype=np.uint32)
for i in range(n):
    for j in range(sa.size):
        result[i, j] = out[i * wpbo + j]
print("Result:")
print(result)
in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
