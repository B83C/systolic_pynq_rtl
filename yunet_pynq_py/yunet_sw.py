"""
Full YuNet 2023mar INT8 software pipeline for PYNQ-Z2.

Pointwise 1×1 uses the block-major GEMM (drop-in for the FPGA).
Depthwise 3×3, first conv 3×3, and all non-GEMM ops run in software.

Usage
─────
  python3 yunet_sw.py                    # compare against OpenCV
  python3 yunet_sw.py --input test.png   # detect + show
"""

import numpy as np

# Use scipy for fast conv on ARM (C implementation, not Python-level einsum)
try:
    from scipy.ndimage import correlate as _scorrelate
    HAS_SCIPY = True
except ImportError:
    HAS_SCIPY = False

# ── Block-major GEMM (from gemm_hw.py) ──────────────────────────────

def to_block_major(matrix, bh, bw):
    H, W = matrix.shape
    reshaped = matrix.reshape(H // bh, bh, W // bw, bw)
    transposed = reshaped.transpose(0, 2, 1, 3)
    return np.ascontiguousarray(transposed)

def gemm(A_rowmajor, B, bias, mul, zp_out, zp_act):
    """Row-major weight × channel-last activation → quantized INT8.
    
    Uses float32 matmul on ARM for NEON SIMD acceleration.
    A_rowmajor : (Cout_orig, Cin_orig) int32 — zp_w pre-subtracted.
    """
    B_off = B.astype(np.float32) - zp_act
    A = A_rowmajor.astype(np.float32)
    acc = B_off @ A.T
    acc += bias.astype(np.float32)
    out = np.round(acc * mul).astype(np.int32) + zp_out
    out = np.clip(out, -128, 127).astype(np.int8)
    return out


# ── Conv blocks (vectorized with numpy) ──────────────────────────────

def _depthwise_scipy(in_off, w_3x3):
    """Per-channel 3×3 corr via scipy.ndimage.correlate (C loop)."""
    C = w_3x3.shape[0]
    ch = []
    for c in range(C):
        ch.append(_scorrelate(in_off[:, :, c], w_3x3[c],
                              mode='constant', cval=0))
    return np.stack(ch, axis=-1)

def _conv3x3_scipy(in_off, w_off):
    """Regular 3×3 corr via scipy: sum over input channels."""
    C_out, C_in, _, _ = w_off.shape
    H, W = in_off.shape[:2]
    acc = np.zeros((H, W, C_out), dtype=np.int32)
    for l in range(C_out):
        for c in range(C_in):
            acc[:, :, l] += _scorrelate(in_off[:, :, c], w_off[l, c],
                                        mode='constant', cval=0)
    return acc

def _depthwise_np(in_off, w_3x3):
    """Depthwise via sliding_window_view + einsum."""
    in_pad = np.pad(in_off, ((1, 1), (1, 1), (0, 0)), mode='constant')
    windows = np.lib.stride_tricks.sliding_window_view(in_pad, (3, 3), axis=(0, 1))
    return np.einsum('hwcij,cij->hwc', windows, w_3x3, optimize='greedy')

def _conv3x3_np(in_off, w_off, stride):
    """Regular 3×3 via sliding_window_view + einsum."""
    H, W, C_in = in_off.shape
    in_pad = np.pad(in_off, ((1, 1), (1, 1), (0, 0)), mode='constant')
    windows = np.lib.stride_tricks.sliding_window_view(in_pad, (3, 3), axis=(0, 1))
    if stride == 2:
        windows = windows[::2, ::2]
    return np.einsum('hwcij,lcij->hwl', windows, w_off, optimize='greedy')

def conv3x3(inp, w, b_i32, zp_in, zp_w, mul, zp_out, stride):
    H, W, C_in = inp.shape
    in_off = inp.astype(np.int32) - zp_in
    w_off = w.astype(np.int32) - zp_w
    if HAS_SCIPY and stride == 1:
        acc = _conv3x3_scipy(in_off, w_off)
    else:
        acc = _conv3x3_np(in_off, w_off, stride)
    acc += b_i32
    q = np.round(acc.astype(np.float32) * mul).astype(np.int32) + zp_out
    return np.clip(q, -128, 127).astype(np.int8)

def depthwise3x3(inp, w, b_i32, zp_in, zp_w, mul, zp_out):
    H, W, C = inp.shape
    in_off = inp.astype(np.int32) - zp_in
    w_3x3 = w[:, 0, :, :].astype(np.int32) - zp_w
    if HAS_SCIPY and C <= 128:
        acc = _depthwise_scipy(in_off, w_3x3)
    else:
        acc = _depthwise_np(in_off, w_3x3)
    acc += b_i32
    q = np.round(acc.astype(np.float32) * mul).astype(np.int32) + zp_out
    return np.clip(q, -128, 127).astype(np.int8)


# ── Other ops ────────────────────────────────────────────────────────

def maxpool2x2(inp):
    return inp.reshape(inp.shape[0] // 2, 2,
                       inp.shape[1] // 2, 2,
                       inp.shape[2]).max(axis=(1, 3))

def resize2x(inp):
    return np.repeat(np.repeat(inp, 2, axis=0), 2, axis=1)

def eltwise_add(a, b, zp_a, zp_b, mul_a, mul_b, zp_out):
    af = (a.astype(np.int32) - zp_a).astype(np.float32)
    bf = (b.astype(np.int32) - zp_b).astype(np.float32)
    out = np.round(af * mul_a + bf * mul_b).astype(np.int32) + zp_out
    return np.clip(out, -128, 127).astype(np.int8)

def sigmoid_int8(inp, zp_in, s_in, zp_out, s_out):
    in_fp = (inp.astype(np.float32) - zp_in) * s_in
    sig = 1.0 / (1.0 + np.exp(-in_fp))
    q = np.round(sig / s_out).astype(np.int32) + zp_out
    return np.clip(q, -128, 127).astype(np.int8)


# ═════════════════════════════════════════════════════════════════════
#  Stage table  (from ONNX graph)
# ═════════════════════════════════════════════════════════════════════

def build_stage_table(onnx_path, T=8):
    import onnx, onnx.numpy_helper as nph
    m = onnx.load(onnx_path)
    P = {}
    for init in m.graph.initializer:
        P[init.name] = nph.to_array(init)

    nodes = list(m.graph.node)

    # Classify conv types
    ctype = {}
    for n in nodes:
        if n.op_type != 'QLinearConv':
            continue
        w = P[n.input[3]]
        if w.ndim != 4:
            continue
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
            if a.name == name:
                return list(a.ints)
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
            # For pointwise: pre-compute weight in both formats
            if ct == 'pw':
                weight_2d = w.reshape(w.shape[0], -1)
                stage['A_block'] = _prepare_weight(weight_2d, zp_w, T)
                # Row-major with zp_w pre-subtracted for the fast GEMM path
                stage['A_rm'] = (weight_2d.astype(np.int32) - zp_w)
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
                           'mul_a': s_a / s_out, 'mul_b': s_b / s_out,
                           'zp_out': zp_out})

        elif op == 'QLinearSigmoid':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_out = P[inp[3]].item(); zp_out = int(P[inp[4]].item())
            stages.append({'type': 'sigmoid', 'inp': [inp[0]], 'out': out,
                           'zp_in': zp_in, 's_in': s_in,
                           'zp_out': zp_out, 's_out': s_out})

        elif op == 'QuantizeLinear':
            stages.append({'type': 'quant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})

        elif op == 'DequantizeLinear':
            stages.append({'type': 'dequant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})

        elif op in ('Transpose', 'Reshape'):
            stages.append({'type': 'view', 'inp': [inp[0]], 'out': out})

    return stages, P


def _prepare_weight(weight, zp_w, T):
    """Pre-subtract zp_w, pad to tile boundary, convert to block-major."""
    Cout, Cin = weight.shape
    Cp = ((Cout + T - 1) // T) * T
    Kp = ((Cin  + T - 1) // T) * T
    w = np.zeros((Cp, Kp), dtype=np.int8)
    w[:Cout, :Cin] = weight
    w_off = w.astype(np.int16) - zp_w
    w_off = np.clip(w_off, -128, 127).astype(np.int8)
    return to_block_major(w_off, T, T)


# ═════════════════════════════════════════════════════════════════════
#  Pipeline runner
# ═════════════════════════════════════════════════════════════════════

def run_pipeline(img, stages, P, verbose=False):
    store = {'input': img}

    for idx, s in enumerate(stages):
        typ = s['type']
        out = s['out']
        inputs = [store.get(n) for n in s['inp']]
        if any(v is None for v in inputs):
            continue

        if typ == 'conv3x3':
            store[out] = conv3x3(inputs[0], s['w'], s['b_i32'],
                                 s['zp_in'], s['zp_w'], s['mul'], s['zp_out'],
                                 s.get('stride', 1))
        elif typ == 'dw':
            store[out] = depthwise3x3(inputs[0], s['w'], s['b_i32'],
                                      s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
        elif typ == 'pw':
            H, W = inputs[0].shape[0], inputs[0].shape[1]
            B = inputs[0].reshape(-1, inputs[0].shape[2])  # (HW, Cin)
            out_flat = gemm(s['A_rm'], B, s['b_i32'],
                            s['mul'], s['zp_out'], s['zp_in'])
            store[out] = out_flat.reshape(H, W, -1)   # (H, W, Cout)
        elif typ == 'maxpool':
            store[out] = maxpool2x2(inputs[0])
        elif typ == 'resize':
            store[out] = resize2x(inputs[0])
        elif typ == 'add':
            store[out] = eltwise_add(inputs[0], inputs[1],
                                     s['zp_a'], s['zp_b'],
                                     s['mul_a'], s['mul_b'], s['zp_out'])
        elif typ == 'sigmoid':
            store[out] = sigmoid_int8(inputs[0], s['zp_in'], s['s_in'],
                                      s['zp_out'], s['s_out'])
        elif typ == 'quant':
            q = np.round(inputs[0].astype(np.float32) / s['s']).astype(np.int32) + s['zp']
            store[out] = np.clip(q, -128, 127).astype(np.int8)
        elif typ == 'dequant':
            store[out] = (inputs[0].astype(np.float32) - s['zp']) * s['s']
        elif typ == 'view':
            store[out] = inputs[0]

        if verbose and typ not in ('view',):
            v = store[out]
            lo = v.min() if isinstance(v, np.ndarray) else v
            hi = v.max() if isinstance(v, np.ndarray) else v
            print(f'  [{idx:3d}] {typ:10s} {out:30s} {str(v.shape):20s} [{lo:8.4f},{hi:8.4f}]')

    final = {}
    for name in ('cls_8', 'cls_16', 'cls_32',
                 'obj_8', 'obj_16', 'obj_32',
                 'bbox_8', 'bbox_16', 'bbox_32',
                 'kps_8', 'kps_16', 'kps_32'):
        if name in store:
            final[name] = store[name]
    return final


# ── Decode faces (from sim_pynq) ─────────────────────────────────────

def decode_faces(outputs, conf_thresh=0.9):
    import math
    faces = []
    for scale, (obj_name, cls_name, bbox_name, kps_name) in enumerate([
        ('obj_8', 'cls_8', 'bbox_8', 'kps_8'),
        ('obj_16', 'cls_16', 'bbox_16', 'kps_16'),
        ('obj_32', 'cls_32', 'bbox_32', 'kps_32'),
    ], 1):
        obj = outputs[obj_name].squeeze()
        cls = outputs[cls_name].squeeze()
        bbox = outputs[bbox_name].squeeze()
        kps = outputs[kps_name].squeeze()
        bbox = bbox.reshape(-1, 4) if bbox.ndim > 2 else bbox
        kps = kps.reshape(-1, 10) if kps.ndim > 2 else kps
        scores = np.sqrt(obj.ravel() * cls.ravel())
        stride = 2 ** (scale + 2)
        grid = 640 // stride
        for i in range(len(scores)):
            if scores[i] < conf_thresh:
                continue
            cx = (i % grid + 0.5) * stride
            cy = (i // grid + 0.5) * stride
            w = bbox[i, 0] * stride
            h = bbox[i, 1] * stride
            x1 = cx - w / 2
            y1 = cy - h / 2
            face = [x1, y1, w, h]
            for j in range(5):
                face.append(cx + kps[i, j * 2] * stride)
                face.append(cy + kps[i, j * 2 + 1] * stride)
            face.append(scores[i])
            faces.append(face)
    return faces


# ═════════════════════════════════════════════════════════════════════
#  Main
# ═════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    import cv2, sys, time

    # ── Parse args ─────────────────────────────────────────────────
    img_path = 'test.png'
    verbose = False
    skip_cv = False
    for a in sys.argv[1:]:
        if a.startswith('--input='):
            img_path = a.split('=', 1)[1]
        elif a == '--verbose' or a == '-v':
            verbose = True
        elif a == '--skip-cv':
            skip_cv = True

    # ── Load and pre-process ───────────────────────────────────────
    print('Compiling stage table...')
    t0 = time.time()
    stages, P = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    print(f'  {len(stages)} stages  ({(time.time() - t0) * 1000:.0f} ms)')

    img = cv2.imread(img_path)
    if img is None:
        print(f'ERROR: cannot read {img_path}')
        sys.exit(1)
    img_640 = cv2.resize(img, (640, 640))

    print('Running pipeline...')
    t0 = time.time()
    outputs = run_pipeline(img_640, stages, P, verbose=verbose)
    t_pipe = (time.time() - t0) * 1000
    print(f'  Done  ({t_pipe:.0f} ms)')

    # ── Compare against OpenCV ─────────────────────────────────────
    if not skip_cv:
        print('\nComparing against OpenCV DNN...')
        try:
            import cv2.dnn
            net = cv2.dnn.readNetFromONNX('face_detection_yunet_2023mar_int8.onnx')
            blob = cv2.dnn.blobFromImage(img_640, 1.0, (640, 640), swapRB=False)
            net.setInput(blob)
            out_names = ['cls_8','cls_16','cls_32','obj_8','obj_16','obj_32',
                         'bbox_8','bbox_16','bbox_32','kps_8','kps_16','kps_32']
            ref = net.forward(out_names)

            for i, name in enumerate(out_names):
                v1 = outputs[name].astype(np.float32).ravel()
                v2 = ref[i].astype(np.float32).ravel()
                d = np.abs(v1 - v2)
                print(f'  {name:12s} maxdiff={d.max():.6f}  meandiff={d.mean():.6f}')
        except Exception as e:
            print(f'  OpenCV comparison skipped: {e}')

    # ── Face detection ─────────────────────────────────────────────
    print(f'\nDetection at conf=0.9:')
    faces = decode_faces(outputs, conf_thresh=0.9)
    if len(faces) == 0:
        print('  0 faces.')
    else:
        sx, sy = img.shape[1] / 640, img.shape[0] / 640
        for det in faces:
            det[0] *= sx; det[1] *= sy
            det[2] *= sx; det[3] *= sy
            for j in range(5):
                det[4 + j * 2] *= sx
                det[4 + j * 2 + 1] *= sy
        for idx, det in enumerate(faces):
            print(f'  [{idx}] score={det[14]:.4f} box={np.array(det[:4]).astype(int)}')
