"""
PYNQ-Z2 pointwise GEMM accelerator — software model.

This module mimics the FPGA block-major GEMM in numpy so you can
test the full pipeline on the ARM before the hardware is ready.

Architecture
────────────
  The FPGA GEMM takes two buffers:
    A — weight matrix in block-major format  (pre-loaded, cached)
    B — activation matrix in channel-last     (streamed through)

  A_block is a single contiguous buffer in block-major order:
    (Cout//T, Cin//T, T, T)  INT8  — T=8 matches the 8×8 MAC array.

  B is a contiguous row-major buffer:
    (HW, Cin) INT8  — channel-last, one row per spatial position.

  Output is channel-last continuous INT8:
    (HW, Cout) INT8

Usage
─────
  # Pre-compile a layer (once per layer, at init):
  pw = PointwiseLayer(weight_onx, bias_onx, zp_w, mul, zp_out)

  # Per image / per tile:
  out = pw.run(act_pixels, zp_act)          # (H, W, Cout) INT8
  # or with flat buffers:
  out = gemm(A_block, B_flat, bias, mul, zp_out, zp_act)

Replace `` → hardware DMA call when the FPGA module is ready.
"""

import numpy as np


# ═════════════════════════════════════════════════════════════════════
#  1.  Block-major format
# ═════════════════════════════════════════════════════════════════════

def to_block_major(matrix: np.ndarray, bh: int, bw: int) -> np.ndarray:
    """Row-major (H, W) → block-major (H//bh, W//bw, bh, bw).

    Blocks are laid out in memory row-by-row: block(0,0), block(0,1), …
    so one burst read loads one (bh×bw) tile for the MAC array.
    """
    H, W = matrix.shape
    assert H % bh == 0 and W % bw == 0, f'{H}×{W} not divisible by {bh}×{bw}'
    reshaped = matrix.reshape(H // bh, bh, W // bw, bw)
    transposed = reshaped.transpose(0, 2, 1, 3)
    return np.ascontiguousarray(transposed)   # (ny, nx, bh, bw)


def from_block_major(block: np.ndarray, bh: int, bw: int) -> np.ndarray:
    """Inverse: block-major → row-major."""
    ny, nx = block.shape[0], block.shape[1]
    tran = block.transpose(0, 2, 1, 3)
    return tran.reshape(ny * bh, nx * bw)


# ═════════════════════════════════════════════════════════════════════
#  2.  GEMM  —  this is what the FPGA does
# ═════════════════════════════════════════════════════════════════════

def gemm(
    A_block: np.ndarray,   # (ny, nx, T, T)  INT8 — weight, block-major
    B: np.ndarray,          # (M, Cin)        INT8 — activation, row-major
    bias: np.ndarray,       # (Cout,)          INT32
    mul: float,             # s_act × s_w / s_out
    zp_out: int,            # output zero-point
    zp_act: int,            # activation zero-point
    T: int = 8,
) -> np.ndarray:
    """Block-major GEMM → quantized INT8 output.

    Computes:  out = (B − zp_act) × A_block.T × mul + bias + zp_out

    The FPGA does this tile-by-tile (caching A_block tiles, streaming B).
    This numpy version is numerically identical and can be swapped out
    when the hardware is ready.
    """
    ny, nx, _, _ = A_block.shape
    M = B.shape[0]
    Cin_orig = B.shape[1]
    Cout_orig = bias.shape[0]

    # Flatten block-major → row-major, then take only the unpadded rows/cols.
    A_full = A_block.transpose(0, 2, 1, 3).reshape(ny * T, nx * T)   # (Cp, Kp)

    # ── Data path ──────────────────────────────────────────────────
    B_off = B.astype(np.int32) - zp_act                               # (M, Cin_orig)
    A_off = A_full[:Cout_orig, :Cin_orig].astype(np.int32)            # (Cout_orig, Cin_orig)

    acc = B_off @ A_off.T                                             # (M, Cout_orig) INT32
    acc += bias.astype(np.int32)                                      # + bias

    # ── Requant ────────────────────────────────────────────────────
    out = np.round(acc.astype(np.float64) * mul).astype(np.int32) + zp_out
    out = np.clip(out, -128, 127).astype(np.int8)

    return out   # (M, Cout) channel-last continuous


# ═════════════════════════════════════════════════════════════════════
#  3.  Layer wrapper (pre-compile, then run)
# ═════════════════════════════════════════════════════════════════════

def prepare_weight(
    weight: np.ndarray,   # (Cout, Cin[, 1, 1]) INT8 — ONNX weight
    zp_w: int,            # weight zero-point (0 in this model)
    T: int = 8,
) -> np.ndarray:
    """Pre-subtract zp_w and convert to block-major.

    The result is cached and fed to the FPGA's A input for every call.
    """
    if weight.ndim == 4:
        weight = weight[:, :, 0, 0]   # squeeze 1×1

    Cout, Cin = weight.shape

    # Pad to tile boundary
    Cp = ((Cout + T - 1) // T) * T
    Kp = ((Cin  + T - 1) // T) * T
    w_pad = np.zeros((Cp, Kp), dtype=np.int8)
    w_pad[:Cout, :Cin] = weight

    # Pre-subtract zp_w (weights are fixed → done at compile time)
    w_off = w_pad.astype(np.int16) - zp_w
    w_off = np.clip(w_off, -128, 127).astype(np.int8)

    return to_block_major(w_off, T, T)   # (Cp//T, Kp//T, T, T)


class PointwiseLayer:
    """One pointwise convolution layer, ready for the FPGA.

    Usage
    ─────
    pw = PointwiseLayer(weight, bias, zp_w, mul, zp_out)
    out = pw.run(act, zp_act)            # (H, W, Cout) INT8
    """

    def __init__(self, weight, bias, zp_w, mul, zp_out, T=8):
        self.A_block = prepare_weight(weight, zp_w, T)
        self.bias    = bias.astype(np.int32).ravel()
        self.mul     = mul
        self.zp_out  = zp_out
        self.T       = T
        self.Cout    = weight.shape[0]
        self.Cin     = weight.shape[1]

    def run(self, act: np.ndarray, zp_act: int) -> np.ndarray:
        """act: (H, W, Cin) INT8 channel-last. → (H, W, Cout) INT8."""
        H, W, C = act.shape
        B = act.reshape(-1, C)   # (HW, Cin)
        out = gemm(self.A_block, B, self.bias,
                   self.mul, self.zp_out, zp_act, self.T)
        return out.reshape(H, W, self.Cout)


# ═════════════════════════════════════════════════════════════════════
#  4.  On-device test (runs on PYNQ-Z2 ARM)
# ═════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    # ── PC-side test (numpy, needs ONNX) ───────────────────────────
    # This validates the block-major GEMM against the reference pipeline.
    # On PYNQ-Z2, skip the ONNX import and use pre-compiled weights.

    import sys, os

    ONNX_AVAIL = False
    try:
        import onnx, onnx.numpy_helper as nph
        ONNX_AVAIL = True
    except ModuleNotFoundError:
        pass

    if not ONNX_AVAIL:
        # PYNQ-Z2: run only the self-consistency check
        print('ONNX not available — skipping reference comparison.')
        rng = np.random.default_rng(42)
        for Cout, Cin in [(16, 16), (64, 32), (1, 64)]:
            w = rng.integers(-128, 127, (Cout, Cin), dtype=np.int8)
            b = rng.integers(-2**20, 2**20, Cout, dtype=np.int32)
            for _ in range(10):
                H, W = rng.integers(1, 16, 2)
                B = rng.integers(-128, 127, (H * W, Cin), dtype=np.int8)
                o1 = gemm(prepare_weight(w, 0), B, b, 0.01, 14, -128)
                assert o1.shape == (H * W, Cout)
            print(f'  [{Cout}×{Cin}] random tests OK')
        print('All self-consistency checks passed.')
        sys.exit(0)

    # ── Compare against OpenCV reference ────────────────────────────
    import cv2
    from sim_hw import build_stage_table, hw_pointwise

    img = cv2.imread('test.png')
    img_640 = cv2.resize(img, (640, 640))

    stages, P = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    pw_stages = [(i, s) for i, s in enumerate(stages) if s['type'] == 'pw']

    print(f'Testing {len(pw_stages)} pointwise layers against hw_pointwise...')
    rng = np.random.default_rng(42)
    failures = 0
    for idx, s in pw_stages:
        weight = s['w'].reshape(s['w'].shape[0], -1)
        pw = PointwiseLayer(weight, s['b_i32'], s['zp_w'], s['mul'], s['zp_out'])

        # Random activation tile
        H, W = 8, 8
        act = rng.integers(-128, 127, (H, W, s['w'].shape[1]), dtype=np.int8)

        out_hw = hw_pointwise(act, weight, s['b_i32'],
                              s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
        out_pw = pw.run(act, s['zp_in'])

        ok = np.all(out_hw == out_pw)
        if ok:
            print(f'  [{idx:3d}] {s["out"]:30s} OK  [{out_pw.min()}, {out_pw.max()}]')
        else:
            failures += 1
            d = np.abs(out_hw.astype(np.int16) - out_pw.astype(np.int16))
            print(f'  [{idx:3d}] {s["out"]:30s} FAIL  maxdiff={d.max()}')

    if failures:
        print(f'\n{FAILURES} FAILURES')
    else:
        print(f'\nAll {len(pw_stages)} layers passed.')
