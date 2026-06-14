"""Cocotb simulation driver — drives RTL signals directly."""

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, ClockCycles, Timer
from cocotb.binary import BinaryValue
import numpy as np

from .sa_driver import SADriver, REG_A_LOAD, REG_RST_INDEX


class SimDriver(SADriver):
    """SA driver backed by a cocotb DUT."""

    def __init__(self, dut, size=4):
        # Probe SIZE from DUT if possible, else use provided
        try:
            size = int(dut.SIZE.value)
        except Exception:
            pass
        super().__init__(size=size)
        self.dut = dut
        self._out_buf = []  # consumed by output monitor

    # ------------------------------------------------------------------
    #  Clock / reset
    # ------------------------------------------------------------------
    async def start_clock(self, period_ns=2):
        cocotb.start_soon(Clock(self.dut.clk, period_ns, units="ns").start())

    async def reset(self, cycles=5):
        self.dut.rst_n.value = 0
        await ClockCycles(self.dut.clk, cycles)
        self.dut.rst_n.value = 1
        await ClockCycles(self.dut.clk, 1)

    # ------------------------------------------------------------------
    #  AXI-Lite
    # ------------------------------------------------------------------
    async def reg_write(self, addr, value):
        d = self.dut
        d.s_axil_awvalid.value = 1
        d.s_axil_awaddr.value = addr
        d.s_axil_wvalid.value = 1
        d.s_axil_wdata.value = value
        await RisingEdge(d.clk)
        while not d.s_axil_bvalid.value:
            await RisingEdge(d.clk)
        d.s_axil_awvalid.value = 0
        d.s_axil_wvalid.value = 0
        d.s_axil_bready.value = 1
        await RisingEdge(d.clk)
        d.s_axil_bready.value = 0

    async def reg_read(self, addr):
        d = self.dut
        d.s_axil_arvalid.value = 1
        d.s_axil_araddr.value = addr
        await RisingEdge(d.clk)
        while not d.s_axil_rvalid.value:
            await RisingEdge(d.clk)
        data = int(d.s_axil_rdata.value)
        d.s_axil_arvalid.value = 0
        d.s_axil_rready.value = 1
        await RisingEdge(d.clk)
        d.s_axil_rready.value = 0
        return data

    # ------------------------------------------------------------------
    #  AXI-Stream send  (drives s_axis_B)
    # ------------------------------------------------------------------
    async def stream_send(self, data):
        """Send uint32 words as AXI-Stream beats (packed into tdata)."""
        d = self.dut
        d.s_axis_B_tvalid.value = 0
        d.s_axis_B_tlast.value = 0
        await RisingEdge(d.clk)

        words = data if hasattr(data, '__len__') else [data]
        n = len(words)
        for i, w in enumerate(words):
            d.s_axis_B_tdata.value = int(w)
            d.s_axis_B_tvalid.value = 1
            d.s_axis_B_tlast.value = (i == n - 1)
            while not d.s_axis_B_tready.value:
                await RisingEdge(d.clk)
            await RisingEdge(d.clk)
        d.s_axis_B_tvalid.value = 0
        d.s_axis_B_tlast.value = 0

    # ------------------------------------------------------------------
    #  AXI-Stream receive (reads m_axis, collected by monitor)
    # ------------------------------------------------------------------
    async def stream_recv(self, n_words):
        """Return `n_words` uint32 words from the output monitor buffer."""
        d = self.dut
        d.m_axis_tready.value = 1
        while len(self._out_buf) < n_words:
            await RisingEdge(d.clk)
        d.m_axis_tready.value = 0
        words = self._out_buf[:n_words]
        self._out_buf[:] = self._out_buf[n_words:]
        return np.array(words, dtype=np.uint32)


# ---------------------------------------------------------------------------
#  Output monitor — always-on capture
# ---------------------------------------------------------------------------
@cocotb.coroutine
def _output_monitor(dut, buf):
    while True:
        await RisingEdge(dut.clk)
        if dut.m_axis_tvalid.value and dut.m_axis_tready.value:
            data = int(dut.m_axis_tdata.value)
            buf.append(data)


# ---------------------------------------------------------------------------
#  Pytest-compatible test factory
# ---------------------------------------------------------------------------
def run_test(test_fn):
    """Decorator that creates a cocotb test from a test_* function."""
    import functools

    @cocotb.test
    @functools.wraps(test_fn)
    async def wrapper(dut):
        driver = SimDriver(dut)
        await driver.start_clock()
        await driver.reset()
        # Start output monitor
        cocotb.start_soon(_output_monitor(dut, driver._out_buf))
        # Mark ready
        dut.m_axis_tready.value = 1
        # Run the test
        await test_fn(driver)
        # Let pipeline flush
        await ClockCycles(dut.clk, 20)

    # Register with cocotb
    wrapper.__name__ = f"test_{test_fn.__name__}"
    return wrapper
