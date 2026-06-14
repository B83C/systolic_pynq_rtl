"""Cocotb tests — wraps reusable test functions for the cocotb pytest runner.

Usage:
    cd tb/cocotb
    uv run pytest -k "single_multiply or negative"
"""

import cocotb
from cocotb.triggers import Timer
from .sa_sim import SimDriver
from .test_sa import (
    test_single_multiply,
    test_identity,
    test_accumulation,
    test_negative,
    test_quant_bypass,
    test_quant_identity,
    test_quant_scale,
    test_quant_downscale,
    test_quant_clamp,
    test_quant_input_zp,
)


def _cocotb_test(fn):
    """Decorate a reusable test as a cocotb test."""
    name = fn.__name__

    @cocotb.test()
    async def wrapper(dut):
        await Timer(1, units="ns")
        drv = SimDriver(dut)
        await drv.start_clock()
        await drv.reset()
        fn(drv)  # regular function, not async

    wrapper.__name__ = name
    wrapper.__qualname__ = name
    return wrapper


# Wrap every function above
test_single_multiply  = _cocotb_test(test_single_multiply)
test_identity         = _cocotb_test(test_identity)
test_accumulation     = _cocotb_test(test_accumulation)
test_negative         = _cocotb_test(test_negative)
test_quant_bypass     = _cocotb_test(test_quant_bypass)
test_quant_identity   = _cocotb_test(test_quant_identity)
test_quant_scale      = _cocotb_test(test_quant_scale)
test_quant_downscale  = _cocotb_test(test_quant_downscale)
test_quant_clamp      = _cocotb_test(test_quant_clamp)
test_quant_input_zp   = _cocotb_test(test_quant_input_zp)
