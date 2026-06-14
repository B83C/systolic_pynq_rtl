#!/usr/bin/env python3
"""Build SA RTL with Verilator + Cocotb and run tests.

Usage (from repo root):
    uv run --directory tb/cocotb python3 -m tb.cocotb.run
"""

import sys, os, subprocess

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

if __name__ == "__main__":
    # Build using the existing justfile command (which already works)
    os.chdir(REPO)
    subprocess.run(["just", "test-ctrl"], check=True)
