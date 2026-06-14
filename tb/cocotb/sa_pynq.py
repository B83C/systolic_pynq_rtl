"""PYNQ hardware driver — caller manages DMA buffer lifetime."""

from .sa_driver import SADriver


class PynqDriver(SADriver):
    """SA driver backed by PYNQ MMIO + DMA.

    Buffers are passed in from outside (e.g. from ``pynq.allocate()``).
    """

    def __init__(self, mmio, dma, size=8, a_depth=4, c_depth=4):
        super().__init__(size=size, a_depth=a_depth, c_depth=c_depth)
        self.mmio = mmio
        self.dma = dma

    def reg_write(self, addr, value):
        self.mmio.write(addr, value)

    def reg_read(self, addr):
        return self.mmio.read(addr)

    def stream_send(self, buf):
        """Send a pre-allocated buffer via DMA MM2S (blocking)."""
        self.dma.sendchannel.transfer(buf)
        self.dma.sendchannel.wait()

    def stream_recv(self, buf):
        """Receive into a pre-allocated buffer via DMA S2MM (blocking)."""
        self.dma.recvchannel.transfer(buf)
        self.dma.recvchannel.wait()
