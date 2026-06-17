import torch
import cv2 as cv
import numpy as np
import math

device = torch.device('cpu')

# Load converted PyTorch model
model = torch.load('yunet_2026may_full.pt', map_location=device, weights_only=False)
model.eval()

# Load image
img = cv.imread('example_outputs/largest_selfie.jpg')
h, w = img.shape[:2]

# OpenCV preprocessing:
# 1. Pad to multiple of 32
divisor = 32
pad_w = math.ceil(w / divisor) * divisor
pad_h = math.ceil(h / divisor) * divisor
pad_img = cv.copyMakeBorder(img, 0, pad_h - h, 0, pad_w - w, cv.BORDER_CONSTANT, value=0)

# 2. blobFromImage with defaults: no resize, no swapRB, no scaling
blob = cv.dnn.blobFromImage(pad_img)  # [1, 3, pad_h, pad_w], BGR, [0,255]
input_tensor = torch.from_numpy(blob)

# Run inference
with torch.no_grad():
    outputs = model(input_tensor)

strides = [8, 16, 32]
conf_thresh = 0.6
nms_thresh = 0.3
top_k = 5000

all_faces = []

for i, stride in enumerate(strides):
    cls = outputs[i].squeeze().numpy()        # [N]
    obj = outputs[i + 3].squeeze().numpy()     # [N]
    bbox = outputs[i + 6].squeeze().numpy()    # [N, 4]
    kps = outputs[i + 9].squeeze().numpy()     # [N, 10]

    cols = pad_w // stride
    rows = pad_h // stride

    for r in range(rows):
        for c in range(cols):
            idx = r * cols + c

            # OpenCV score = sqrt(cls * obj), clamped to [0,1]
            cls_score = min(max(cls[idx], 0.0), 1.0)
            obj_score = min(max(obj[idx], 0.0), 1.0)
            score = math.sqrt(cls_score * obj_score)

            if score < conf_thresh:
                continue

            # Decode bbox
            cx = (c + bbox[idx, 0]) * stride
            cy = (r + bbox[idx, 1]) * stride
            bw = math.exp(bbox[idx, 2]) * stride
            bh = math.exp(bbox[idx, 3]) * stride
            x1 = cx - bw / 2
            y1 = cy - bh / 2

            # Decode landmarks
            landmarks = np.zeros(10, dtype=np.float32)
            for n in range(5):
                landmarks[2*n] = (kps[idx, 2*n] + c) * stride
                landmarks[2*n+1] = (kps[idx, 2*n+1] + r) * stride

            face = np.array([x1, y1, bw, bh, *landmarks, score], dtype=np.float32)
            all_faces.append(face)

all_faces = np.array(all_faces) if all_faces else np.empty((0, 15))

# NMS
if len(all_faces) > 0:
    boxes = all_faces[:, :4].astype(np.int32).tolist()
    scores = all_faces[:, 14].tolist()
    keep = cv.dnn.NMSBoxes(boxes, scores, conf_thresh, nms_thresh, top_k=top_k)
    if len(keep) > 0:
        keep = keep.flatten()
        all_faces = all_faces[keep]

print(f'PyTorch (corrected): {len(all_faces)} faces detected')
for i in range(min(5, len(all_faces))):
    print(f'  [{i}] score={all_faces[i, 14]:.4f}, box={all_faces[i, :4].astype(int)}')

# Compare with OpenCV
model_cv = cv.FaceDetectorYN.create(
    model='face_detection_yunet_2026may.onnx',
    config='',
    input_size=[w, h],
    score_threshold=conf_thresh,
    nms_threshold=nms_thresh,
    top_k=top_k)
_, results_cv = model_cv.detect(img)
print(f'\nOpenCV: {results_cv.shape[0]} faces detected')
for i in range(min(5, results_cv.shape[0])):
    print(f'  [{i}] score={results_cv[i, 14]:.4f}, box={results_cv[i, :4].astype(int)}')

# Check matching rate
if len(all_faces) > 0 and results_cv.shape[0] > 0:
    pt_scores = set([round(s, 2) for s in all_faces[:, 14]])
    cv_scores = set([round(s, 2) for s in results_cv[:, 14]])
    common = pt_scores & cv_scores
    print(f'\nScore match: {len(common)} common scores out of {len(pt_scores)} PyTorch / {len(cv_scores)} OpenCV')
    print(f'PyTorch scores: {sorted(pt_scores, reverse=True)[:5]}')
    print(f'OpenCV scores: {sorted(cv_scores, reverse=True)[:5]}')
