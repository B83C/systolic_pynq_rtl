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

print("1. r: send", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)

sa.set_mode(False, False)
print("2. after set_mode: send", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)

sa.set_loop_len(n)
print("3. after set_loop_len: send", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)

sa.dma_a.recvchannel.transfer(out)
print("4. after recv transfer: send", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)

sa.dma_a.sendchannel.transfer(in_a)
print("5. after sendA transfer: send", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)
print("   send DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("   recv DMASR:", hex(sa.dma_a.recvchannel._mmio.read(0x34)))

sa.dma_b.sendchannel.transfer(in_b)
print("6. after sendB transfer: sendA", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)

sa.start()
print("7. after start: sendA", sa.dma_a.sendchannel.running, "recv", sa.dma_a.recvchannel.running)
print("   send DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("   recv DMASR:", hex(sa.dma_a.recvchannel._mmio.read(0x34)))
print("   SA ctrl:", hex(sa.reg_read(sa.REG_CTRL)))

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
