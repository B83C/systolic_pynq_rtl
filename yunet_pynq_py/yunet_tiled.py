"""
YuNet INT8 pipeline — tiled memory layout end-to-end.

All activations stored as (H, W//4, C*4):
  per tile: [ch0:px0..3] [ch1:px0..3] ... [ch{C-1}:px0..3]

No layout conversion in the inner loop. Depthwise and pointwise
both operate directly on the same tiled representation.
Tile width 4 divides all feature map widths (640..20).
"""

import numpy as np

try:
    from scipy.ndimage import correlate as _correlate
    HAS_SCIPY = True
except ImportError:
    HAS_SCIPY = False

TW = 4


# ── Entry / exit conversions ────────────────────────────────────────

def to_tiled(act):
    """(H, W, C) → (H, W//8, C*8)  W must be divisible by 8."""
    H, W, C = act.shape
    assert W % TW == 0
    nx = W // TW
    return np.ascontiguousarray(
        act.reshape(H, nx, TW, C).transpose(0, 1, 3, 2).reshape(H, nx, C * TW))

def from_tiled(tiled, W_orig=None):
    """(H, nx, C*8) → (H, W, C). Crops to W_orig if given."""
    H, nx, _ = tiled.shape
    C = tiled.shape[2] // TW
    W = nx * TW
    hwc = tiled.reshape(H, nx, C, TW).transpose(0, 1, 3, 2).reshape(H, W, C)
    if W_orig is not None and W_orig < W:
        hwc = hwc[:, :W_orig, :]
    return hwc

# ── Ops (all work directly on tiled layout) ─────────────────────────

def _depthwise_tiled(inp_t, w_3x3, b_i32, zp_in, zp_w, mul, zp_out):
    """Depthwise 3×3 on tiled layout.
    
    Accesses pixels via: val = inp_t[y, x//8, c*8 + x%8]
    Uses scipy correlate per channel after a no-copy reshape.
    """
    H, nx, _ = inp_t.shape
    C = inp_t.shape[2] // TW
    W = nx * TW
    w_off = w_3x3.astype(np.float32) - zp_w
    acc = np.zeros((H, W, C), dtype=np.float32)

    for c in range(C):
        # Extract channel c: (H, nx, 8) → (H, W) — no copy (contiguous slice)
        ch_slice = inp_t[:, :, c*TW:(c+1)*TW]     # (H, nx, TW)
        ch = ch_slice.reshape(H, W).astype(np.float32) - zp_in
        acc[:, :, c] = _correlate(ch, w_off[c], mode='constant', cval=0)

    acc += b_i32.astype(np.float32)
    q = np.round(acc * mul).astype(np.int32) + zp_out
    return to_tiled(np.clip(q, -128, 127).astype(np.int8))

def _pw_gemm_tiled(inp_t, A_rm, bias, mul, zp_out, zp_act):
    """Pointwise GEMM on tiled layout.
    
    Processes tile by tile: each tile = (TW, C) = one spatial group.
    No flatten to HW×C — the tile loop IS the hardware FSM.
    """
    H, nx, _ = inp_t.shape
    C_in = A_rm.shape[1]
    C_out = A_rm.shape[0]
    W = nx * TW
    M = H * nx  # number of tiles

    # Output in tiled format: (H, nx, C_out*TW)
    out_t = np.empty((H, nx, C_out * TW), dtype=np.int8)

    for y in range(H):
        for tx in range(nx):
            tile = inp_t[y, tx].astype(np.float32)        # (C_in*TW,)
            A = tile.reshape(C_in, TW).T                   # (TW, C_in)
            acc = (A - zp_act) @ A_rm.T.astype(np.float32) # (TW, C_out)
            acc += bias.astype(np.float32)
            acc = np.round(acc * mul).astype(np.int32) + zp_out
            out_t[y, tx] = np.clip(acc, -128, 127).astype(np.int8).T.reshape(-1)

    return out_t

def _maxpool_tiled(inp_t):
    """2×2 maxpool on tiled layout. All widths divisible by TW."""
    H, nx, _ = inp_t.shape
    C = inp_t.shape[2] // TW
    W = nx * TW
    hwc = from_tiled(inp_t)
    pooled = hwc.reshape(H // 2, 2, W // 2, 2, C).max(axis=(1, 3))
    return to_tiled(pooled)


# ── Stage table ─────────────────────────────────────────────────────

def build_stage_table(onnx_path):
    import onnx, onnx.numpy_helper as nph
    m = onnx.load(onnx_path)
    P = {}
    for init in m.graph.initializer:
        P[init.name] = nph.to_array(init)
    nodes = list(m.graph.node)

    ctype = {}
    for n in nodes:
        if n.op_type != 'QLinearConv': continue
        w = P[n.input[3]]
        if w.ndim != 4: continue
        ks = (w.shape[2], w.shape[3])
        if ks == (1, 1):
            ctype[n.output[0]] = 'pw'
        elif w.shape[1] == 1:
            ctype[n.output[0]] = 'dw'
        else:
            ctype[n.output[0]] = 'conv3x3'

    stages = []
    def attr_ints(n, name, default=None):
        for a in n.attribute:
            if a.name == name: return list(a.ints)
        return default

    for n in nodes:
        op = n.op_type
        inp = list(n.input)
        out = n.output[0]

        if op == 'QLinearConv':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_w   = P[inp[4]].item(); zp_w   = int(P[inp[5]].item())
            s_out = P[inp[6]].item(); zp_out = int(P[inp[7]].item())
            w = P[inp[3]]
            b_i32 = P[inp[8]].astype(np.int64).ravel()
            mul = (s_in * s_w) / s_out
            strides = attr_ints(n, 'strides', [1, 1])
            stride = strides[0]
            ct = ctype.get(out, 'pw')
            stage = {'type': ct, 'inp': [inp[0]], 'out': out,
                     'w': w, 'b_i32': b_i32,
                     'zp_in': zp_in, 'zp_w': zp_w, 'mul': mul, 'zp_out': zp_out}
            if ct == 'conv3x3' or ct == 'dw':
                stage['stride'] = stride
            if ct == 'pw':
                stage['A_rm'] = w.reshape(w.shape[0], -1).astype(np.int32) - zp_w
            stages.append(stage)

        elif op == 'MaxPool':
            stages.append({'type': 'maxpool', 'inp': [inp[0]], 'out': out})
        elif op == 'Resize':
            stages.append({'type': 'resize', 'inp': [inp[0]], 'out': out})
        elif op == 'QLinearAdd':
            s_a = P[inp[1]].item(); zp_a = int(P[inp[2]].item())
            s_b = P[inp[4]].item(); zp_b = int(P[inp[5]].item())
            s_out = P[inp[6]].item(); zp_out = int(P[inp[7]].item())
            stages.append({'type': 'add', 'inp': [inp[0], inp[3]], 'out': out,
                           'zp_a': zp_a, 'zp_b': zp_b,
                           'mul_a': s_a / s_out, 'mul_b': s_b / s_out, 'zp_out': zp_out})
        elif op == 'QLinearSigmoid':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_out = P[inp[3]].item(); zp_out = int(P[inp[4]].item())
            stages.append({'type': 'sigmoid', 'inp': [inp[0]], 'out': out,
                           'zp_in': zp_in, 's_in': s_in, 'zp_out': zp_out, 's_out': s_out})
        elif op == 'QuantizeLinear':
            stages.append({'type': 'quant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})
        elif op == 'DequantizeLinear':
            stages.append({'type': 'dequant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})
        elif op in ('Transpose', 'Reshape'):
            stages.append({'type': 'view', 'inp': [inp[0]], 'out': out})

    return stages, P


# ── Pipeline runner ─────────────────────────────────────────────────

def run_pipeline(img, stages, P):
    store = {'input': img}
    is_tiled = {}  # track per-tensor whether it's in tiled format

    for idx, s in enumerate(stages):
        typ = s['type']
        out = s['out']
        inp = [store.get(n) for n in s['inp']]
        if any(v is None for v in inp):
            continue

        if typ == 'conv3x3':
            H, nx, _ = inp[0].shape
            W = nx * TW
            hwc = from_tiled(inp[0])
            in_off = hwc.astype(np.int32) - s['zp_in']
            in_pad = np.pad(in_off, ((1, 1), (1, 1), (0, 0)), mode='constant')
            w_off = s['w'].astype(np.int32) - s['zp_w']
            windows = np.lib.stride_tricks.sliding_window_view(
                in_pad, (3, 3), axis=(0, 1))[::2, ::2]
            acc = np.einsum('hwcij,lcij->hwl', windows, w_off, optimize='greedy')
            acc += s['b_i32']
            q = np.round(acc.astype(np.float32) * s['mul']).astype(np.int32) + s['zp_out']
            store[out] = to_tiled(np.clip(q, -128, 127).astype(np.int8))
            is_tiled[out] = True

        elif typ == 'dw':
            w3 = s['w'][:, 0, :, :]
            store[out] = _depthwise_tiled(
                inp[0], w3, s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
            is_tiled[out] = True

        elif typ == 'pw':
            store[out] = _pw_gemm_tiled(
                inp[0], s['A_rm'], s['b_i32'], s['mul'], s['zp_out'], s['zp_in'])
            is_tiled[out] = True

        elif typ == 'maxpool':
            store[out] = _maxpool_tiled(inp[0])
            is_tiled[out] = True

        elif typ == 'resize':
            hwc = from_tiled(inp[0])
            up = np.repeat(np.repeat(hwc, 2, axis=0), 2, axis=1)
            store[out] = to_tiled(up)
            is_tiled[out] = True

        elif typ == 'add':
            af = (inp[0].astype(np.int32) - s['zp_a']).astype(np.float32)
            bf = (inp[1].astype(np.int32) - s['zp_b']).astype(np.float32)
            store[out] = np.clip(
                np.round(af * s['mul_a'] + bf * s['mul_b']).astype(np.int32) + s['zp_out'],
                -128, 127).astype(np.int8)
            is_tiled[out] = True

        elif typ == 'sigmoid':
            in_fp = (inp[0].astype(np.float32) - s['zp_in']) * s['s_in']
            sig = 1.0 / (1.0 + np.exp(-in_fp))
            store[out] = np.clip(
                np.round(sig / s['s_out']).astype(np.int32) + s['zp_out'],
                -128, 127).astype(np.int8)
            is_tiled[out] = True

        elif typ == 'quant':
            q = np.round(inp[0].astype(np.float32) / s['s']).astype(np.int32) + s['zp']
            out_i8 = np.clip(q, -128, 127).astype(np.int8)
            if is_tiled.get(s['inp'][0], False):
                store[out] = out_i8  # already tiled
            else:
                store[out] = to_tiled(out_i8)  # first quant: tile it
            is_tiled[out] = True

        elif typ == 'dequant':
            store[out] = (inp[0].astype(np.float32) - s['zp']) * s['s']

        elif typ == 'view':
            store[out] = inp[0].copy()

    # Convert tiled outputs → HWC for face decode
    final = {}
    for name in ('cls_8','cls_16','cls_32','obj_8','obj_16','obj_32',
                 'bbox_8','bbox_16','bbox_32','kps_8','kps_16','kps_32'):
        if name in store:
            v = store[name]
            if v.ndim == 3 and v.shape[2] <= 10:
                final[name] = v
            elif v.ndim == 3:
                final[name] = from_tiled(v)
            else:
                final[name] = v
    return final


if __name__ == '__main__':
    import cv2, sys, time
    img_path = 'test.png'
    for a in sys.argv[1:]:
        if a.startswith('--input='):
            img_path = a.split('=', 1)[1]

    stages, P = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    img = cv2.resize(cv2.imread(img_path), (640, 640))

    print('Running tiled pipeline...')
    t0 = time.time()
    outputs = run_pipeline(img, stages, P)
    t = (time.time() - t0) * 1000
    print(f'Done ({t:.0f} ms)')

    print('\nComparing against OpenCV DNN...')
    net = cv2.dnn.readNetFromONNX('face_detection_yunet_2023mar_int8.onnx')
    blob = cv2.dnn.blobFromImage(img, 1.0, (640, 640), swapRB=False)
    net.setInput(blob)
    out_names = ['cls_8','cls_16','cls_32','obj_8','obj_16','obj_32',
                 'bbox_8','bbox_16','bbox_32','kps_8','kps_16','kps_32']
    ref = net.forward(out_names)
    all_ok = True
    for i, name in enumerate(out_names):
        v1 = outputs[name].astype(np.float32).ravel()
        v2 = ref[i].astype(np.float32).ravel()
        d = np.abs(v1 - v2)
        ok = d.max() < 0.2
        if not ok: all_ok = False
        print(f'  {name:12s} [{"OK" if ok else "CHECK"}] maxdiff={d.max():.6f}')
    print(f'\n{"ALL MATCH" if all_ok else "SOME DEVIATIONS"}')
