#!/usr/bin/env python3
"""
Simulation of YuNet 2023mar_int8 on PYNQ-Z2 architecture.

Hardware blocks:
  - depthwise_conv: streaming 3x3 per-channel, 2-row line buffer, 9-tap FIR
  - gemm_1x1: your 64x64 systolic GEMM for 1x1 pointwise convs
  - requantize: INT32 -> INT8 with scale*zp + clamp
  - maxpool_2x2: streaming stride-2 max
  - resize_nearest: nearest-neighbor x2 upsampling
  - add_eltwise: element-wise add with saturation
"""

import math, time
import numpy as np
import cv2 as cv
import onnx
import onnx.numpy_helper as nph

# ---------------------------------------------------------------------------
# Hardware block simulators
# ---------------------------------------------------------------------------

class Conv3x3:
    """Regular 3x3 conv (not depthwise). Used for the first layer only."""

    @staticmethod
    def forward(inp: np.ndarray, weight: np.ndarray, bias: np.ndarray,
                scale_in: float, zp_in: int,
                scale_w: float, zp_w: int,
                scale_out: float, zp_out: int,
                stride: int = 1) -> np.ndarray:
        """
        inp:    (H, W, C_in)  uint8
        weight: (C_out, C_in, 3, 3) int8
        bias:   (C_out,) int32
        """
        H, W, C_in = inp.shape
        C_out = weight.shape[0]
        Kh = Kw = 3
        pad = 1  # same padding

        w_fp = (weight.astype(np.float32) - zp_w) * scale_w
        in_fp = (inp.astype(np.float32) - zp_in) * scale_in
        in_pad = np.pad(in_fp, ((pad, pad), (pad, pad), (0, 0)), mode='constant')

        out_h = (H + 2 * pad - Kh) // stride + 1
        out_w = (W + 2 * pad - Kw) // stride + 1
        acc = np.zeros((out_h, out_w, C_out), dtype=np.float32)

        for oy in range(out_h):
            for ox in range(out_w):
                iy = oy * stride
                ix = ox * stride
                for ky in range(Kh):
                    for kx in range(Kw):
                        patch = in_pad[iy + ky, ix + kx, :]
                        w_ = w_fp[:, :, ky, kx]
                        acc[oy, ox, :] += patch @ w_.T

        acc += bias.astype(np.float32) * (scale_in * scale_w)
        q = np.round(acc / scale_out).astype(np.int32) + zp_out
        return np.clip(q, -128, 127).astype(np.int8)


class Requantizer:
    """INT32 accumulator -> INT8/UINT8 output: clamp(round(acc/scale) + zp, type_range)"""

    @staticmethod
    def forward(acc: np.ndarray, scale: float, zp: int) -> np.ndarray:
        out = np.round(acc.astype(np.float64) / scale).astype(np.int32) + zp
        return np.clip(out, -128, 127).astype(np.int8)


class DepthwiseConv3x3:
    """
    Streaming depthwise 3x3 conv (channel-last HWC).
    Per channel, 2-row line buffer + 3x3 window, 9 MACs -> 1 output pixel/cycle.

    In HWC format: one cycle reads 3 adjacent pixels (all C channels),
    produces 1 output pixel (all C channels).
    """

    @staticmethod
    def forward(inp: np.ndarray, weight: np.ndarray, bias: np.ndarray,
                scale_in: float, zp_in: int,
                scale_w: float, zp_w: int,
                scale_out: float, zp_out: int,
                stride: int = 1) -> np.ndarray:
        """
        inp:   (H, W, C)  uint8   input feature map
        weight:(C, 1, 3, 3) int8   depthwise weight  (kernel 3x3, 1 group per chan)
        bias:  (C,)       int32   bias
        """
        H, W, C = inp.shape
        Kh = Kw = 3
        pad = 1  # SAME padding

        # Dequantize weight once
        w_fp = (weight.astype(np.float32) - zp_w) * scale_w  # (C,1,3,3)
        w_per_chan = w_fp[:, 0, :, :]  # (C, 3, 3)

        # Dequantize input
        in_fp = (inp.astype(np.float32) - zp_in) * scale_in  # (H,W,C)

        # Pad input (reflect or zero)
        in_pad = np.pad(in_fp, ((pad, pad), (pad, pad), (0, 0)), mode='constant')

        out_h = (H + 2 * pad - Kh) // stride + 1
        out_w = (W + 2 * pad - Kw) // stride + 1
        acc = np.zeros((out_h, out_w, C), dtype=np.float32)

        # Hardware-style: streaming scan
        # For each output pixel, accumulate 9 taps per channel
        for oy in range(out_h):
            for ox in range(out_w):
                for ky in range(Kh):
                    for kx in range(Kw):
                        iy = oy * stride + ky
                        ix = ox * stride + kx
                        # Vector of C channels at this pixel
                        px = in_pad[iy, ix, :]  # (C,)
                        w_ = w_per_chan[:, ky, kx]  # (C,)
                        acc[oy, ox, :] += px * w_

        # Add bias (scaled by input*weight scale factor)
        acc += bias.astype(np.float32) * (scale_in * scale_w)

        # Requantize to INT8 or UINT8 based on zp_out
        quantized = np.round(acc / scale_out).astype(np.int32) + zp_out
        quantized = np.clip(quantized, -128, 127).astype(np.int8)
        return quantized


class GEMM1x1:
    """
    Simulate your 64x64 systolic GEMM array doing 1x1 pointwise conv.

    Input:  (H, W, C)  uint8   -> flatten to (H*W, C)
    Weight: (C_out, C, 1, 1) int8 -> (C_out, C)
    Output: (H, W, C_out) uint8

    The GEMM tiles across (M, K, N) in 64x64 blocks.
    """

    @staticmethod
    def forward(inp: np.ndarray, weight: np.ndarray, bias: np.ndarray,
                scale_in: float, zp_in: int,
                scale_w: float, zp_w: int,
                scale_out: float, zp_out: int) -> np.ndarray:
        H, W, C_in = inp.shape
        C_out = weight.shape[0]
        assert weight.shape[1] == C_in
        assert weight.shape[2] == 1 and weight.shape[3] == 1

        # Dequantize input
        in_fp = (inp.astype(np.float32) - zp_in) * scale_in  # (H,W,C)
        in_flat = in_fp.reshape(-1, C_in)  # (H*W, C_in)

        # Dequantize weight
        w_flat = weight.reshape(C_out, C_in).astype(np.float32)
        w_fp = (w_flat - zp_w) * scale_w  # (C_out, C_in)

        # GEMM: out (HW, C_out) = in_flat (HW, C_in) @ w_fp.T (C_in, C_out)
        # Tile in 64x64
        M, K = in_flat.shape
        N = C_out
        acc = np.zeros((M, N), dtype=np.float32)

        for m0 in range(0, M, 64):
            m1 = min(m0 + 64, M)
            for k0 in range(0, K, 64):
                k1 = min(k0 + 64, K)
                for n0 in range(0, N, 64):
                    n1 = min(n0 + 64, N)
                    # 64x64 tile multiply
                    tile_a = in_flat[m0:m1, k0:k1]  # (<=64, <=64)
                    tile_b = w_fp[n0:n1, k0:k1].T   # (<=64, <=64)
                    acc[m0:m1, n0:n1] += tile_a @ tile_b  # type: ignore

        # Add bias (scaled by input*weight scale factor)
        acc += bias.astype(np.float32) * (scale_in * scale_w)  # (C_out,) broadcast

        # Requantize to INT8 or UINT8 based on zp_out
        quantized = np.round(acc / scale_out).astype(np.int32) + zp_out
        quantized = np.clip(quantized, -128, 127).astype(np.int8)
        return quantized.reshape(H, W, C_out)


class MaxPool2x2:
    """Streaming 2x2 max pool, stride 2. Operates on FP32."""

    @staticmethod
    def forward(inp: np.ndarray) -> np.ndarray:
        H, W, C = inp.shape
        out_h, out_w = H // 2, W // 2
        out = inp.reshape(out_h, 2, out_w, 2, C).max(axis=(1, 3))
        return out


class ResizeNearest:
    """Nearest-neighbor 2x upsampling on INT8."""

    @staticmethod
    def forward(inp: np.ndarray) -> np.ndarray:
        H, W, C = inp.shape
        out = np.repeat(np.repeat(inp, 2, axis=0), 2, axis=1)
        return out


class AddEltwise:
    """Element-wise add on INT8 with saturation."""

    @staticmethod
    def forward(a: np.ndarray, b: np.ndarray,
                scale_a: float, zp_a: int,
                scale_b: float, zp_b: int,
                scale_out: float, zp_out: int) -> np.ndarray:
        # Dequantize
        a_fp = (a.astype(np.float32) - zp_a) * scale_a
        b_fp = (b.astype(np.float32) - zp_b) * scale_b
        # Add
        s = a_fp + b_fp
        # Requantize
        q = np.round(s / scale_out).astype(np.int32) + zp_out
        return np.clip(q, -128, 127).astype(np.int8)


# ---------------------------------------------------------------------------
# ONNX graph executor
# ---------------------------------------------------------------------------

class OnnxNode:
    """Minimal node wrapper."""
    def __init__(self, node):
        self.op = node.op_type
        self.inputs = list(node.input)
        self.outputs = list(node.output)
        self.name = node.name if node.name else f'{node.op}_{node.output[0][:8]}'
        # Parse attributes
        self.attrs = {}
        for a in node.attribute:
            if a.type == onnx.AttributeProto.INTS:
                self.attrs[a.name] = list(a.ints)
            elif a.type == onnx.AttributeProto.INT:
                self.attrs[a.name] = a.i
            elif a.type == onnx.AttributeProto.FLOAT:
                self.attrs[a.name] = a.f
            elif a.type == onnx.AttributeProto.STRING:
                self.attrs[a.name] = a.s


class YuNetSimulator:
    def __init__(self, onnx_path: str):
        model = onnx.load(onnx_path)
        self.graph = model.graph

        # Build name -> tensor map (initializers + inputs)
        self.params = {}
        for init in self.graph.initializer:
            arr = nph.to_array(init)
            self.params[init.name] = arr

        # Build input info
        self.input_name = self.graph.input[0].name

        # Parse all nodes
        self.nodes = [OnnxNode(n) for n in self.graph.node]

        # Build output shape tracking
        self.tensors = {}

        # Pre-scan which QLinearConv are depthwise vs pointwise
        self.conv_type = {}  # node_output_name -> 'gemm' or 'depthwise'
        self._classify_convs()

    def _classify_convs(self):
        """Identify each QLinearConv as 1x1 pointwise (GEMM) or 3x3 depthwise."""
        for node in self.nodes:
            if node.op != 'QLinearConv':
                continue
            # weight name is input[3]
            w_name = node.inputs[3]
            if w_name in self.params:
                w = self.params[w_name]
                if w.ndim == 4:
                    if w.shape[2] == 1 and w.shape[3] == 1:
                        self.conv_type[node.outputs[0]] = 'gemm'
                    elif w.shape[1] == 1 and w.shape[2] == 3 and w.shape[3] == 3:
                        self.conv_type[node.outputs[0]] = 'depthwise'
                    else:
                        # Regular 3x3 conv (e.g. first layer: 3->16)
                        self.conv_type[node.outputs[0]] = 'conv3x3'

    def _get(self, name: str):
        """Get a tensor by name from the workspace."""
        if name in self.tensors:
            return self.tensors[name]
        if name in self.params:
            return self.params[name]
        return None

    def _set(self, name: str, val: np.ndarray):
        self.tensors[name] = val

    def simulate(self, img_bgr: np.ndarray, verbose: bool = False) -> dict:
        """Run the full network on an image. Returns dict of output tensors."""
        H, W = img_bgr.shape[:2]
        assert H == 640 and W == 640, 'int8 model expects 640x640 input'
        t0 = time.time()

        # Quantize input: BGR uint8 -> uint8 with input scale/zp
        input_scale = self.params['input_scale'].item()
        input_zp = int(self.params['input_zero_point'].item())
        # ONNX model expects channel-first for QuantizeLinear? Check...
        # Actually the model input 'input_scale' is a scalar.
        # The QuantizeLinear node quantizes the input.
        # We'll feed the BGR image as-is and use the QuantizeLinear node.

        # Store input image (BGR uint8) as the named input tensor
        self.tensors[self.input_name] = img_bgr

        # We'll process nodes sequentially
        for idx, node in enumerate(self.nodes):
            t1 = time.time()
            op = node.op
            inp_names = node.inputs
            out_name = node.outputs[0]

            if verbose:
                print(f'[{idx:3d}] {op:20s} {out_name[:40]}', end=' ')

            skip = False
            result = None

            if op == 'QuantizeLinear':
                # input, scale, zp -> quantized int8 or uint8
                inp = self._get(inp_names[0])
                scale = self._get(inp_names[1]).item()
                zp = int(self._get(inp_names[2]).item())
                result = np.round(inp.astype(np.float32) / scale).astype(np.int32) + zp
                result = np.clip(result, -128, 127).astype(np.int8)

            elif op == 'DequantizeLinear':
                inp = self._get(inp_names[0])
                scale = self._get(inp_names[1]).item()
                zp = int(self._get(inp_names[2]).item())
                result = (inp.astype(np.float32) - zp) * scale

            elif op == 'QLinearConv':
                # inputs: X, X_scale, X_zp, W, W_scale, W_zp, Y_scale, Y_zp, B
                X = self._get(inp_names[0])  # uint8
                X_scale = self._get(inp_names[1]).item()
                X_zp = int(self._get(inp_names[2]).item())
                W = self._get(inp_names[3])  # int8
                W_scale = self._get(inp_names[4]).item()
                W_zp = int(self._get(inp_names[5]).item())
                Y_scale = self._get(inp_names[6]).item()
                Y_zp = int(self._get(inp_names[7]).item())
                B = self._get(inp_names[8])  # int32 bias

                # Channel-last: X is (H,W,C) uint8
                # But the model might use channel-first. Let's check a key detail:
                # The ONNX QLinearConv uses channel-first (N, C, H, W) format.
                # In our simulation we might keep it channel-first for ONNX compat.

                # Actually, let's handle the conversion: most ONNX models are NCHW
                # But our hardware architecture is HWC (channel-last).
                # We'll convert to HWC internally.

                # Check shape of X: might be (N, C, H, W) from ONNX flow
                # We need to handle both cases
                if X.ndim == 4:  # NCHW
                    X_hwc = X[0].transpose(1, 2, 0)  # (H,W,C) uint8
                else:
                    X_hwc = X

                ctype = self.conv_type.get(out_name, 'gemm')
                stride = tuple(node.attrs.get('strides', [1, 1]))
                if ctype == 'gemm':
                    # 1x1 pointwise -> GEMM
                    result = GEMM1x1.forward(X_hwc, W, B,
                                             X_scale, X_zp,
                                             W_scale, W_zp,
                                             Y_scale, Y_zp)
                elif ctype == 'conv3x3':
                    result = Conv3x3.forward(X_hwc, W, B,
                                             X_scale, X_zp,
                                             W_scale, W_zp,
                                             Y_scale, Y_zp,
                                             stride=stride[0])
                else:
                    # 3x3 depthwise -> streaming depthwise conv
                    result = DepthwiseConv3x3.forward(X_hwc, W, B,
                                                       X_scale, X_zp,
                                                       W_scale, W_zp,
                                                       Y_scale, Y_zp)

                # Convert back to NCHW for ONNX compat
                if X.ndim == 4:
                    result = result.transpose(2, 0, 1)[np.newaxis, :, :, :]

            elif op == 'Relu':
                inp = self._get(inp_names[0])
                result = np.clip(inp, 0, None)

            elif op == 'MaxPool':
                inp = self._get(inp_names[0])
                # ONNX MaxPool: input (N, C, H, W), output (N, C, H/2, W/2)
                if inp.ndim == 4:
                    n, c, h, w = inp.shape
                    out = np.zeros((n, c, h // 2, w // 2), dtype=inp.dtype)
                    for i in range(n):
                        for j in range(c):
                            out[i, j] = MaxPool2x2.forward(inp[i, j])
                    result = out
                else:
                    result = MaxPool2x2.forward(inp)

            elif op == 'Resize':
                inp = self._get(inp_names[0])
                # Nearest-neighbor 2x upsampling
                if inp.ndim == 4:  # NCHW
                    n, c, h, w = inp.shape
                    out = np.zeros((n, c, h * 2, w * 2), dtype=inp.dtype)
                    for i in range(n):
                        for j in range(c):
                            out[i, j] = ResizeNearest.forward(inp[i, j])
                    result = out
                else:
                    result = ResizeNearest.forward(inp)

            elif op == 'QLinearSigmoid':
                # inputs: X, X_scale, X_zp, Y_scale, Y_zp
                x = self._get(inp_names[0])
                x_scale = self._get(inp_names[1]).item()
                x_zp = int(self._get(inp_names[2]).item())
                y_scale = self._get(inp_names[3]).item()
                y_zp = int(self._get(inp_names[4]).item())
                x_fp = (x.astype(np.float32) - x_zp) * x_scale
                y_fp = 1.0 / (1.0 + np.exp(-x_fp))
                result = np.round(y_fp / y_scale).astype(np.int32) + y_zp
                result = np.clip(result, -128, 127).astype(np.int8)

            elif op == 'QLinearAdd':
                # inputs: A, A_scale, A_zp, B, B_scale, B_zp, C_scale, C_zp
                a = self._get(inp_names[0])
                a_scale = self._get(inp_names[1]).item()
                a_zp = int(self._get(inp_names[2]).item())
                b = self._get(inp_names[3])
                b_scale = self._get(inp_names[4]).item()
                b_zp = int(self._get(inp_names[5]).item())
                c_scale = self._get(inp_names[6]).item()
                c_zp = int(self._get(inp_names[7]).item())
                result = AddEltwise.forward(a, b, a_scale, a_zp, b_scale, b_zp, c_scale, c_zp)

            elif op == 'Add':
                a = self._get(inp_names[0])
                b = self._get(inp_names[1])
                if a.dtype == np.uint8:
                    # Quantized add: need scales/zps
                    # In this model, the Add operates on dequantized or quantized
                    # tensors. We need to find the scale/zp from the graph context.
                    # For simplicity, if both are uint8, use the incoming scale
                    # (we'll handle it properly later)
                    scale_a = 1.0; zp_a = 0
                    scale_b = 1.0; zp_b = 0
                    scale_out = 1.0; zp_out = 0
                    # Try to infer from quantize/dequantize surrounding nodes
                    # Actually the int8 model uses Resize output + lateral conv output
                    # They're both INT8 quantized. We need to find their scales.
                    # For a quick sim, convert to float and back:
                    if '455_scale' in self.params:
                        scale_a = self.params['455_scale'].item()
                        zp_a = int(self.params['455_zero_point'].item())
                    if '223_scale' in self.params:
                        scale_b = self.params['223_scale'].item()
                        zp_b = int(self.params['223_zero_point'].item())
                    # Output scale
                    if '242_scale' in self.params:
                        scale_out = self.params['242_scale'].item()
                        zp_out = int(self.params['242_zero_point'].item())
                    result = AddEltwise.forward(a, b, scale_a, zp_a, scale_b, zp_b, scale_out, zp_out)
                else:
                    result = a + b

            elif op == 'Transpose':
                inp = self._get(inp_names[0])
                # Usually perm=[0,2,3,1] or [0,3,1,2] — infer from shape
                if inp.ndim == 4:
                    # Assume [0,2,3,1] (NCHW -> NHWC) for cls/obj/bbox/kps heads
                    result = inp.transpose(0, 2, 3, 1)
                else:
                    result = inp

            elif op == 'Reshape':
                inp = self._get(inp_names[0])
                shape = self._get(inp_names[1])
                result = inp.reshape(tuple(shape.astype(int)))

            elif op == 'Sigmoid':
                inp = self._get(inp_names[0])
                result = 1.0 / (1.0 + np.exp(-inp))

            else:
                if verbose:
                    print('   (skipped/unhandled)')
                skip = True

            if not skip:
                self._set(out_name, result)
                if verbose:
                    dt = time.time() - t1
                    sz = result.size
                    print(f'  shape={result.shape} dtype={result.dtype}  {dt*1000:.1f}ms')

        # Collect output tensors
        outputs = {}
        for node in self.nodes:
            if node.op == 'Reshape' and node.outputs[0].startswith(('cls_', 'obj_', 'bbox_', 'kps_')):
                out_name = node.outputs[0]
                val = self._get(out_name)
                if val is not None:
                    outputs[out_name] = val
            if node.op == 'DequantizeLinear' and node.outputs[0].startswith(('cls_', 'obj_', 'bbox_', 'kps_')):
                out_name = node.outputs[0]
                val = self._get(out_name)
                if val is not None:
                    outputs[out_name] = val

        total_time = time.time() - t0
        print(f'\nTotal inference time: {total_time*1000:.1f} ms')
        return outputs


# ---------------------------------------------------------------------------
# Decode (same as demo_raw.py)
# ---------------------------------------------------------------------------

def decode_faces(outputs: dict, conf_thresh=0.9, nms_thresh=0.3, top_k=5000):
    strides = [8, 16, 32]
    names = ['cls_8', 'cls_16', 'cls_32', 'obj_8', 'obj_16', 'obj_32',
             'bbox_8', 'bbox_16', 'bbox_32', 'kps_8', 'kps_16', 'kps_32']

    for n in names:
        if n not in outputs:
            print(f'Missing output: {n}')
            return np.empty((0, 15))

    cls_list = [outputs[n] for n in names[0:3]]
    obj_list = [outputs[n] for n in names[3:6]]
    bbox_list = [outputs[n] for n in names[6:9]]
    kps_list = [outputs[n] for n in names[9:12]]

    pad_h, pad_w = 640, 640  # fixed for this model
    all_boxes, all_scores, all_kps = [], [], []

    for idx, stride in enumerate(strides):
        cls = cls_list[idx].reshape(-1)
        obj = obj_list[idx].reshape(-1)
        bbox = bbox_list[idx].reshape(-1, 4)
        kps = kps_list[idx].reshape(-1, 10)

        feat_h = pad_h // stride
        feat_w = pad_w // stride
        anchors = np.stack(np.meshgrid(np.arange(feat_w), np.arange(feat_h)), axis=-1).reshape(-1, 2).astype(np.float32)
        anchors[:, 0] = (anchors[:, 0] + 0.5) * stride
        anchors[:, 1] = (anchors[:, 1] + 0.5) * stride
        # One anchor per grid cell
        # (model uses 4 anchor boxes, but anchor offsets are computed per cell)

        scores = np.sqrt(np.clip(cls, 0, 1) * np.clip(obj, 0, 1))

        bw = np.exp(bbox[:, 2]) * stride
        bh = np.exp(bbox[:, 3]) * stride
        bx = bbox[:, 0] * stride + anchors[:, 0]
        by = bbox[:, 1] * stride + anchors[:, 1]
        decoded_boxes = np.stack([bx - bw / 2, by - bh / 2, bw, bh], axis=1)

        decoded_kps = np.zeros_like(kps)
        for i in range(5):
            decoded_kps[:, i * 2] = kps[:, i * 2] * stride + anchors[:, 0]
            decoded_kps[:, i * 2 + 1] = kps[:, i * 2 + 1] * stride + anchors[:, 1]

        keep = scores >= conf_thresh
        all_boxes.append(decoded_boxes[keep])
        all_scores.append(scores[keep])
        all_kps.append(decoded_kps[keep])

    if not all_boxes:
        return np.empty((0, 15))

    all_boxes = np.concatenate(all_boxes, axis=0)
    all_scores = np.concatenate(all_scores, axis=0)
    all_kps = np.concatenate(all_kps, axis=0)

    indices = cv.dnn.NMSBoxes(all_boxes.tolist(), all_scores.tolist(), conf_thresh, nms_thresh, top_k=top_k)
    if len(indices) > 0:
        indices = indices.flatten()
        all_boxes = all_boxes[indices]
        all_scores = all_scores[indices]
        all_kps = all_kps[indices]

    faces = np.zeros((len(all_boxes), 15), dtype=np.float32)
    faces[:, :4] = all_boxes
    faces[:, 4:14] = all_kps
    faces[:, 14] = all_scores
    return faces


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', '-i', type=str, required=True)
    parser.add_argument('--model', '-m', type=str, default='face_detection_yunet_2023mar_int8.onnx')
    parser.add_argument('--conf_threshold', type=float, default=0.9)
    parser.add_argument('--verbose', '-v', action='store_true')
    parser.add_argument('--reference', '-r', action='store_true', help='Also run via OpenCV for comparison')
    args = parser.parse_args()

    img = cv.imread(args.input)
    img_640 = cv.resize(img, (640, 640))

    print(f'Loading model: {args.model}')
    sim = YuNetSimulator(args.model)
    print(f'Running simulation...')
    outputs = sim.simulate(img_640, verbose=args.verbose)

    print('Decoding faces...')
    faces = decode_faces(outputs, conf_thresh=args.conf_threshold)
    if len(faces) == 0:
        print('0 faces detected.')
    else:
        sx, sy = img.shape[1] / 640, img.shape[0] / 640
        for face in faces:
            face[0] *= sx; face[1] *= sy; face[2] *= sx; face[3] *= sy
            for i in range(5):
                face[4 + i * 2] *= sx
                face[4 + i * 2 + 1] *= sy
        print(f'{len(faces)} faces detected.')
        for idx, det in enumerate(faces):
            print(f'  [{idx}] score={det[14]:.4f} box={det[:4].astype(int)}')

    if args.reference:
        print('\n--- Reference (OpenCV) ---')
        net = cv.dnn.readNetFromONNX(args.model)
        blob = cv.dnn.blobFromImage(img_640, 1.0, (640, 640), swapRB=False, crop=False)
        net.setInput(blob)
        out_names = ['cls_8', 'cls_16', 'cls_32', 'obj_8', 'obj_16', 'obj_32',
                     'bbox_8', 'bbox_16', 'bbox_32', 'kps_8', 'kps_16', 'kps_32']
        ref_raw = net.forward(out_names)

        # Decode reference
        ref_faces = decode_faces(dict(zip(out_names, ref_raw)), conf_thresh=args.conf_threshold)
        if len(ref_faces) > 0:
            sx, sy = img.shape[1] / 640, img.shape[0] / 640
            for face in ref_faces:
                face[0] *= sx; face[1] *= sy; face[2] *= sx; face[3] *= sy
                for i in range(5):
                    face[4 + i * 2] *= sx
                    face[4 + i * 2 + 1] *= sy

        print(f'{len(ref_faces)} faces detected.')
        for idx, det in enumerate(ref_faces):
            print(f'  [{idx}] score={det[14]:.4f} box={det[:4].astype(int)}')

        # Compare raw outputs
        print('\n--- Raw output comparison ---')
        for name in out_names:
            if name in outputs and name in dict(zip(out_names, ref_raw)):
                sim = outputs[name].reshape(-1)
                ref = dict(zip(out_names, ref_raw))[name].reshape(-1)
                mae = np.mean(np.abs(sim.astype(np.float32) - ref.astype(np.float32)))
                print(f'  {name}: sim_range=[{sim.min():.3f},{sim.max():.3f}] ref_range=[{ref.min():.3f},{ref.max():.3f}] MAE={mae:.4f}')

        # Compare scores
        if len(faces) > 0 and len(ref_faces) > 0:
            sim_scores = set(round(s, 4) for s in faces[:, 14])
            ref_scores = set(round(s, 4) for s in ref_faces[:, 14])
            common = sim_scores & ref_scores
            print(f'\nScore overlap: {len(common)}/{len(ref_scores)}')
            if len(common) < len(ref_scores):
                print(f'  Missing: {ref_scores - sim_scores}')
                print(f'  Extra:   {sim_scores - ref_scores}')
