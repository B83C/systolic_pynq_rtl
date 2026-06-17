#!/usr/bin/env python3
"""
Hardware-alike pipeline for YuNet 2023mar_int8.

Fabric blocks (all INT8 data path, INT32 MAC):
  - conv3×3 / depthwise / pointwise: INT8 × INT8 → INT32 MAC + INT32 bias → INT32
  - requant: INT32 → INT8  (the only place scale/zp are applied)
  - maxpool 2×2: INT8 → INT8
  - resize nearest 2×: INT8 → INT8
  - eltwise add: INT8 + INT8 → INT8  (dequant internally, add, requant)
  - sigmoid: INT8 → INT8  (LUT in hardware)

The microcode sequencer runs a pre-compiled stage table.  Each stage reads
named tensor(s) from the store and writes a named tensor — no implicit
accumulator, so branch points (one tensor → many consumers) work correctly.

Weights are stored as raw INT8; biases as raw INT32 (already in the
accumulator domain per the QLinearConv spec).
"""

import numpy as np
import onnx
import onnx.numpy_helper as nph

# ==========================================================================
# 1. HW blocks
# ==========================================================================

def _int8(x, zp):
    return np.clip(x, -128, 127).astype(np.int8)


def hw_conv3x3(inp, w, b_i32, zp_in, zp_w, mul, zp_out, stride):
    """INT8 → INT8: MAC + bias + requant all folded."""
    H, W, C_in = inp.shape
    C_out = w.shape[0]
    in_pad = np.pad(inp.astype(np.int32), ((1, 1), (1, 1), (0, 0)),
                    mode='constant', constant_values=zp_in)
    oh = (H + 2 - 3) // stride + 1
    ow = (W + 2 - 3) // stride + 1
    acc = np.zeros((oh, ow, C_out), dtype=np.int64)
    for oy in range(oh):
        for ox in range(ow):
            iy = oy * stride
            ix = ox * stride
            for ky in range(3):
                for kx in range(3):
                    act = in_pad[iy + ky, ix + kx, :] - zp_in
                    wgt = w[:, :, ky, kx] - zp_w
                    acc[oy, ox, :] += act @ wgt.T
    acc += b_i32
    q = np.round(acc.astype(np.float64) * mul).astype(np.int32) + zp_out
    return _int8(q, zp_out)


def hw_pointwise(inp, w, b_i32, zp_in, zp_w, mul, zp_out):
    """INT8 → INT8: GEMM MAC + bias + requant folded."""
    H, W, C_in = inp.shape
    C_out = w.shape[0]
    X = inp.astype(np.int32).reshape(-1, C_in) - zp_in
    Wg = w.astype(np.int32) - zp_w
    M, K = X.shape
    N = C_out
    acc = np.zeros((M, N), dtype=np.int64)
    for m0 in range(0, M, 64):
        m1 = min(m0 + 64, M)
        for k0 in range(0, K, 64):
            k1 = min(k0 + 64, K)
            for n0 in range(0, N, 64):
                n1 = min(n0 + 64, N)
                acc[m0:m1, n0:n1] += X[m0:m1, k0:k1] @ Wg[n0:n1, k0:k1].T
    acc += b_i32
    q = np.round(acc.astype(np.float64) * mul).astype(np.int32) + zp_out
    return _int8(q, zp_out).reshape(H, W, C_out)


def hw_depthwise(inp, w, b_i32, zp_in, zp_w, mul, zp_out):
    """INT8 → INT8: depthwise MAC + bias + requant folded."""
    H, W, C = inp.shape
    in_pad = np.pad(inp.astype(np.int32), ((1, 1), (1, 1), (0, 0)),
                    mode='constant', constant_values=zp_in)
    acc = np.zeros((H, W, C), dtype=np.int64)
    for oy in range(H):
        for ox in range(W):
            for ky in range(3):
                for kx in range(3):
                    av = in_pad[oy + ky, ox + kx, :] - zp_in
                    wv = w[:, ky, kx] - zp_w
                    acc[oy, ox, :] += av * wv
    acc += b_i32
    q = np.round(acc.astype(np.float64) * mul).astype(np.int32) + zp_out
    return _int8(q, zp_out)


def hw_maxpool(inp):
    """2×2 maxpool stride 2 on FP32 or INT8."""
    if inp.ndim != 3:
        H, W = inp.shape
        return inp.reshape(H // 2, 2, W // 2, 2).max(axis=(1, 3))
    H, W, C = inp.shape
    return inp.reshape(H // 2, 2, W // 2, 2, C).max(axis=(1, 3))


def hw_resize2x(inp):
    return np.repeat(np.repeat(inp, 2, axis=0), 2, axis=1)


def hw_add(a, b, zp_a, zp_b, mul_a, mul_b, zp_out):
    """INT8 + INT8 → INT8 with internal dequant/add/requant."""
    af = (a.astype(np.int32) - zp_a).astype(np.float64)
    bf = (b.astype(np.int32) - zp_b).astype(np.float64)
    out = np.round(af * mul_a + bf * mul_b).astype(np.int32) + zp_out
    return _int8(out, zp_out)


def hw_sigmoid(inp, zp_in, s_in, zp_out, s_out):
    """INT8 → INT8 via FP32 compute (LUT in HW)."""
    inp_fp = (inp.astype(np.float32) - zp_in) * s_in
    sig = 1.0 / (1.0 + np.exp(-inp_fp))
    q = np.round(sig / s_out).astype(np.int32) + zp_out
    return _int8(q, zp_out)


# ==========================================================================
# 2. Stage-table builder
# ==========================================================================

def build_stage_table(onnx_path: str):
    m = onnx.load(onnx_path)
    P = {}
    for init in m.graph.initializer:
        P[init.name] = nph.to_array(init)

    nodes = list(m.graph.node)

    # Classify each QLinearConv
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

            stage = {
                'type': ct,
                'inp': [inp[0]],
                'out': out,
                'w': w.astype(np.int8),
                'b_i32': b_i32,
                'zp_in': zp_in, 'zp_w': zp_w,
                'mul': mul,
                'zp_out': zp_out,
            }
            if ct == 'conv3x3':
                stage['stride'] = stride
            stages.append(stage)

        elif op == 'MaxPool':
            stages.append({'type': 'maxpool', 'inp': [inp[0]], 'out': out})

        elif op == 'Resize':
            stages.append({'type': 'resize', 'inp': [inp[0]], 'out': out})

        elif op == 'QLinearAdd':
            s_a = P[inp[1]].item(); zp_a = int(P[inp[2]].item())
            s_b = P[inp[4]].item(); zp_b = int(P[inp[5]].item())
            s_out = P[inp[6]].item(); zp_out = int(P[inp[7]].item())
            stages.append({
                'type': 'add',
                'inp': [inp[0], inp[3]],
                'out': out,
                'zp_a': zp_a, 'zp_b': zp_b,
                'mul_a': s_a / s_out,
                'mul_b': s_b / s_out,
                'zp_out': zp_out,
            })

        elif op == 'QLinearSigmoid':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_out = P[inp[3]].item(); zp_out = int(P[inp[4]].item())
            stages.append({
                'type': 'sigmoid',
                'inp': [inp[0]],
                'out': out,
                'zp_in': zp_in, 's_in': s_in,
                'zp_out': zp_out, 's_out': s_out,
            })

        elif op == 'QuantizeLinear':
            stages.append({'type': 'quant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})

        elif op == 'DequantizeLinear':
            stages.append({'type': 'dequant', 'inp': [inp[0]], 'out': out,
                           's': P[inp[1]].item(), 'zp': int(P[inp[2]].item())})

        elif op in ('Transpose', 'Reshape'):
            stages.append({'type': 'view', 'inp': [inp[0]], 'out': out})

    return stages, P


# ==========================================================================
# 3. Runtime — microcode sequencer
# ==========================================================================

def run_pipeline(img: np.ndarray, stages: list, P: dict,
                 verbose: bool = False):
    import time
    t0 = time.time()
    store = {}

    # Store raw image — the first QuantizeLinear stage handles quantization
    store['input'] = img
    if verbose:
        print(f'  [init] input → {img.shape} {img.dtype}  z={img.min()}/{img.max()}')

    for idx, s in enumerate(stages):
        st = time.time()
        typ = s['type']
        out = s['out']
        inputs = [store.get(n) for n in s['inp']]
        missing = [n for n, v in zip(s['inp'], inputs) if v is None]
        if missing:
            if verbose:
                print(f'  [{idx:3d}] {typ:10s} → {out}  MISSING inputs: {missing}')
            continue

        if typ == 'conv3x3':
            store[out] = hw_conv3x3(inputs[0], s['w'], s['b_i32'],
                                    s['zp_in'], s['zp_w'],
                                    s['mul'], s['zp_out'], s['stride'])
        elif typ == 'pw':
            w = s['w'].reshape(s['w'].shape[0], -1)
            store[out] = hw_pointwise(inputs[0], w, s['b_i32'],
                                      s['zp_in'], s['zp_w'],
                                      s['mul'], s['zp_out'])
        elif typ == 'dw':
            w = s['w'][:, 0, :, :]
            store[out] = hw_depthwise(inputs[0], w, s['b_i32'],
                                      s['zp_in'], s['zp_w'],
                                      s['mul'], s['zp_out'])
        elif typ == 'maxpool':
            store[out] = hw_maxpool(inputs[0])
        elif typ == 'resize':
            store[out] = hw_resize2x(inputs[0])
        elif typ == 'add':
            store[out] = hw_add(inputs[0], inputs[1],
                                s['zp_a'], s['zp_b'],
                                s['mul_a'], s['mul_b'],
                                s['zp_out'])
        elif typ == 'sigmoid':
            store[out] = hw_sigmoid(inputs[0], s['zp_in'], s['s_in'],
                                    s['zp_out'], s['s_out'])
        elif typ == 'quant':
            q = np.round(inputs[0].astype(np.float32) / s['s']).astype(np.int32) + s['zp']
            store[out] = _int8(q, s['zp'])
        elif typ == 'dequant':
            store[out] = (inputs[0].astype(np.float32) - s['zp']) * s['s']
        elif typ == 'view':
            store[out] = inputs[0]
        else:
            if verbose:
                print(f'  [{idx:3d}] ? {typ}  (skipped)')
            continue

        if verbose:
            v = store[out]
            dt = (time.time() - st) * 1000
            lo = v.min() if isinstance(v, np.ndarray) else v
            hi = v.max() if isinstance(v, np.ndarray) else v
            print(f'  [{idx:3d}] {typ:10s} → {out:30s} {str(v.shape):20s} {v.dtype} [{lo:8.4f},{hi:8.4f}] {dt:7.1f}ms')

    total = (time.time() - t0) * 1000
    final = {}
    for name in ('cls_8', 'cls_16', 'cls_32',
                 'obj_8', 'obj_16', 'obj_32',
                 'bbox_8', 'bbox_16', 'bbox_32',
                 'kps_8', 'kps_16', 'kps_32'):
        if name in store:
            final[name] = store[name]
    return final, total


# ==========================================================================
# 4. Standalone
# ==========================================================================

if __name__ == '__main__':
    import cv2, argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', '-i', type=str, required=True)
    parser.add_argument('--conf_threshold', type=float, default=0.9)
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--compare', '-c', action='store_true')
    args = parser.parse_args()

    print('Compiling stage table...')
    stages, P = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    print(f'  {len(stages)} stages')

    img = cv2.imread(args.input)
    img_640 = cv2.resize(img, (640, 640))

    print('Running pipeline...')
    outputs, t_total = run_pipeline(img_640, stages, P, verbose=args.verbose)
    print(f'Total: {t_total:.0f} ms')

    from sim_pynq import decode_faces

    if args.compare:
        from sim_pynq import YuNetSimulator
        print('\nComparing against sim_pynq...')
        sim = YuNetSimulator('face_detection_yunet_2023mar_int8.onnx')
        ref = sim.simulate(img_640)
        for name in ['cls_8', 'cls_16', 'cls_32', 'obj_8', 'obj_16', 'obj_32',
                      'bbox_8', 'bbox_16', 'bbox_32', 'kps_8', 'kps_16', 'kps_32']:
            a = outputs[name].astype(np.float32).ravel()
            b = ref[name].astype(np.float32).ravel()
            d = np.abs(a - b)
            print(f'  {name:12s} maxdiff={d.max():.6f}  meandiff={d.mean():.6f}')

    print(f'\nDetection at conf={args.conf_threshold}:')
    faces = decode_faces(outputs, conf_thresh=args.conf_threshold)
    if len(faces) == 0:
        print('  0 faces.')
    else:
        sx, sy = img.shape[1] / 640, img.shape[0] / 640
        for face in faces:
            face[0] *= sx; face[1] *= sy; face[2] *= sx; face[3] *= sy
            for i in range(5):
                face[4 + i * 2] *= sx
                face[4 + i * 2 + 1] *= sy
        for idx, det in enumerate(faces):
            print(f'  [{idx}] score={det[14]:.4f} box={det[:4].astype(int)}')
