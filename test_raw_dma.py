from pynq import Overlay, MMIO

ol = Overlay("/home/xilinx/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit")
ip0 = ol.ip_dict["axi_dma_0"]
pa = ip0["phys_addr"]
print("phys_addr:", hex(pa))
mmio0 = MMIO(pa, 0x10000)
print("Raw DMA0 MM2S_DMACR:", hex(mmio0.read(0)))
print("Raw DMA0 MM2S_DMASR:", hex(mmio0.read(4)))
print("Raw DMA0 S2MM_DMACR:", hex(mmio0.read(0x30)))
print("Raw DMA0 S2MM_DMASR:", hex(mmio0.read(0x34)))

from pynq.lib.dma import _SDMAChannel
ch = _SDMAChannel(mmio0, 1 << 25, 8, 1, False)
print("After _SDMAChannel init, DMACR:", hex(mmio0.read(0)))
print("After _SDMAChannel init, DMASR:", hex(mmio0.read(4)))
