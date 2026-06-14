"""Pytest unit tests — test Python-level logic (constants, reference models).
No RTL simulation needed — these run instantly.

Usage:
    cd tb/cocotb && uv run pytest
"""

import numpy as np
from .sa_driver import SADriver, REG_STATE, REG_FB_CNT, REG_A_LOAD, REG_MUL_Q, REG_SHIFT, REG_ZP_OUT, REG_ZP_IN


def test_register_constants():
    """Register offsets match defs.svh."""
    assert REG_STATE == 0x00
    assert REG_FB_CNT == 0x0C
    assert REG_A_LOAD == 0x10
    assert REG_MUL_Q == 0x30
    assert REG_SHIFT == 0x34
    assert REG_ZP_OUT == 0x38
    assert REG_ZP_IN == 0x3C


def test_reference_matmul():
    """Numpy matmul matches expected sizes."""
    A = np.random.randint(-128, 127, (4, 4)).astype(np.int8)
    B = np.random.randint(-128, 127, (4, 4)).astype(np.int8)
    C = A.astype(np.int32) @ B.astype(np.int32)
    assert C.shape == (4, 4), f"shape mismatch {C.shape}"


def test_quant_reference():
    """Quantization formula matches expected."""
    raw = np.array([[-208, 55], [1000, -2000]], dtype=np.int32)
    mul_q, shift, zp_out = 169, 8, -5
    q = ((raw.astype(np.int64) * mul_q) >> shift) + zp_out
    clamped = np.clip(q, -128, 127)
    assert clamped[0, 0] == -128  # (-208*169>>8)-5 = -142, clamped
    assert clamped[1, 1] == -128  # (-2000*169>>8)-5 = -1325, clamped


def test_sa_driver_abstract():
    """SADriver base class raises NotImplementedError."""
    sa = SADriver(size=4)
    try:
        sa.stream_send(np.array([0]))
        assert False, "should have raised"
    except NotImplementedError:
        pass
