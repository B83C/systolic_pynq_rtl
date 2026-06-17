import torch
import cv2 as cv
import numpy as np
import math

device = torch.device('cpu')
model = torch.load('yunet_2026may_full.pt', map_location=device, weights_only=False)
model.eval()

strides = [8, 16, 32]
conf_thresh = 0.6
nms_thresh = 0.3
top_k = 5000

cap = cv.VideoCapture(1)
if not cap.isOpened():
    print("Failed to open /dev/video1")
    exit(1)

print("Press 'q' to quit")
tm = cv.TickMeter()

while True:
    has_frame, frame = cap.read()
    if not has_frame:
        break

    h, w = frame.shape[:2]
    h = int(h)
    w = int(w)
    tm.start()

    # Preprocess: pad to multiple of 32
    pad_w = math.ceil(w / 32) * 32
    pad_h = math.ceil(h / 32) * 32
    pad_img = cv.copyMakeBorder(frame, 0, pad_h - h, 0, pad_w - w, cv.BORDER_CONSTANT, 0)
    blob = cv.dnn.blobFromImage(pad_img)

    # Inference
    with torch.no_grad():
        outputs = model(torch.from_numpy(blob))

    # Post-process
    all_faces = []
    for i, stride in enumerate(strides):
        cls = outputs[i].squeeze().numpy()
        obj = outputs[i + 3].squeeze().numpy()
        bbox = outputs[i + 6].squeeze().numpy()
        kps = outputs[i + 9].squeeze().numpy()

        cols = pad_w // stride
        rows = pad_h // stride

        for r in range(rows):
            for c in range(cols):
                idx = r * cols + c
                cls_s = min(max(cls[idx], 0.0), 1.0)
                obj_s = min(max(obj[idx], 0.0), 1.0)
                score = math.sqrt(cls_s * obj_s)
                if score < conf_thresh:
                    continue

                cx = (c + bbox[idx, 0]) * stride
                cy = (r + bbox[idx, 1]) * stride
                bw = math.exp(bbox[idx, 2]) * stride
                bh = math.exp(bbox[idx, 3]) * stride
                x1 = cx - bw / 2
                y1 = cy - bh / 2

                landmarks = np.zeros(10, dtype=np.float32)
                for n in range(5):
                    landmarks[2*n] = (kps[idx, 2*n] + c) * stride
                    landmarks[2*n+1] = (kps[idx, 2*n+1] + r) * stride

                all_faces.append(np.array([x1, y1, bw, bh, *landmarks, score], dtype=np.float32))

    all_faces = np.array(all_faces) if all_faces else np.empty((0, 15))
    if len(all_faces) > 0:
        boxes = all_faces[:, :4].astype(np.int32).tolist()
        scores = all_faces[:, 14].tolist()
        keep = cv.dnn.NMSBoxes(boxes, scores, conf_thresh, nms_thresh, top_k=top_k)
        if len(keep) > 0:
            keep = keep.flatten()
            all_faces = all_faces[keep]

    tm.stop()
    fps = tm.getFPS()
    tm.reset()

    # Draw
    for face in all_faces:
        x, y, bw, bh = face[:4].astype(int)
        score = face[14]
        cv.rectangle(frame, (x, y), (x + bw, y + bh), (0, 255, 0), 2)
        cv.putText(frame, f'{score:.3f}', (x, y - 5), cv.FONT_HERSHEY_DUPLEX, 0.5, (0, 0, 255))

        landmarks = face[4:14].reshape(5, 2).astype(int)
        for lm in landmarks:
            cv.circle(frame, lm, 2, (255, 0, 0), 2)

    cv.putText(frame, f'FPS: {fps:.1f}', (10, 30), cv.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
    cv.imshow('YuNet PyTorch', frame)

    if cv.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv.destroyAllWindows()
