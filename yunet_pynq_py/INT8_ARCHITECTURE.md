# INT8 Model Architecture & Hardware Accelerator Guide

This document documents the `face_detection_yunet_2023mar_int8.onnx` model from a hardware implementation perspective: the quantization scheme, the QLinearConv computation, the GEMM accelerator architecture, and the validation methodology.

## 1. Model Overview

YuNet 2023mar INT8 is a fully quantized face detection network with 112 ONNX nodes (53 QLinearConv + supporting ops). It runs at 640×640 input and produces detection scores up to 0.99 at conf=0.9, detecting faces from ~10×10 to ~300×300 pixels.

### ONNX Op Types Present

| Op | Count | Role |
|----|-------|------|
| QLinearConv | 53 | All conv: 1 regular 3×3, 26 depthwise 3×3, 26 pointwise 1×1 |
| QuantizeLinear | 5 | Input + post-maxpool re-quantization |
| DequantizeLinear | 16 | Pre-maxpool dequantization |
| QLinearAdd | 2 | FPN element-wise add |
| QLinearSigmoid | 6 | cls/obj scores × 3 scales |
| MaxPool | 4 | 2×2 stride-2 in backbone |
| Resize | 2 | Nearest 2× in FPN neck |
| Transpose | 12 | Layout conversion (HWC↔CHW) |
| Reshape | 12 | Flatten for detection heads |

## 2. Quantization Format

### Weight Storage

All weights use **symmetric quantization** (`zp_w = 0`):

```
w_float = int8_val * w_scale
```

`w_scale` is per-tensor (scalar). Weights range in `[0.00004, 0.022]`.

### Activation Storage

Activations use **asymmetric quantization** with per-tensor zero-point:

```
act_float = (int8_val - zp_act) * act_scale
```

`zp_act` varies per tensor. In this model:
- `zp = -128` for most internal features (signed INT8)
- `zp = 14, 47` etc. for some intermediate outputs
- **All tensors are stored as INT8** `[-128, 127]` regardless of zp sign

Key insight: OpenCV's `ConvolutionInt8` stores all intermediate tensors as **signed INT8** internally, treating the zero-point as an offset applied during dequantization, not as an output storage type. Clamping to UINT8 `[0, 255]` mid-network destroys information.

### Bias Storage

Bias is stored as **INT32** pre-scaled by `1 / (s_act * s_w)`:

```
bias_int32 = round(bias_float / (s_act * s_w))
```

This allows the bias to be added directly to the INT32 accumulator before requantization.

## 3. QLinearConv Computation

Each QLinearConv (regular 3×3, depthwise 3×3, or pointwise 1×1) follows the same computation:

```
acc_i32 = Σ (act[i] - zp_act) * (w[i] - zp_w)   # INT8 × INT8 → INT32 MAC
acc_i32 += bias_int32                              # INT32 bias add
out_i8   = clamp(round(acc_i32 * mul) + zp_out, -128, 127)
```

where:

```
mul = s_act * s_w / s_out
```

All three conv types produce INT8 output. The `mul` values in this model range from `[0.0014, 0.052]`.

```
                          ┌─────────────────────┐
 act_i8 ─→[−zp_act]──┐   │                     │
                      │   │   INT8 × INT8       │
                      ├──→│   → INT32 MAC       ├──→ +bias → [×mul] → [+zp_out] → [clamp] → out_i8
                      │   │   (systolic array)  │
 w_i8 ──→[−zp_w]─────┘   │                     │
                          └─────────────────────┘
```

### Verbatim formula (all data types):

```
out = clip(
    round(
        (conv((act - zp_act) × (w - zp_w)) + bias_int32) × s_act × s_w / s_out
    ) + zp_out,
    -128, 127
).astype(int8)
```

## 4. GEMM Accelerator Architecture (Pointwise 1×1)

The pointwise 1×1 convolutions can be accelerated with an 8×8 INT8 systolic MAC array.

### 4.1 Array Structure

```
                B[k,0]  B[k,1]  ...  B[k,7]     ← streamed weight columns
                  │       │             │
          ┌───────┼───────┼───────┐     │
 A[0,k] ─→│  PE₀₀  │  PE₀₁  │ ...  │ PE₀₇  │
          ├───────┼───────┼───────┤     │
 A[1,k] ─→│  PE₁₀  │  PE₁₁  │ ...  │ PE₁₇  │
  (cached)├───────┼───────┼───────┤     │
   ...    │  ...   │  ...   │ ...  │ ...  │
          ├───────┼───────┼───────┤     │
 A[7,k] ─→│  PE₇₀  │  PE₇₁  │ ...  │ PE₇₇  │
          └───────┴───────┴───────┘     │
```

- 8 × 8 = 64 PEs
- Each PE: `acc += A[row][k] × B[k][col]` (INT8 × INT8 → INT32)
- A broadcast by row, B broadcast by column
- One cycle per `k` (inner dimension), Cin cycles per output tile

### 4.2 Data Flow: A-Cached, B-Streamed

```
for each activation row-tile (T=8 rows × Cin):
    load A_tile from row-buffer → subtract zp_act → cache in array

    for each weight column-tile (Cin × T output channels):
        stream B_tile through the array
        acc = A_tile @ B_tile                # (T, Cin) × (Cin, T) → (T, T) INT32
        acc += bias_tile                     # INT32 bias
        out = clamp(round(acc × mul) + zp_out, -128, 127)  # INT8
        write out_tile to SRAM
```

### 4.3 Memory Layout: Tiled Rows

Activations and weights are stored in **tiled row order** — groups of T consecutive rows:

```
Standard:     [r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, ...]
Tiled (T=8):  [block0: r0..r7,  block1: r8..r15, ...]

block_b = rows[b*T : (b+1)*T]   →   shape (T, K)
```

Benefits:
- One burst read = one tile = one A-cache load
- No gather/scatter address generation
- Weights: stored as `(Cout//T, Cin, T)` column-blocks, streamed sequentially

## 5. Requantization Precision

### mul_q: UINT16 + right-shift

The multiplier `mul = s_act × s_w / s_out` is implemented as a fixed-point multiply:

```
mul_q = round(mul × 2^shift)    # UINT16, stored per layer
shift                            # UINT5 (0..31), stored per layer
```

For this model `mul ∈ [0.0014, 0.052]`, a shift of 16 gives `mul_q ∈ [90, 3389]`, easily within UINT16 range.

Requant in RTL:

```
tmp = (acc_i32 × mul_q) >> shift    # fixed-point → INT32
out = clamp(tmp + zp_out, -128, 127) # add zp, clamp → INT8
```

### zp_out: INT8

Always `[-128, 127]`, stored per layer as signed 8-bit.

### Precision Budget

| Step | Width | Error Source |
|------|-------|-------------|
| MAC multiply | INT8 × INT8 → INT32 | Exact |
| MAC accumulate | INT32 | Exact |
| Bias add | INT32 + INT32 → INT32 | Exact |
| Mul_q multiply | INT32 × UINT16 → INT48, >> shift → INT32 | Rounding (~0.5 LSB) |
| Add zp_out | INT32 + INT8 → INT32 | Exact |
| Clamp | INT32 → INT8 | Truncation |

Total error per requant: < 1 LSB of INT8. Matches OpenCV `ConvolutionInt8` within floating-point rounding.

## 6. Depthwise → Pointwise Pipeline

The backbone alternates depthwise 3×3 (DW) → pointwise 1×1 (PW). These can be pipelined with a double-buffered row FIFO:

```
         ┌──────────────┐   T rows ready   ┌──────────────┐
         │  Depthwise   │─────────────────→│  Pointwise    │
         │  3×3 streaming│  row_buf[fill]   │  GEMM 8×8    │
         │              │←─────────────────│              │
         └──────────────┘   buf_id ^= 1    └──────────────┘
```

- Depthwise writes output rows (W × C) into buffer[fill]
- When T=8 rows are full, the buffers swap
- Pointwise loads the full buffer into A-cache and streams all weight columns
- Depthwise immediately starts filling the other buffer
- Zero stalling: the 8-row FIFO decouples the two stages

The row buffer size for the largest feature map (level 8, 80×80×64): 2 × 8 × 80 × 64 = 80 KB.

## 7. Graph Structure (FPN Detail)

YuNet uses a Feature Pyramid Network with 3 scales (8×, 16×, 32× downsampled):

```
Input (640×640, 3ch)
  │
  ┌─── Regular 3×3 conv (3→16) ───┐
  │                                │
  ├─── GEMM 1×1 (16→16)           │
  ├─── DW 3×3 (16→16)             │
  │              ...              │
  │     (4× downsampling stages)  │
  │              ...              │
  │                                │
  ├─── Backbone_8 (64ch) ─────────┤
  ├─── Backbone_16 (64ch) ────────┤
  ├─── Backbone_32 (64ch) ────────┤
  │                                │
  │     FPN neck:                  │
  │      32_conv → resize → add ← 16_lateral
  │      16_conv → resize → add ← 8_lateral
  │                                │
  │     Detection heads per scale: │
  │      cls (1ch) → Sigmoid       │
  │      obj (1ch) → Sigmoid       │
  │      bbox (4ch) → Dequant      │
  │      kps (10ch) → Dequant      │
  │                                │
  └─── Scores: √(obj × cls) ──────┘
```

## 8. Validation Against OpenCV DNN

To verify an INT8 hardware simulation matches the reference implementation:

```python
import cv2, numpy as np

# Reference: OpenCV 4.13+ ConvolutionInt8
net = cv2.dnn.readNetFromONNX('face_detection_yunet_2023mar_int8.onnx')
img_640 = cv2.resize(cv2.imread('test.png'), (640, 640))
blob = cv2.dnn.blobFromImage(img_640, 1.0, (640, 640), swapRB=False)
net.setInput(blob)

# Get intermediate tensors (OpenCV must store them)
net.enableFusion(False)
out = net.forward(net.getLayer(layer_id).name)
tensor_cv = out[0].squeeze()  # (C, H, W) int8 values as float32

# Compare with your simulation tensor in (H, W, C) layout
sim = tensor.transpose(2, 0, 1)  # if your sim uses HWC
assert np.array_equal(sim.astype(np.int16), tensor_cv.astype(np.int16))
```

### Expected Output Ranges

| Tensor | Range INT8 | After Dequant (FP32) |
|--------|-----------|---------------------|
| First conv (419_quantized) | [-128, 103] | — |
| obj_8_quantized (sigmoid) | [-128, 127] | [0.000, 0.996] |
| cls_8_quantized (sigmoid) | [-128, 127] | [0.389, 0.937] |
| obj_8 (final) | — | [0.000, 0.996] |
| cls_8 (final) | — | [0.389, 0.937] |

### Detection Check

At `conf_threshold=0.9`, the model should detect 7-12 faces in a typical group photo (640×640 input). Scores range from 0.90 to 0.93.

## 9. Common Pitfalls

### 9.1 UINT8 Clamping

**Don't** clamp intermediate conv outputs to `[0, 255]` based on zp_out sign. All intermediate tensors are INT8 `[-128, 127]` regardless of whether the ONNX model specifies unsigned output. The zero-point handles the offset.

```
# WRONG — loses negative values:
if zp_out >= 0:
    out = clip(q, 0, 255).astype(uint8)

# RIGHT — always INT8:
out = clip(q, -128, 127).astype(int8)
```

### 9.2 INT16 Overflow in MAC

The dot product of 3 input channels (first conv) with values up to 255×127 exceeds INT16 range (32,767). Always accumulate in INT32 or wider:

```
# WRONG — overflows:
acc += int16_val @ int16_w.T     # int16 dot product overflows

# RIGHT:
acc += int32_val @ int32_w.T     # int32 matmul
acc = int64_accumulator           # or use int64 for safety
```

### 9.3 Double Quantization

When the ONNX graph has a `QuantizeLinear` node at the input, don't pre-quantize the image manually — store the raw uint8 and let the QuantizeLinear stage handle it.

### 9.4 MaxPool on Quantized Data

`MaxPool` in the ONNX graph operates on **dequantized FP32** values (after DequantizeLinear, before QuantizeLinear). Don't truncate to int32 before maxpooling.

### 9.5 Bias Scaling

The INT32 bias in QLinearConv is pre-scaled by `1/(s_act × s_w)`. Add it directly to the INT32 accumulator — don't scale it again:

```
# RIGHT:
acc_i32 = conv_int32 + bias_int32
out = round(acc_i32 × s_act × s_w / s_out) + zp_out

# WRONG (double-scaling):
acc_fp = conv_fp + bias_int32 × s_act × s_w
```

## 10. Files

| File | Description |
|------|-------------|
| `face_detection_yunet_2023mar_int8.onnx` | INT8 QLinearConv model (112 nodes) |
| `face_detection_yunet_2023mar_int8bq.onnx` | Block-quantized FP32 Conv model (higher accuracy, 214 nodes) |
| `sim_pynq.py` | FP32-based reference simulation (matches OpenCV exactly) |
| `sim_hw.py` | Pure-INT8 hardware-alike pipeline with folded requant (112 stages) |
| `gemm_accelerator.py` | 8×8 INT8 systolic GEMM accelerator template |
