"""
Export YuNet INT8 ONNX parameters to .npz for PYNQ (no onnx, no pickle).
Each weight saved as a separate array with shape recovery metadata.
"""
import numpy as np
import onnx
import onnx.numpy_helper as nph


def attr_ints(n, name, default=None):
    for a in n.attribute:
        if a.name == name:
            return list(a.ints)
    return default


def export_stages(onnx_path, out_path='yunet_stages.npz'):
    m = onnx.load(onnx_path)
    P = {}
    for init in m.graph.initializer:
        P[init.name] = nph.to_array(init)

    nodes = list(m.graph.node)

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
    for n in nodes:
        op = n.op_type
        inp = list(n.input)
        out = n.output[0]

        if op == 'QLinearConv':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_w   = P[inp[4]].item(); zp_w   = int(P[inp[5]].item())
            s_out = P[inp[6]].item(); zp_out = int(P[inp[7]].item())
            w = P[inp[3]]
            b_i32 = P[inp[8]].astype(np.int32).ravel()
            mul = (s_in * s_w) / s_out
            strides = attr_ints(n, 'strides', [1, 1])
            stride = strides[0]
            ct = ctype.get(out, 'pw')

            stage = {
                'type': ct, 'inp': inp[0], 'out': out,
                'w': w.astype(np.int8),
                'b': b_i32,
                'zp_in': zp_in, 'zp_w': zp_w,
                'mul': mul, 'zp_out': zp_out,
            }
            if ct in ('conv3x3', 'dw'):
                stage['stride'] = stride
            stages.append(stage)

        elif op == 'MaxPool':
            stages.append({'type': 'maxpool', 'inp': inp[0], 'out': out})

        elif op == 'Resize':
            stages.append({'type': 'resize', 'inp': inp[0], 'out': out})

        elif op == 'QLinearAdd':
            s_a = P[inp[1]].item(); zp_a = int(P[inp[2]].item())
            s_b = P[inp[4]].item(); zp_b = int(P[inp[5]].item())
            s_out = P[inp[6]].item(); zp_out = int(P[inp[7]].item())
            stages.append({
                'type': 'add',
                'inp': [inp[0], inp[3]], 'out': out,
                'zp_a': zp_a, 'zp_b': zp_b,
                'mul_a': float(s_a / s_out), 'mul_b': float(s_b / s_out),
                'zp_out': zp_out,
            })

        elif op == 'QLinearSigmoid':
            s_in  = P[inp[1]].item(); zp_in  = int(P[inp[2]].item())
            s_out = P[inp[3]].item(); zp_out = int(P[inp[4]].item())
            stages.append({
                'type': 'sigmoid', 'inp': inp[0], 'out': out,
                'zp_in': zp_in, 's_in': float(s_in),
                'zp_out': zp_out, 's_out': float(s_out),
            })

        elif op == 'QuantizeLinear':
            stages.append({
                'type': 'quant', 'inp': inp[0], 'out': out,
                's': float(P[inp[1]].item()),
                'zp': int(P[inp[2]].item()),
            })

        elif op == 'DequantizeLinear':
            stages.append({
                'type': 'dequant', 'inp': inp[0], 'out': out,
                's': float(P[inp[1]].item()),
                'zp': int(P[inp[2]].item()),
            })

        elif op in ('Transpose', 'Reshape'):
            stages.append({'type': 'view', 'inp': inp[0], 'out': out})

    # ── Build flat arrays ────────────────────────────────────────────
    n = len(stages)
    types = np.array([s['type'] for s in stages], dtype='U10')
    inps = np.array([str(s.get('inp', '')) for s in stages], dtype='U80')
    outs = np.array([s['out'] for s in stages], dtype='U80')

    has_weight = np.zeros(n, dtype=bool)
    zp_ins = np.zeros(n, dtype=np.int32)
    zp_ws = np.zeros(n, dtype=np.int32)
    muls = np.zeros(n, dtype=np.float64)
    zp_outs = np.zeros(n, dtype=np.int32)
    strides = np.zeros(n, dtype=np.int32)
    add_par = np.zeros((n, 5), dtype=np.float64)
    sig_par = np.zeros((n, 4), dtype=np.float64)
    qdq_par = np.zeros((n, 2), dtype=np.float64)

    weight_keys = []
    bias_keys = []
    for i, s in enumerate(stages):
        if 'w' in s:
            w = s['w']
            b = s['b']
            has_weight[i] = True
            wk = f'w_{i:03d}'; bk = f'b_{i:03d}'
            weight_keys.append(wk)
            bias_keys.append(bk)
            zp_ins[i] = s['zp_in']
            zp_ws[i] = s['zp_w']
            muls[i] = s['mul']
            zp_outs[i] = s['zp_out']
            strides[i] = s.get('stride', 0)

        if s['type'] == 'add':
            add_par[i] = [s['zp_a'], s['zp_b'], s['mul_a'], s['mul_b'], s['zp_out']]
        if s['type'] == 'sigmoid':
            sig_par[i] = [s['zp_in'], s['s_in'], s['zp_out'], s['s_out']]
        if s['type'] in ('quant', 'dequant'):
            qdq_par[i] = [s['s'], s['zp']]

    save_dict = dict(
        types=types, inps=inps, outs=outs,
        has_weight=has_weight,
        zp_in=zp_ins, zp_w=zp_ws,
        mul=muls, zp_out=zp_outs,
        stride=strides,
        add_par=add_par, sig_par=sig_par, qdq_par=qdq_par,
        weight_keys=np.array(weight_keys, dtype='U10'),
        bias_keys=np.array(bias_keys, dtype='U10'),
    )

    # Add weights and biases as separate arrays
    for i, s in enumerate(stages):
        if 'w' in s:
            save_dict[f'w_{i:03d}'] = s['w']
            save_dict[f'b_{i:03d}'] = s['b']

    np.savez(out_path, **save_dict)
    print(f'Exported {n} stages to {out_path}')
    print(f'  weights: {len(weight_keys)}')


if __name__ == '__main__':
    import sys
    model = sys.argv[1] if len(sys.argv) > 1 else 'face_detection_yunet_2023mar_int8.onnx'
    out = sys.argv[2] if len(sys.argv) > 2 else 'yunet_stages.npz'
    export_stages(model, out)
