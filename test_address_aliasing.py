import sys
sys.path.insert(0, "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual")
from systolic import SystolicArray
from pynq import MMIO
import numpy as np

sa = SystolicArray(
    "/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit",
    ctrl_base_addr=0x40000000
)

# Check DMACR (offset 0) for both DMAs
print("DMA0 send DMACR before:", hex(sa.dma_a.sendchannel._mmio.read(0)))
print("DMA0 recv DMACR before:", hex(sa.dma_a.recvchannel._mmio.read(0x30)))
print("DMA1 send DMACR before:", hex(sa.dma_b.sendchannel._mmio.read(0)))

# Write to SA ctrl via OUR MMIO
sa.reg_write(sa.REG_CTRL, 1)

print("DMA0 send DMACR after:", hex(sa.dma_a.sendchannel._mmio.read(0)))
print("DMA0 recv DMACR after:", hex(sa.dma_a.recvchannel._mmio.read(0x30)))
print("DMA1 send DMACR after:", hex(sa.dma_b.sendchannel._mmio.read(0)))

# Now write via the SA IP's overlay MMIO directly
sa_ip_mmio = sa.ol.ip_dict["sa_wrapper_axi_ctrl_0"]["phys_addr"]
print("SA phys_addr:", hex(sa_ip_mmio))
mmio2 = MMIO(sa_ip_mmio, 0x1000)
mmio2.write(0, 0)
print("DMA0 send DMACR after SA write 0:", hex(sa.dma_a.sendchannel._mmio.read(0)))
print("DMA0 recv DMACR after SA write 0:", hex(sa.dma_a.recvchannel._mmio.read(0x30)))
print("DMA1 send DMACR after SA write 0:", hex(sa.dma_b.sendchannel._mmio.read(0)))
mmio2.write(0, 1)
print("DMA0 send DMACR after SA write 1:", hex(sa.dma_a.sendchannel._mmio.read(0)))
print("DMA0 recv DMACR after SA write 1:", hex(sa.dma_a.recvchannel._mmio.read(0x30)))
print("DMA1 send DMACR after SA write 1:", hex(sa.dma_b.sendchannel._mmio.read(0)))
