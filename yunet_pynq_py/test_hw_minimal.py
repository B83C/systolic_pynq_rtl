"""HW+NEON pipeline test — using Python DMA for SA (stable)."""
import numpy as np, cv2, time, sys
from load_stages import load_stages
from yunet_sw import conv3x3 as sw_cv3, depthwise3x3 as sw_dw, maxpool2x2, resize2x, eltwise_add, sigmoid_int8, decode_faces
from yunet_pynq import HwPointwise, find_quant_params, sw_pointwise, _hw_depthwise, _hw_conv3x3

print("Loading stages...")
stages = load_stages('yunet_stages.npz')
print(f"  {len(stages)} stages")

print("Initializing HW...")
from pynq import Overlay, MMIO
ol = Overlay("systolic.bit")
mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)
hw_pw = HwPointwise(mmio, mmio_ch, mmio_q, mmio_tc, ol.axi_dma_0)
print("  SA + NEON ready")

img = cv2.resize(cv2.imread('test.png'), (640, 640))
store = {'input': img}

for idx, s in enumerate(stages):
    typ = s['type']; out = s['out']
    inp_names = s['inp'] if isinstance(s['inp'], list) else [s['inp']]
    inputs = [store.get(n) for n in inp_names]
    if any(v is None for v in inputs): continue
    
    t0 = time.time()
    try:
        if typ == 'conv3x3':
            store[out] = _hw_conv3x3(inputs[0], s['w'], s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'], s.get('stride',1))
        elif typ == 'dw':
            store[out] = _hw_depthwise(inputs[0], s['w'], s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
        elif typ == 'pw':
            if inputs[0].shape[2] >= 8:
                hw_pw.load_layer(s['w'], s['b_i32'], s['zp_w'])
                store[out] = hw_pw.run(inputs[0], s['zp_in'], s['mul'], s['zp_out'])
            else:
                store[out] = sw_pointwise(inputs[0], s['w'], s['b_i32'], s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
        elif typ == 'maxpool': store[out] = maxpool2x2(inputs[0])
        elif typ == 'resize': store[out] = resize2x(inputs[0])
        elif typ == 'add':
            store[out] = eltwise_add(inputs[0], inputs[1], s['zp_a'], s['zp_b'], s['mul_a'], s['mul_b'], s['zp_out'])
        elif typ == 'sigmoid':
            store[out] = sigmoid_int8(inputs[0], s['zp_in'], s['s_in'], s['zp_out'], s['s_out'])
        elif typ == 'quant':
            q = np.round(inputs[0].astype(np.float32)/s['s']).astype(np.int32)+s['zp']
            store[out] = np.clip(q,-128,127).astype(np.int8)
        elif typ == 'dequant':
            store[out] = (inputs[0].astype(np.float32)-s['zp'])*s['s']
        elif typ == 'view': store[out] = inputs[0]
    except Exception as e:
        print(f"[{idx}] {typ} {out} FAILED: {e}")
        sys.exit(1)

final = {}
for n in ('cls_8','cls_16','cls_32','obj_8','obj_16','obj_32','bbox_8','bbox_16','bbox_32','kps_8','kps_16','kps_32'):
    if n in store: final[n] = store[n]

faces = decode_faces(final, conf_thresh=0.9)
print(f"\n{len(faces)} faces")
for i, det in enumerate(faces):
    print(f"  [{i}] box={det[:4]} score={det[14]:.4f}")
