"""Cocotb simulation driver — uses cocotbext-axi for AXI-Lite/Stream."""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, ClockCycles
from cocotbext.axi import AxiLiteBus, AxiLiteMaster, AxiStreamBus, AxiStreamSource, AxiStreamSink
import numpy as np

from .sa_driver import SADriver


class SimDriver(SADriver):
    """SA driver backed by cocotb + cocotbext-axi."""

    def __init__(self, dut):
        # Probe SIZE from DUT
        try:
            size = int(dut.SIZE.value)
        except Exception:
            size = 4
        super().__init__(size=size)
        self.dut = dut

        # AXI-Lite master (control)
        axil_bus = AxiLiteBus.from_prefix(dut, "s_axil")
        self.axil = AxiLiteMaster(axil_bus, dut.clk, dut.rst_n)

        # AXI-Stream source (drives s_axis_B)
        s_bus = AxiStreamBus.from_prefix(dut, "s_axis_B")
        self.s_axis = AxiStreamSource(s_bus, dut.clk, dut.rst_n)

        # AXI-Stream sink (reads m_axis)
        m_bus = AxiStreamBus.from_prefix(dut, "m_axis")
        self.m_axis = AxiStreamSink(m_bus, dut.clk, dut.rst_n)

    async def start_clock(self, period_ns=2):
        cocotb.start_soon(Clock(self.dut.clk, period_ns, units="ns").start())

    async def reset(self, cycles=5):
        self.dut.rst_n.value = 0
        await ClockCycles(self.dut.clk, cycles)
        self.dut.rst_n.value = 1
        await ClockCycles(self.dut.clk, 1)

    # -- AXI-Lite ---------------------------------------------------------
    async def reg_write(self, addr, value):
        await self.axil.write(addr, value)

    async def reg_read(self, addr):
        return (await self.axil.read(addr)).data

    # -- AXI-Stream -------------------------------------------------------
    async def stream_send(self, data):
        """Send uint32 words via AXI-Stream, tlast on last word."""
        words = data if hasattr(data, '__len__') else [data]
        for i, w in enumerate(words):
            tlast = (i == len(words) - 1)
            await self.s_axis.send(int(w), tlast=tlast)

    async def stream_recv(self, n_words):
        """Receive `n_words` uint32 words from m_axis."""
        out = []
        while len(out) < n_words:
            tdata = await self.m_axis.recv()
            out.append(int(tdata.tdata))
        return np.array(out[:n_words], dtype=np.uint32)
