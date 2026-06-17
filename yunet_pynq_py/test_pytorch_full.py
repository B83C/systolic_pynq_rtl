import torch
import cv2 as cv
import numpy as np
import math

device = torch.device('cpu')

# Load converted model
model = torch.load('yunet_2026may_full.pt', map_location=device, weights_only=False)
model.eval()

# Load and preprocess image
img = cv.imread('example_outputs/largest_selfie.jpg')
h, w = img.shape[:2]

# Preprocess to 320x320
input_size = (320, 320)
img_resized = cv.resize(img, input_size)
img_rgb = cv.cvtColor(img_resized, cv.COLOR_BGR2RGB)
img_float = img_rgb.astype(np.float32) / 255.0
img_tensor = torch.from_numpy(img_float).permute(2, 0, 1).unsqueeze(0)

# Run inference
with torch.no_grad():
    outputs = model(img_tensor)

# Outputs: 12 tensors in order: cls_8, cls_16, cls_32, obj_8, obj_16, obj_32,
#                                   bbox_8, bbox_16, bbox_32, kps_8, kps_16, kps_32
cls_list = [outputs[0], outputs[1], outputs[2]]
obj_list = [outputs[3], outputs[4], outputs[5]]
bbox_list = [outputs[6], outputs[7], outputs[8]]
kps_list = [outputs[9], outputs[10], outputs[11]]
strides = [8, 16, 32]

# Build anchors
def generate_anchors(feat_h, feat_w, stride):
    anchors = []
    for y in range(feat_h):
        for x in range(feat_w):
            cx = (x + 0.5) * stride
            cy = (y + 0.5) * stride
            anchors.append([cx, cy])
    return np.array(anchors, dtype=np.float32)

all_boxes = []
all_scores = []
all_kps = []

conf_thresh = 0.6
nms_thresh = 0.3

for idx, (cls, obj, bbox, kps, stride) in enumerate(zip(cls_list, obj_list, bbox_list, kps_list, strides)):
    cls = cls.squeeze().numpy()  # [N, 1] -> [N]
    obj = obj.squeeze().numpy()  # [N, 1] -> [N]
    bbox = bbox.squeeze().numpy()  # [N, 4]
    kps = kps.squeeze().numpy()  # [N, 10]

    feat_h = math.ceil(320 / stride)
    feat_w = math.ceil(320 / stride)
    anchors = generate_anchors(feat_h, feat_w, stride)
    N = len(anchors)

    scores = 1 / (1 + np.exp(-cls))

    # Decode bboxes: [dx, dy, dw, dh] -> [x, y, w, h]
    anchor_centers = anchors
    bw = np.exp(bbox[:, 2]) * stride
    bh = np.exp(bbox[:, 3]) * stride
    bx = bbox[:, 0] * stride + anchor_centers[:, 0]
    by = bbox[:, 1] * stride + anchor_centers[:, 1]
    decoded_boxes = np.stack([bx - bw / 2, by - bh / 2, bw, bh], axis=1)

    # Decode kps
    decoded_kps = np.zeros_like(kps)
    for i in range(5):
        decoded_kps[:, i*2] = kps[:, i*2] * stride + anchor_centers[:, 0]
        decoded_kps[:, i*2+1] = kps[:, i*2+1] * stride + anchor_centers[:, 1]

    # Filter by confidence
    keep = scores >= conf_thresh
    all_boxes.append(decoded_boxes[keep])
    all_scores.append(scores[keep])
    all_kps.append(decoded_kps[keep])

all_boxes = np.concatenate(all_boxes, axis=0)
all_scores = np.concatenate(all_scores, axis=0)
all_kps = np.concatenate(all_kps, axis=0)

# NMS
indices = cv.dnn.NMSBoxes(all_boxes.tolist(), all_scores.tolist(), conf_thresh, nms_thresh)
if len(indices) > 0:
    indices = indices.flatten()
    all_boxes = all_boxes[indices]
    all_scores = all_scores[indices]
    all_kps = all_kps[indices]

# Scale boxes back to original image size
scale_x = w / 320
scale_y = h / 320
all_boxes[:, 0] *= scale_x
all_boxes[:, 1] * scale_y
all_boxes[:, 2] *= scale_x
all_boxes[:, 3] *= scale_y
for i in range(5):
    all_kps[:, i*2] *= scale_x
    all_kps[:, i*2+1] *= scale_y

print(f'PyTorch: {len(all_boxes)} faces detected')
for i in range(min(3, len(all_boxes))):
    print(f'  [{i}] score={all_scores[i]:.4f}, box={all_boxes[i].astype(int)}')

# Compare with OpenCV
model_cv = cv.FaceDetectorYN.create(
    model='face_detection_yunet_2026may.onnx',
    config='',
    input_size=[320, 320],
    score_threshold=0.6,
    nms_threshold=0.3,
    top_k=5000)
model_cv.setInputSize([w, h])
_, results_cv = model_cv.detect(img)
print(f'\nOpenCV: {results_cv.shape[0]} faces detected')
for i in range(min(3, results_cv.shape[0])):
    print(f'  [{i}] score={results_cv[i, -1]:.4f}, box={results_cv[i, :4].astype(int)}')
