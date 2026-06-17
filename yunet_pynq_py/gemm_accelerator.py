"""
8×8 INT8 systolic GEMM for YuNet pointwise (1×1) convolution.

Architecture
────────────
  MAC array:   8 × 8 = 64 PEs, each INT8 × INT8 → INT32, accumulate in INT32
  Data flow:   Input-stationary (A cached, B streamed)
  Matrix A:    Activations   (T × K)  INT8   — cached per row-tile
  Matrix B:    Weights       (K × T)  INT8   — streamed column-tile by column-tile
  Output:      Accumulator   (T × T)  INT32  — stays in array, then requant → INT8

  T = tile size = array dimension = 8.

Pointwise conv is: out(HW, Cout) = act(HW, Cin) @ weight.T(Cin, Cout)

Tiled execution:
────────────────
  act is stored in hardware-friendly row-tiled order.
  weight.T is stored column-tiled.

  for each row-tile of act (T spatial positions):
    cache A_tile  (T × Cin) in the array
    for each column-tile of weight.T (T output channels):
      stream B_col (Cin × T) through the array
      acc += A_tile @ B_col     ← 8×8 MAC array, partial-sum accumulates
    bias → requant → store T rows

Memory layout (tiled rows):
───────────────────────────
  Instead of row-major [row0, row1, row2, …, rowM-1],
  store as [block0, block1, …] where each block = T consecutive rows:
    block_b = rows[b*T : (b+1)*T] flattened as a (T × K) matrix.
  This matches the A-cache line size (T rows / load).
"""

import numpy as np


# ── Tiled memory layout helpers ──────────────────────────────────────────

def to_tiled_rows(mat: np.ndarray, T: int = 8) -> np.ndarray:
    """Reshape (M, K) → (M//T, T, K) — tiled row blocks.

    The tiled representation groups T consecutive rows into one block,
    so that one cache-load brings exactly one tile into the MAC array.
    """
    M, K = mat.shape
    n_full = M // T
    tiles = mat[:n_full * T].reshape(-1, T, K)
    tail = mat[n_full * T:]
    if len(tail):
        pad = np.zeros((T - len(tail), K), dtype=mat.dtype)
        tiles = np.vstack([tiles, np.vstack([tail, pad])[np.newaxis]])
    return tiles


def from_tiled_rows(tiles: np.ndarray, orig_M: int, T: int = 8) -> np.ndarray:
    """Undo to_tiled_rows; drop padding."""
    flat = tiles.reshape(-1, tiles.shape[-1])
    return flat[:orig_M]


# ── GEMM accelerator ────────────────────────────────────────────────────

class GemmAccel:
    """8×8 INT8 systolic GEMM for 1×1 pointwise conv.

    Usage
    -----
    accel = GemmAccel(weight, zp_act, zp_w, mul, zp_out, bias)  # once per conv layer
    out = accel.run(act)                                        # per image

    Pipeline with depthwise:
    ─────────────────────────
    accel = GemmAccel(…)
    for each row-tile of depthwise output (T rows):
        accel.push_act_tile(dw_tile)   # cache A
        for col in range(Cout // 8):
            out_tile = accel.stream_b_col(col)  # MAC + requant
            write out_tile
    """

    T = 8  # systolic array dimension

    def __init__(self,
                 weight: np.ndarray,    # (Cout, Cin) int8 — pointwise weight
                 zp_act: int,           # activation zero point (subtracted in data path before MAC)
                 zp_w: int,             # weight zero point
                 mul: float,            # s_act × s_w / s_out
                 zp_out: int,           # output zero point
                 bias: np.ndarray):     # (Cout,) int32
        C_out, C_in = weight.shape

        # Pre-compute B = weight.T  (Cin, Cout) for efficient column streaming.
        # Store column-tiled: (Cout//T, Cin, T) — each tile is T columns of B.
        B = weight.astype(np.int32).T - zp_w           # (Cin, Cout)
        # Pad Cout to multiple of T
        if C_out % self.T:
            pad = np.zeros((C_in, self.T - C_out % self.T), dtype=np.int32)
            B = np.hstack([B, pad])
        self.B_tiles = np.array_split(B, B.shape[1] // self.T, axis=1)  # list of (Cin, T)

        # Pre-scale bias to INT32 per tile group (T channels per tile)
        self.bias = bias.astype(np.int32)
        if C_out % self.T:
            self.bias = np.hstack([self.bias, np.zeros(self.T - C_out % self.T, dtype=np.int32)])
        self.bias_tiles = np.array_split(self.bias, len(self.bias) // self.T)  # list of (T,)

        self.zp_act = zp_act
        self.mul = mul
        self.zp_out = zp_out
        self.C_in = C_in
        self.C_out = C_out

        # State: current activation tile cached in the array
        self._A_cache = None   # (T, Cin) int32 (zero-point subtracted)

    # ── User-facing API ──────────────────────────────────────────────────

    def load_act(self, act_tile: np.ndarray):
        """Cache one row-tile of activations.

        The zero-point subtraction happens in the data path before the MAC,
        just like in RTL: (int8_val - zp) → int32 → into the systolic array.

        act_tile : (T, C_in) INT8 — T spatial positions × Cin channels.
        """
        assert act_tile.shape == (self.T, self.C_in)
        self._A_cache = act_tile.astype(np.int32) - self.zp_act  # (T, Cin) int32

    def stream_b_col(self, col_idx: int) -> np.ndarray:
        """Stream one B column-tile through the MAC array.

        Returns (T, T) INT8 output tile (T rows × T output channels).
        """
        assert self._A_cache is not None, "load_act() must be called first"
        A = self._A_cache                                     # (T, Cin) int32
        B_col = self.B_tiles[col_idx]                        # (Cin, T) int32

        # ── 8×8 systolic MAC: INT32 accumulation ────────────────
        # In RTL: 64 PEs, each doing INT8×INT8→INT32, accumulate per PE.
        acc = A @ B_col                                       # (T, T) int32

        # ── Add bias ────────────────────────────────────────────
        acc += self.bias_tiles[col_idx]                       # (T,) broadcast

        # ── Requant to INT8 ─────────────────────────────────────
        out = np.round(acc.astype(np.float64) * self.mul).astype(np.int32) + self.zp_out
        out = np.clip(out, -128, 127).astype(np.int8)
        return out

    # ── Full layer (for verification) ───────────────────────────────

    def run(self, act: np.ndarray) -> np.ndarray:
        """Full pointwise conv.

        act : (H, W, C_in) INT8 — activation map.
        returns (H, W, C_out) INT8 — output map.
        """
        H, W, C_in = act.shape
        assert C_in == self.C_in
        M = H * W  # number of spatial positions (= rows of A)

        # Activation in tiled row layout
        A_mat = act.reshape(M, C_in)
        A_tiles = to_tiled_rows(A_mat, self.T)  # (nTiles, T, Cin)

        # Output buffer
        out_flat = np.empty((M, self.C_out), dtype=np.int8)

        for ti, A_tile in enumerate(A_tiles):
            self.load_act(A_tile)
            row_start = ti * self.T
            for ci in range(len(self.B_tiles)):
                col_start = ci * self.T
                out_tile = self.stream_b_col(ci)        # (T, T) int8
                n_out = min(self.T, M - row_start)
                n_col = min(self.T, self.C_out - col_start)
                out_flat[row_start:row_start + n_out,
                         col_start:col_start + n_col] = out_tile[:n_out, :n_col]

        return out_flat.reshape(H, W, self.C_out)


# ── Pipeline: depthwise → pointwise ──────────────────────────────────────

class DwPwPipeline:
    """Depthwise 3×3 → Pointwise 1×1 streaming pipeline.

    Depthwise produces one spatial row of (W × C) activations at a time.
    The row buffer collects T=8 rows before launching the pointwise GEMM.

    Memory:
      row_buf : (T, W, C) INT8  — double-buffered row FIFO
      gemm    : GemmAccel       — the GEMM accelerator

    Execution:
      for each row group (T rows) that depthwise produces:
        gemm.load_act(row_buf reshaped as (T, W*C))
        for each weight column tile:
          out_tile = gemm.stream_b_col(col)
          store (T, W, C_out) tile
    """

    def __init__(self, weight, zp_act, zp_w, mul, zp_out, bias, W: int, T: int = 8):
        self.gemm = GemmAccel(weight, zp_act, zp_w, mul, zp_out, bias)
        self.W = W
        self.T = T
        # row_buf[buf_id][row][col][ch] — double buffer
        self._buf = [np.empty((T, W, weight.shape[0]), dtype=np.int8) for _ in range(2)]
        self._fill_row = 0
        self._buf_id = 0

    def push_depthwise_row(self, row: np.ndarray):
        """Depthwise calls this for each output row (W, C) INT8."""
        self._buf[self._buf_id][self._fill_row] = row
        self._fill_row += 1
        if self._fill_row == self.T:
            # Buffer full: launch pointwise on this tile
            self._launch_gemm()
            self._fill_row = 0
            self._buf_id ^= 1  # swap double buffer

    def _launch_gemm(self):
        """Send the full row-tile through the GEMM."""
        tile = self._buf[self._buf_id]                 # (T, W, C_in)
        act_2d = tile.reshape(self.T, -1)              # (T, W*C_in)
        self.gemm.load_act(act_2d)
        # Stream all weight columns
        for ci in range(len(self.gemm.B_tiles)):
            self.gemm.stream_b_col(ci)
            # In real HW: DMA out the T×T INT8 tile here


# ====================================================================
# Verification
# ====================================================================

if __name__ == '__main__':
    import cv2
    from sim_pynq import YuNetSimulator
    from sim_hw import _int8

    img = cv2.imread('test.png')
    img_640 = cv2.resize(img, (640, 640))

    # Get reference pointwise (pw) parameters from the first pw stage
    from sim_hw import build_stage_table
    stages, P = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    pw_stages = [(i, s) for i, s in enumerate(stages) if s['type'] == 'pw']
    idx, s = pw_stages[0]  # first pw: 185_quantized

    print(f'Testing first pw stage: {s["out"]}')
    print(f'  zp_in={s["zp_in"]}  zp_w={s["zp_w"]}  mul={s["mul"]:.10f}  zp_out={s["zp_out"]}')
    print(f'  Cin={s["w"].shape[1]}  Cout={s["w"].shape[0]}')

    # Run sim_pynq to get the activation feeding into this pw
    sim = YuNetSimulator('face_detection_yunet_2023mar_int8.onnx')
    sim_out = sim.simulate(img_640)
    act = sim.tensors[s['inp'][0]]  # (H, W, Cin) int8
    H, W, Cin = act.shape
    print(f'  act: {act.shape} [{act.min()}, {act.max()}]')

    # GemmAccel
    weight = s['w'].reshape(s['w'].shape[0], -1)  # (Cout, Cin)
    accel = GemmAccel(weight,
                      zp_act=s['zp_in'],
                      zp_w=s['zp_w'],
                      mul=s['mul'],
                      zp_out=s['zp_out'],
                      bias=s['b_i32'])
    out_accel = accel.run(act)  # (H, W, Cout) int8

    # Reference: hw_pointwise
    from sim_hw import hw_pointwise
    out_ref = hw_pointwise(act, weight, s['b_i32'],
                           s['zp_in'], s['zp_w'],
                           s['mul'], s['zp_out'])
    out_ref = out_ref.reshape(H, W, -1).astype(np.int8)

    ok = np.all(out_accel == out_ref)
    print(f'\n  GemmAccel match hw_pointwise: {ok}')
    if not ok:
        diff = np.abs(out_accel.astype(np.int16) - out_ref.astype(np.int16))
        print(f'  maxdiff = {diff.max()}')
    else:
        print(f'  range   = [{out_accel.min()}, {out_accel.max()}]')
