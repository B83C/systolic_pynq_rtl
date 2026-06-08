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

in_a = allocate(shape=(n * wpbi,), dtype=np.uint32)
in_b = allocate(shape=(n * wpbi,), dtype=np.uint32)
in_a[:] = np.ones(n * wpbi, dtype=np.uint32)
in_b[:] = np.ones(n * wpbi, dtype=np.uint32)

sa.set_mode(False, False)
sa.set_loop_len(n)

# Send only without recv
sa.dma_a.sendchannel.transfer(in_a)
print("A transfer", "idle:", sa.dma_a.sendchannel.idle, "DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))

sa.dma_b.sendchannel.transfer(in_b)
print("B transfer", "idle:", sa.dma_b.sendchannel.idle, "DMASR:", hex(sa.dma_b.sendchannel._mmio.read(4)))
# dma_b is axi_dma_1
import time
time.sleep(0.5)

print("A idle:", sa.dma_a.sendchannel.idle, "DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("B idle:", sa.dma_b.sendchannel.idle, "DMASR:", hex(sa.dma_b.sendchannel._mmio.read(4)))

sa.start()
time.sleep(0.001)

print("After start:")
print("A idle:", sa.dma_a.sendchannel.idle, "DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("B idle:", sa.dma_b.sendchannel.idle, "DMASR:", hex(sa.dma_b.sendchannel._mmio.read(4)))
print("SA ctrl:", hex(sa.reg_read(sa.REG_CTRL)))

in_a.freebuffer()
in_b.freebuffer()
