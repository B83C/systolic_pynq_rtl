"""PYNQ hardware driver — drives MMIO + DMA."""

import numpy as np
from .sa_driver import SADriver, REG_A_LOAD


class PynqDriver(SADriver):
    """SA driver backed by PYNQ MMIO + DMA."""

    def __init__(self, mmio, dma, size=8, a_depth=4, c_depth=4):
        super().__init__(size=size, a_depth=a_depth, c_depth=c_depth)
        self.mmio = mmio
        self.dma = dma

    def reg_write(self, addr, value):
        self.mmio.write(addr, value)

    def reg_read(self, addr):
        return self.mmio.read(addr)

    def stream_send(self, data):
        """Send via DMA MM2S."""
        from pynq import allocate
        buf = allocate(shape=(len(data),), dtype=np.uint32)
        buf[:] = data
        self.dma.sendchannel.transfer(buf)
        self.dma.sendchannel.wait()
        buf.freebuffer()

    def stream_recv(self, n_words):
        """Receive via DMA S2MM."""
        from pynq import allocate
        buf = allocate(shape=(n_words,), dtype=np.uint32)
        self.dma.recvchannel.transfer(buf)
        self.dma.recvchannel.wait()
        out = np.array(buf, dtype=np.uint32)
        buf.freebuffer()
        return out
