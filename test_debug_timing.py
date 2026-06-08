import sys, time
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
import numpy as np
from pynq import allocate, MMIO

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

print("SA ctrl before:", hex(sa.reg_read(sa.REG_CTRL)))
print("SA mode before:", hex(sa.reg_read(sa.REG_MODE)))
print("SA loop before:", hex(sa.reg_read(sa.REG_LOOP)))

sa.set_mode(False, False)
sa.set_loop_len(n)

print("DMA0 DMACR:", hex(sa.dma_a.sendchannel._mmio.read(0)))
print("DMA0 DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))

sa.dma_a.sendchannel.transfer(in_a)
print("After A trans, DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("After A trans, idle:", sa.dma_a.sendchannel.idle)

sa.dma_b.sendchannel.transfer(in_b)
print("After B trans, DMA0 DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("After B trans, DMA1 DMASR:", hex(sa.dma_b.sendchannel._mmio.read(4)))

# BEFORE starting SA, check DMA status in a loop
for i in range(5):
    d0 = sa.dma_a.sendchannel._mmio.read(4)
    d1 = sa.dma_b.sendchannel._mmio.read(4)
    print(f"  t={i}: DMA0 DMASR={hex(d0)} idle={bool(d0&2)} halt={bool(d0&1)} DMA1 DMASR={hex(d1)}")
    time.sleep(0.0001)

sa.start()
print("After SA start:")
for i in range(5):
    d0 = sa.dma_a.sendchannel._mmio.read(4)
    d1 = sa.dma_b.sendchannel._mmio.read(4)
    d0cr = sa.dma_a.sendchannel._mmio.read(0)
    print(f"  t={i}: DMA0 DMASR={hex(d0)} Halt={bool(d0&1)} DMACR={hex(d0cr)} DMA1 DMASR={hex(d1)}")
    time.sleep(0.0001)

print("SA ctrl after:", hex(sa.reg_read(sa.REG_CTRL)))

in_a.freebuffer()
in_b.freebuffer()
out.freebuffer()
