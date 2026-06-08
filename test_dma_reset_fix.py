from pynq import Overlay, MMIO
import time

ol = Overlay("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit")
dma = ol.axi_dma_0

print("DMACR before:", hex(dma.sendchannel._mmio.read(0)))
print("DMASR before:", hex(dma.sendchannel._mmio.read(4)))
print("running:", dma.sendchannel.running)

# Explicitly clear Reset bit by writing DMACR=0 then DMACR=1
dma.sendchannel._mmio.write(0, 0x0000)
time.sleep(0.001)
print("After write 0, DMACR:", hex(dma.sendchannel._mmio.read(0)))
print("After write 0, DMASR:", hex(dma.sendchannel._mmio.read(4)))

dma.sendchannel._mmio.write(0, 0x0001)
time.sleep(0.001)
print("After write 1, DMACR:", hex(dma.sendchannel._mmio.read(0)))
print("After write 1, DMASR:", hex(dma.sendchannel._mmio.read(4)))
print("running:", dma.sendchannel.running)
