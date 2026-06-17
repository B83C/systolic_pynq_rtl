"""
Load exported YuNet stages from .npz — no onnx, no pickle.
Old numpy (~1.24) compatible.
"""
import numpy as np


def _fix_inp(val):
    """Parse inp string back to list or str."""
    if val.startswith('['):
        return eval(val)
    return val


def load_stages(path='yunet_stages.npz'):
    data = np.load(path)
    stages = []
    n = len(data['types'])
    for i in range(n):
        typ = str(data['types'][i])
        inp_raw = _fix_inp(str(data['inps'][i]))
        if isinstance(inp_raw, str):
            inp_raw = [inp_raw]
        s = {'type': typ, 'inp': inp_raw, 'out': str(data['outs'][i])}

        if data['has_weight'][i]:
            wk = f'w_{i:03d}'; bk = f'b_{i:03d}'
            s['w'] = data[wk]
            s['b_i32'] = data[bk].ravel()
            s['zp_in'] = int(data['zp_in'][i])
            s['zp_w'] = int(data['zp_w'][i])
            s['mul'] = float(data['mul'][i])
            s['zp_out'] = int(data['zp_out'][i])
            st = int(data['stride'][i])
            if st:
                s['stride'] = st

        elif typ == 'add':
            p = data['add_par'][i]
            s.update(zp_a=int(p[0]), zp_b=int(p[1]),
                     mul_a=float(p[2]), mul_b=float(p[3]),
                     zp_out=int(p[4]))

        elif typ == 'sigmoid':
            p = data['sig_par'][i]
            s.update(zp_in=int(p[0]), s_in=float(p[1]),
                     zp_out=int(p[2]), s_out=float(p[3]))

        elif typ in ('quant', 'dequant'):
            p = data['qdq_par'][i]
            s.update(s=float(p[0]), zp=int(p[1]))

        stages.append(s)
    return stages
