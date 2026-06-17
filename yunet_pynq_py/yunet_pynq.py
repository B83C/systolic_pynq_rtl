"""
YuNet INT8 on PYNQ — HW-accelerated pointwise + SW depthwise/other ops.

All spatial positions in a single DMA call.
Pre-loads A (weight) and C (bias) once, then streams B → output.
"""
import numpy as np, subprocess, time, sys, traceback

ON_PYNQ = False
try:
    import importlib
    _spec = importlib.util.find_spec('systolic_regs') if hasattr(importlib, 'util') else None
    if _spec is not None:
        ON_PYNQ = True
except Exception:
    pass

if ON_PYNQ:
    from pynq import allocate
    from systolic_regs import SystolicMap, SaReg, ChlastReg, QuantReg, TiledReg

from yunet_sw import (
    conv3x3, depthwise3x3, maxpool2x2, resize2x,
    eltwise_add, sigmoid_int8, to_block_major, decode_faces,
)

if ON_PYNQ:
    from load_stages import load_stages


def find_quant_params(mul_float):
    best_mq, best_sh = 1, 0
    best_err = float('inf')
    for sh in range(32):
        mq = int(round(mul_float * (1 << sh)))
        if mq > 65535 or mq < 1:
            continue
        err = abs(mq / (1 << sh) - mul_float) / max(mul_float, 1e-12)
        if err < best_err:
            best_err = err; best_mq, best_sh = mq, sh
    return best_mq, best_sh


# ═════════════════════════════════════════════════════════════════════
#  HW pointwise — single DMA call for ALL spatial positions
# ═════════════════════════════════════════════════════════════════════

class HwPointwise:
    """Pre-load once, then run per layer (pre-allocated buffers)."""

    def __init__(self, mmio, mmio_ch, mmio_q, mmio_tc, dma):
        self.mmio = mmio; self.mmio_ch = mmio_ch
        self.mmio_q = mmio_q; self.mmio_tc = mmio_tc
        self.dma = dma
        self.m = SystolicMap()
        self.T = 8
        self._a_buf = None
        self._c_buf = None
        self._bb = None   # pre-allocated B buffer (max spatial)
        self._ob = None   # pre-allocated output buffer
        self._max_n = 0   # max N seen so far
        self._proc = None # persistent C++ subprocess

    def _ensure_bufs(self, N, Kp, Cp):
        """Allocate B/O buffers for current layer's size."""
        Np = ((N + self.T - 1) // self.T) * self.T
        if self._bb is not None: self._bb.freebuffer()
        if self._ob is not None: self._ob.freebuffer()
        self._bb = allocate(shape=(Np*Kp,), dtype=np.int8)
        self._ob = allocate(shape=(Np*Cp,), dtype=np.int8)

    def load_layer(self, weight, bias, zp_w):
        """Pre-process weight + bias, load A + C rings + config SA. Once per layer."""
        T = self.T
        Cout, Cin = weight.reshape(weight.shape[0], -1).shape
        self.Cout, self.Cin = Cout, Cin
        Cp = ((Cout + T - 1) // T) * T
        Kp = ((Cin  + T - 1) // T) * T
        self.Cp, self.Kp = Cp, Kp

        w_2d = weight.reshape(Cout, Cin)
        w_pad = np.zeros((Cp, Kp), dtype=np.int8)
        w_pad[:Cout, :Cin] = w_2d
        w_off = np.clip(w_pad.astype(np.int16) - zp_w, -128, 127).astype(np.int8)
        A_bm = np.ascontiguousarray(
            w_off.reshape(Cp//T, T, Kp//T, T).transpose(0, 2, 1, 3))

        if self._a_buf is not None: self._a_buf.freebuffer()
        self._a_buf = allocate(shape=(Cp*Kp,), dtype=np.int8)
        self._a_buf[:] = A_bm.reshape(-1)

        if self._c_buf is not None: self._c_buf.freebuffer()
        self._c_buf = allocate(shape=(Cp,), dtype=np.int64)
        c32 = self._c_buf.view(np.int32).reshape(-1, 2)
        c32[:, :] = 0
        for i in range(Cout):
            c32[i, 0] = np.int32(bias[i])
        self._c_buf.sync_to_device()

        # Configure SA once
        self.mmio.write(SaReg.FB_CNT, Kp//T - 1)
        self.mmio.write(SaReg.A_LOOP_END, Cp*Kp//T - 1)
        self.mmio.write(SaReg.C_LOOP_END, Cp - 1)
        self.mmio.write(SaReg.ZP_IN, 0)
        self.m.configure_chlast(self.mmio_ch, cfg_channels=Kp, repeat_cnt=Kp//T, bypass=0)
        self.m.configure_quantizer(self.mmio_q, mul_q=1, shift=0, zp_out=0)
        self.m.configure_tiled(self.mmio_tc, cfg_channels=Cp)

        # A_LOAD once
        self.mmio.write(SaReg.A_LOAD, 1)
        self.mmio_ch.write(ChlastReg.BYPASS, 1)
        self.dma.sendchannel.start()
        self.dma.sendchannel.transfer(self._a_buf)
        self.dma.sendchannel.wait()
        self.mmio_ch.write(ChlastReg.BYPASS, 0)

        # C_LOAD once
        self.mmio.write(SaReg.C_LOAD, 1)
        self.mmio_ch.write(ChlastReg.BYPASS, 1)
        self.dma.sendchannel.start()
        self.dma.sendchannel.transfer(self._c_buf)
        self.dma.sendchannel.wait()
        self.mmio_ch.write(ChlastReg.BYPASS, 0)

    def run(self, act, zp_in, mul_float, zp_out):
        """act: (H,W,Cin) → (H,W,Cout). Only B stream DMA per call."""
        T, Cp, Kp = self.T, self.Cp, self.Kp
        N = act.shape[0] * act.shape[1]
        Np = ((N + T - 1) // T) * T
        self._ensure_bufs(N, Kp, Cp)

        # Pre-offset activations
        b_off = act.reshape(N, self.Cin).astype(np.int16) - zp_in
        b_off = b_off.astype(np.int8)
        self._bb[:N*self.Cin] = b_off.reshape(-1)
        if Np*Kp > N*self.Cin:
            self._bb[N*self.Cin:Np*Kp] = 0
        self._bb.sync_to_device()

        # Update quantizer (params change per layer)
        mq, sh = find_quant_params(mul_float)
        self.m.configure_quantizer(self.mmio_q, mul_q=mq, shift=sh, zp_out=zp_out)

        # B stream only
        self.dma.sendchannel.start()
        self.dma.recvchannel.start()
        self.dma.recvchannel.transfer(self._ob)
        self.dma.sendchannel.transfer(self._bb)

        out = self._ob.reshape(Np, Cp)[:N, :self.Cout].copy().reshape(act.shape[0], act.shape[1], -1)
        return out


# ═════════════════════════════════════════════════════════════════════
#  SW pointwise (fallback)
# ═════════════════════════════════════════════════════════════════════

def sw_pointwise(act, weight, bias, zp_in, zp_w, mul, zp_out):
    H, W, C_in = act.shape
    w_2d = weight.reshape(weight.shape[0], -1)
    B = act.reshape(-1, C_in)
    B_off = B.astype(np.float32) - zp_in
    A_off = w_2d.astype(np.float32) - zp_w
    acc = B_off @ A_off.T + bias.astype(np.float32)
    out = np.round(acc * mul).astype(np.int32) + zp_out
    return np.clip(out, -128, 127).astype(np.int8).reshape(H, W, -1)


# ═════════════════════════════════════════════════════════════════════
#  HW depthwise + conv3x3 (NEON via ctypes shared library)
# ═════════════════════════════════════════════════════════════════════

def _hw_depthwise(inp, weight, bias, zp_in, zp_w, mul_float, zp_out):
    """NEON depthwise via ctypes. inp: (H,W,C), weight: (C,1,3,3)."""
    from hw_ctypes import hw_dw3x3
    H, W, C = inp.shape
    mq, sh = find_quant_params(mul_float)
    w_3x3 = weight[:, 0, :, :]
    w_re = np.zeros((3, 3, C), dtype=np.int8)
    for ky in range(3):
        for kx in range(3):
            w_re[ky, kx, :] = w_3x3[:, ky, kx]
    from pynq import allocate
    ib = allocate(shape=(H*W*C,), dtype=np.int8); ib[:] = inp.reshape(-1)
    wb = allocate(shape=(9*C,), dtype=np.int8); wb[:] = w_re.reshape(-1)
    bb = allocate(shape=(C,), dtype=np.int32); bb[:] = bias.ravel()
    ob = allocate(shape=(H*W*C,), dtype=np.int8)
    hw_dw3x3(ib, wb, bb, ob, H, W, C, zp_in, zp_w, mq, sh, zp_out)
    out = ob.view(np.int8).reshape(H, W, C).copy()
    ib.freebuffer(); wb.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return out

def _hw_conv3x3(inp, weight, bias, zp_in, zp_w, mul_float, zp_out, stride):
    """NEON conv3x3 via ctypes. For first layer (3→16, stride 2)."""
    from hw_ctypes import hw_conv3x3
    H, W, Ci = inp.shape
    Co = weight.shape[0]
    mq, sh = find_quant_params(mul_float)
    C_in = weight.shape[1]
    w_re = np.zeros((C_in, 3, 3, Co), dtype=np.int8)
    for ci in range(C_in):
        for ky in range(3):
            for kx in range(3):
                w_re[ci, ky, kx, :] = weight[:, ci, ky, kx]
    from pynq import allocate
    ib = allocate(shape=(H*W*Ci,), dtype=np.int8); ib[:] = inp.reshape(-1)
    wb = allocate(shape=(C_in*9*Co,), dtype=np.int8); wb[:] = w_re.reshape(-1)
    bb = allocate(shape=(Co,), dtype=np.int32); bb[:] = bias.ravel()
    oH, oW = H//stride, W//stride
    ob = allocate(shape=(oH*oW*Co,), dtype=np.int8)
    hw_conv3x3(ib, wb, bb, ob, H, W, Ci, Co, zp_in, zp_w, mq, sh, zp_out)
    out = ob.view(np.int8).reshape(oH, oW, Co).copy()
    ib.freebuffer(); wb.freebuffer(); bb.freebuffer(); ob.freebuffer()
    return out


# ═════════════════════════════════════════════════════════════════════
#  Pipeline runner
# ═════════════════════════════════════════════════════════════════════

def run_pipeline(img, stages, use_hw=False, hw_pw=None, verbose=False):
    store = {'input': img}

    for idx, s in enumerate(stages):
        typ = s['type']
        out = s['out']
        inp_names = s['inp'] if isinstance(s['inp'], list) else [s['inp']]
        inputs = [store.get(n) for n in inp_names]
        if any(v is None for v in inputs):
            continue
        t0 = time.time() if verbose else None

        if typ == 'conv3x3':
            if use_hw and hw_pw is not None:
                try:
                    store[out] = _hw_conv3x3(inputs[0], s['w'], s['b_i32'],
                                             s['zp_in'], s['zp_w'], s['mul'], s['zp_out'],
                                             s.get('stride', 1))
                except Exception:
                    store[out] = conv3x3(inputs[0], s['w'], s['b_i32'],
                                         s['zp_in'], s['zp_w'], s['mul'], s['zp_out'],
                                         s.get('stride', 1))
            else:
                store[out] = conv3x3(inputs[0], s['w'], s['b_i32'],
                                     s['zp_in'], s['zp_w'], s['mul'], s['zp_out'],
                                     s.get('stride', 1))
        elif typ == 'dw':
            if use_hw and hw_pw is not None:
                try:
                    store[out] = _hw_depthwise(inputs[0], s['w'], s['b_i32'],
                                               s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
                except Exception as e:
                    if verbose: print(f'  [{idx:3d}] dw FAILED -> SW fallback ({e})')
                    store[out] = depthwise3x3(inputs[0], s['w'], s['b_i32'],
                                              s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
            else:
                store[out] = depthwise3x3(inputs[0], s['w'], s['b_i32'],
                                          s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
        elif typ == 'pw':
            _, _, Cin = inputs[0].shape
            _, H, W = inputs[0].shape[:3]
            N = H * W
            use_sa = use_hw and hw_pw is not None and Cin >= 8 and N >= 400
            if use_sa:
                try:
                    hw_pw.load_layer(s['w'], s['b_i32'], s['zp_w'])
                    store[out] = hw_pw.run(inputs[0], s['zp_in'], s['mul'], s['zp_out'])
                except Exception as e:
                    if verbose: print(f'  [{idx:3d}] pw FAILED -> SW fallback ({e})')
                    store[out] = sw_pointwise(inputs[0], s['w'], s['b_i32'],
                                              s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
            else:
                store[out] = sw_pointwise(inputs[0], s['w'], s['b_i32'],
                                          s['zp_in'], s['zp_w'], s['mul'], s['zp_out'])
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

        if verbose and t0 is not None and typ not in ('view',):
            dt = (time.time() - t0) * 1000
            v = store[out]
            lo = v.min() if isinstance(v, np.ndarray) else v
            hi = v.max() if isinstance(v, np.ndarray) else v
            print(f'  [{idx:3d}] {typ:10s} {out:30s} {str(v.shape):20s} [{lo:8.4f},{hi:8.4f}] {dt:7.1f}ms')

    final = {}
    for name in ('cls_8', 'cls_16', 'cls_32',
                 'obj_8', 'obj_16', 'obj_32',
                 'bbox_8', 'bbox_16', 'bbox_32',
                 'kps_8', 'kps_16', 'kps_32'):
        if name in store:
            final[name] = store[name]
    return final


# ═════════════════════════════════════════════════════════════════════
#  Main
# ═════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    import cv2, argparse, sys, traceback

    parser = argparse.ArgumentParser(description='YuNet INT8 on PYNQ')
    parser.add_argument('--input', '-i', type=str, default='test.png')
    parser.add_argument('--hw', action='store_true')
    parser.add_argument('--verbose', '-v', action='store_true')
    args = parser.parse_args()

    use_hw = args.hw and ON_PYNQ
    hw_pw = None

    if use_hw:
        print('Initializing...')
        from pynq import Overlay, MMIO
        ol = Overlay('systolic.bit')
        mmio    = MMIO(ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr'], 0x1000)
        mmio_ch = MMIO(ol.ip_dict['chlast_to_tiled_gath_0']['phys_addr'], 0x1000)
        mmio_q  = MMIO(ol.ip_dict['quantizer_wrapper_0']['phys_addr'], 0x1000)
        mmio_tc = MMIO(ol.ip_dict['tiled_to_chlast_gath_0']['phys_addr'], 0x1000)
        hw_pw = HwPointwise(mmio, mmio_ch, mmio_q, mmio_tc, ol.axi_dma_0)
        ip_addr = ol.ip_dict['sa_wrapper_axi_ctrl_0']['phys_addr']
        print(f'  SA ready (mmio @ 0x{ip_addr:x})')

    print('Loading stage table...')
    t0 = time.time()
    if ON_PYNQ:
        stages = load_stages('yunet_stages.npz')
    else:
        from yunet_sw import build_stage_table
        stages, _ = build_stage_table('face_detection_yunet_2023mar_int8.onnx')
    print(f'  {len(stages)} stages  ({(time.time()-t0)*1000:.0f} ms)')

    try:
        img = cv2.imread(args.input)
        if img is None:
            print(f'ERROR: cannot read {args.input}'); sys.exit(1)
        img_640 = cv2.resize(img, (640, 640))

        print('Running pipeline...')
        t0 = time.time()
        outputs = run_pipeline(img_640, stages, use_hw=use_hw, hw_pw=hw_pw,
                               verbose=args.verbose)
        print(f'  Done  ({(time.time()-t0)*1000:.0f} ms)')

        print(f'\nDetection:')
        faces = decode_faces(outputs, conf_thresh=0.9)
        if len(faces) == 0:
            print('  0 faces.')
        else:
            faces_arr = np.array(faces)
            sx, sy = img.shape[1] / 640, img.shape[0] / 640
            for j in range(15):
                faces_arr[:, j] *= sx if j%2==0 else sy
            for idx, det in enumerate(faces_arr):
                print(f'  [{idx}] score={det[14]:.4f} box={det[:4].astype(int)}')
    except Exception:
        print(f'CRASH at pipeline stage (see above):')
        traceback.print_exc()
