import sys
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
import numpy as np
from pynq import allocate

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

# Check DMA initial state
print("Initial send r:", sa.dma_a.sendchannel.running, "idle:", sa.dma_a.sendchannel.idle)
print("Initial recv r:", sa.dma_a.recvchannel.running, "idle:", sa.dma_a.recvchannel.idle)
print("send DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
print("recv DMASR:", hex(sa.dma_a.recvchannel._mmio.read(0x34)))

# Restart the send channel and check
sa.dma_a.sendchannel.stop()
sa.dma_a.sendchannel.start()
print("After restart send r:", sa.dma_a.sendchannel.running, "idle:", sa.dma_a.sendchannel.idle)
print("DMASR:", hex(sa.dma_a.sendchannel._mmio.read(4)))
