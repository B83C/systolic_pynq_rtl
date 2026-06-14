#!/usr/bin/env python3
"""SA test runner — run the same tests in sim (cocotb) or on PYNQ hardware.

Sim (cocotb + verilator):
    cd tb/cocotb
    uv sync                                     # install deps once
    uv run python3 -m cocotb run --build        # requires Verilator + VPI setup
    uv run python3 -m cocotb run                # run all tests

PYNQ hardware:
    Copy tb/cocotb/ to the PYNQ board, then:
    from sa_pynq import PynqDriver
    from test_sa import *
    mmio = MMIO(0x40000000, 0x1000)
    dma = Overlay('systolic.bit').axi_dma_0
    sa = PynqDriver(mmio, dma, size=8)
    test_single_multiply(sa)
"""

import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent.parent

if __name__ == "__main__":
    print(__doc__)
