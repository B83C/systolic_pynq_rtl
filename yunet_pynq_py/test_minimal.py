"""Minimal SW pipeline test — run on PYNQ to verify."""
import numpy as np, cv2, time
from load_stages import load_stages
from yunet_sw import conv3x3, depthwise3x3, maxpool2x2, resize2x, eltwise_add, sigmoid_int8, decode_faces

stages = load_stages('yunet_stages.npz')
img = cv2.resize(cv2.imread('test.png'), (640, 640))

store = {'input': img}
for s in stages:
    typ = s['type']; out = s['out']
    inp_names = s['inp'] if isinstance(s['inp'], list) else [s['inp']]
    inputs = [store.get(n) for n in inp_names]
    if any(v is None for v in inputs): continue
    if typ == 'conv3x3':
        store[out] = conv3x3(inputs[0], s['w'], s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'], s.get('stride',1))
    elif typ == 'dw':
        store[out] = depthwise3x3(inputs[0], s['w'], s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
    elif typ == 'pw':
        H,W,C = inputs[0].shape
        w = s['w'].reshape(s['w'].shape[0], -1)
        B = inputs[0].reshape(-1, C)
        acc = (B.astype(np.float32)-s['zp_in']) @ (w.astype(np.float32)-s['zp_w']).T + s['b_i32'].astype(np.float32)
        store[out] = np.clip(np.round(acc*s['mul']).astype(np.int32)+s['zp_out'], -128, 127).astype(np.int8).reshape(H,W,-1)
    elif typ == 'maxpool': store[out] = maxpool2x2(inputs[0])
    elif typ == 'resize': store[out] = resize2x(inputs[0])
    elif typ == 'add':
        store[out] = eltwise_add(inputs[0], inputs[1], s['zp_a'], s['zp_b'], s['mul_a'], s['mul_b'], s['zp_out'])
    elif typ == 'sigmoid':
        store[out] = sigmoid_int8(inputs[0], s['zp_in'], s['s_in'], s['zp_out'], s['s_out'])
    elif typ == 'quant':
        q = np.round(inputs[0].astype(np.float32)/s['s']).astype(np.int32)+s['zp']
        store[out] = np.clip(q, -128, 127).astype(np.int8)
    elif typ == 'dequant':
        store[out] = (inputs[0].astype(np.float32)-s['zp'])*s['s']
    elif typ == 'view':
        store[out] = inputs[0]

final = {}
for n in ('cls_8','cls_16','cls_32','obj_8','obj_16','obj_32','bbox_8','bbox_16','bbox_32','kps_8','kps_16','kps_32'):
    if n in store: final[n] = store[n]

faces = decode_faces(final, conf_thresh=0.9)
print(f"{len(faces)} faces")
for i, det in enumerate(faces):
    print(f"  [{i}] box={det[:4]} score={det[14]:.4f}")
