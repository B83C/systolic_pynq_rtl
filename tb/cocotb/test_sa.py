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


def test_c_matrix(sa: SADriver):
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
