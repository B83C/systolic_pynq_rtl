from pynq import Overlay, MMIO
import time

ol = Overlay("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit")
dma = ol.axi_dma_0

print("=== Initial state ===")
print("DMACR:", hex(dma.sendchannel._mmio.read(0)))
print("DMASR:", hex(dma.sendchannel._mmio.read(4)))

# Try writing just bit 1 = 0 (clear Reset)
dma.sendchannel._mmio.write(0, 0x0000)
time.sleep(0.01)
print("\nAfter write 0x0000, DMACR:", hex(dma.sendchannel._mmio.read(0)))

# Try to write RS=1 and Reset=0 simultaneously  
dma.sendchannel._mmio.write(0, 0x0001 & ~0x02)  # same as 0x0001
time.sleep(0.01)
print("After write 0x0001, DMACR:", hex(dma.sendchannel._mmio.read(0)))

# Try explicitly setting Reset=0 by masking it out on a read-modify-write
cur = dma.sendchannel._mmio.read(0)
print("Current DMACR:", hex(cur))
dma.sendchannel._mmio.write(0, cur & ~0x02)  # Clear bit 1
time.sleep(0.01)
print("After clearing bit 1, DMACR:", hex(dma.sendchannel._mmio.read(0)))

# Try writing 0x0004 (just bit 2 set, neither RS nor Reset)
dma.sendchannel._mmio.write(0, 0x0004)
time.sleep(0.01)
print("\nAfter write 0x0004, DMACR:", hex(dma.sendchannel._mmio.read(0)))
print("After write 0x0004, DMASR:", hex(dma.sendchannel._mmio.read(4)))

# Try setting keyhole bit maybe it's confusing things?
# Write RS=1, Reset=0 explicitly
dma.sendchannel._mmio.write(0, 0x0001)
print("\nFinal write 0x0001:")
print("DMACR:", hex(dma.sendchannel._mmio.read(0)))
print("DMASR:", hex(dma.sendchannel._mmio.read(4)))
print("running:", dma.sendchannel.running)
print("idle:", dma.sendchannel.idle)
