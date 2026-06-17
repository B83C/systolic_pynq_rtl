import torch
import cv2 as cv
import numpy as np

device = torch.device('cpu')

# Load converted model
model = torch.load('yunet_2026may_full.pt', map_location=device, weights_only=False)
model.eval()

# Load and preprocess image
img = cv.imread('example_outputs/largest_selfie.jpg')
h, w = img.shape[:2]

# Preprocess: resize to 320x320, normalize
input_size = (320, 320)
img_resized = cv.resize(img, input_size)
img_rgb = cv.cvtColor(img_resized, cv.COLOR_BGR2RGB)
img_float = img_rgb.astype(np.float32) / 255.0
img_tensor = torch.from_numpy(img_float).permute(2, 0, 1).unsqueeze(0)  # [1,3,320,320]

# Run inference
with torch.no_grad():
    outputs = model(img_tensor)

print('Number of output tensors:', len(outputs))
for i, o in enumerate(outputs):
    print(f'  output[{i}]: shape={o.shape}, min={o.min().item():.4f}, max={o.max().item():.4f}')

# Now run with OpenCV for comparison
model_cv = cv.FaceDetectorYN.create(
    model='face_detection_yunet_2026may.onnx',
    config='',
    input_size=[320, 320],
    score_threshold=0.6,
    nms_threshold=0.3,
    top_k=5000,
)
model_cv.setInputSize([w, h])
_, results_cv = model_cv.detect(img)
print(f'\nOpenCV: {results_cv.shape[0]} faces detected')

# Save the raw outputs for later use
output_dict = {f'output_{i}': o.numpy() for i, o in enumerate(outputs)}
np.savez('yunet_raw_outputs.npz', **output_dict)
print('Raw outputs saved to yunet_raw_outputs.npz')
