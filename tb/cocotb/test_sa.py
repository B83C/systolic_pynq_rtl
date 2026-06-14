"""SA test cases using numpy.  Run with either cocotb sim or on PYNQ."""

import numpy as np

from .sa_driver import SADriver


def test_single_multiply(sa: SADriver):
    """A × B1 — every row differs."""
    np.random.seed(42)
    A = np.random.randint(-128, 127, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-128, 127, (sa.size, sa.size)).astype(np.int8)

    expected = A.astype(np.int32) @ B.astype(np.int32)
    result = sa.compute(A, B)
    assert result.shape == expected.shape, f"shape mismatch {result.shape}"
    assert np.allclose(result, expected), f"max err={np.max(np.abs(result - expected))}"


def test_identity(sa: SADriver):
    """A × I = A."""
    A = np.random.randint(-128, 127, (sa.size, sa.size)).astype(np.int8)
    I = np.eye(sa.size, dtype=np.int8)

    result = sa.compute(A, I)
    assert np.allclose(result, A.astype(np.int32)), "A×I != A"


def test_accumulation(sa: SADriver):
    """FB_CNT=1: two B matrices accumulate."""
    np.random.seed(1)
    A = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    B1 = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    B2 = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)

    expected = A.astype(np.int32) @ B1.astype(np.int32) + \
               A.astype(np.int32) @ B2.astype(np.int32)

    sa.configure(fb_cnt=1)
    A_i32 = A.astype(np.int32)
    B1_i32 = B1.astype(np.int32)
    B2_i32 = B2.astype(np.int32)
    expected = A_i32 @ B1_i32 + A_i32 @ B2_i32

    sa.soft_reset()
    sa.reg_write(sa.REG_A_LOAD, 0)
    sa.stream_send(sa.pack_rows(A, sa._input_words))
    sa.stream_send(sa.pack_rows(B1, sa._input_words))
    sa.stream_send(sa.pack_rows(B2, sa._input_words))
    out = sa.stream_recv(2 * sa.size * sa._output_words)
    result = sa.unpack_rows(out, 2 * sa.size, sa.size, sa._output_words)
    # last SIZE rows = accumulated
    acc = result[sa.size:]
    assert np.allclose(acc, expected), f"max err={np.max(np.abs(acc - expected))}"


def test_negative(sa: SADriver):
    """Signed multiplication with negative B."""
    A = np.array([[10, 11, 12, 13],
                  [11, 12, 13, 14],
                  [12, 13, 14, 15],
                  [13, 14, 15, 16]], dtype=np.int8)[:sa.size, :sa.size]
    B = np.array([[-1,  2, -3,  4],
                  [ 5, -6,  7, -8],
                  [-9, 10,-11, 12],
                  [13,-14, 15,-16]], dtype=np.int8)[:sa.size, :sa.size]

    expected = A.astype(np.int32) @ B.astype(np.int32)
    result = sa.compute(A, B)
    assert np.allclose(result, expected), f"max err={np.max(np.abs(result - expected))}"


def test_quant_bypass(sa: SADriver):
    """Default mul_q=0 → bypass, output = raw 32-bit result."""
    A = np.random.randint(-128, 127, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-128, 127, (sa.size, sa.size)).astype(np.int8)
    expected = A.astype(np.int32) @ B.astype(np.int32)
    result = sa.compute(A, B)
    assert np.allclose(result, expected), f"bypass mismatch"


def test_quant_identity(sa: SADriver):
    """mul_q=1, shift=0, zp_out=0 → clamp(result, -128, 127)."""
    A = np.random.randint(-200, 200, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-200, 200, (sa.size, sa.size)).astype(np.int8)
    raw = A.astype(np.int32) @ B.astype(np.int32)
    expected = np.clip(raw, -128, 127).astype(np.int8)

    sa.set_quant(mul_q=1, shift=0, zp_out=0)
    result = sa.compute(A, B)
    assert np.allclose(result, expected.astype(np.int32)), f"identity quant"


def test_quant_scale(sa: SADriver):
    """mul_q=2, shift=1 → no-op (result*2>>1 = result), zp_out=10 adds offset."""
    np.random.seed(7)
    A = np.random.randint(-64, 64, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-64, 64, (sa.size, sa.size)).astype(np.int8)
    raw = A.astype(np.int32) @ B.astype(np.int32)
    expected = np.clip((raw * 2 >> 1) + 10, -128, 127).astype(np.int8)

    sa.set_quant(mul_q=2, shift=1, zp_out=10)
    result = sa.compute(A, B)
    assert np.allclose(result, expected.astype(np.int32)), f"scale+offset"


def test_quant_downscale(sa: SADriver):
    """mul_q=169, shift=8 → scale factor ~0.66 + zp_out."""
    np.random.seed(3)
    A = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    raw = A.astype(np.int32) @ B.astype(np.int32)
    mul_q, shift, zp_out = 169, 8, -5
    expected = np.clip(((raw.astype(np.int64) * mul_q) >> shift) + zp_out, -128, 127)

    sa.set_quant(mul_q=mul_q, shift=shift, zp_out=zp_out)
    result = sa.compute(A, B)
    assert np.allclose(result, expected.astype(np.int32)), f"downscale"


def test_quant_clamp(sa: SADriver):
    """Values outside [-128,127] get clamped."""
    A = np.full((sa.size, sa.size), 100, dtype=np.int8)
    B = np.full((sa.size, sa.size), 100, dtype=np.int8)
    raw = A.astype(np.int32) @ B.astype(np.int32)  # = 100*100*4 = 40000 per col
    expected = np.clip(raw, -128, 127)

    sa.set_quant(mul_q=1, shift=0, zp_out=0)
    result = sa.compute(A, B)
    assert np.allclose(result, expected), f"clamp"


def test_quant_input_zp(sa: SADriver):
    """zp_in shifts B before MAC (B' = B - zp_in)."""
    np.random.seed(5)
    A = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    zp_in = -3
    B_adj = B.astype(np.int32) - zp_in
    raw = A.astype(np.int32) @ B_adj
    expected = np.clip(raw, -128, 127)

    sa.set_quant(mul_q=1, shift=0, zp_out=0, zp_in=zp_in)
    result = sa.compute(A, B)
    assert np.allclose(result, expected.astype(np.int32)), f"input zp"
    """A × B + C (bias)."""
    A = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    B = np.random.randint(-16, 16, (sa.size, sa.size)).astype(np.int8)
    C = np.random.randint(-512, 512, (sa.size,)).astype(np.int32)

    expected = A.astype(np.int32) @ B.astype(np.int32) + C[:, None]

    sa.soft_reset()
    sa.configure(fb_cnt=0)
    sa.reg_write(sa.REG_C_LOAD, 0)
    sa.stream_send(np.array(C, dtype=np.uint32))
    sa.reg_write(sa.REG_A_LOAD, 0)
    sa.stream_send(sa.pack_rows(A, sa._input_words))
    sa.stream_send(sa.pack_rows(B, sa._input_words))
    out = sa.stream_recv(sa.size * sa._output_words)
    result = sa.unpack_rows(out, sa.size, sa.size, sa._output_words)
    assert np.allclose(result, expected), f"max err={np.max(np.abs(result - expected))}"
