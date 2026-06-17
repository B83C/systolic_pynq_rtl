import argparse
import math

import cv2 as cv
import numpy as np

parser = argparse.ArgumentParser(description='YuNet face detection via raw ONNX model (compatible with OpenCV 4.5.x)')
parser.add_argument('--input', '-i', type=str, required=True)
parser.add_argument('--model', '-m', type=str, default='face_detection_yunet_2026may.onnx')
parser.add_argument('--conf_threshold', type=float, default=0.9)
parser.add_argument('--nms_threshold', type=float, default=0.3)
parser.add_argument('--top_k', type=int, default=5000)
parser.add_argument('--save', '-s', action='store_true')
parser.add_argument('--vis', '-v', action='store_true')
args = parser.parse_args()

OUTPUT_NAMES = ['cls_8', 'cls_16', 'cls_32', 'obj_8', 'obj_16', 'obj_32',
                'bbox_8', 'bbox_16', 'bbox_32', 'kps_8', 'kps_16', 'kps_32']


def pad_to_mult32(img):
    h, w = img.shape[:2]
    pad_h = math.ceil(h / 32) * 32
    pad_w = math.ceil(w / 32) * 32
    return cv.copyMakeBorder(img, 0, pad_h - h, 0, pad_w - w, cv.BORDER_CONSTANT, value=0), h, w


def decode(outputs, pad_w, pad_h, conf_thresh, nms_thresh, top_k):
    strides = [8, 16, 32]

    cls_list = outputs[0:3]
    obj_list = outputs[3:6]
    bbox_list = outputs[6:9]
    kps_list = outputs[9:12]

    all_boxes = []
    all_scores = []
    all_kps = []

    for idx, stride in enumerate(strides):
        cls = cls_list[idx].reshape(-1)
        obj = obj_list[idx].reshape(-1)
        bbox = bbox_list[idx].reshape(-1, 4)
        kps = kps_list[idx].reshape(-1, 10)

        feat_h = math.ceil(pad_h / stride)
        feat_w = math.ceil(pad_w / stride)

        # Generate anchors
        anchors = np.stack(np.meshgrid(np.arange(feat_w), np.arange(feat_h)), axis=-1).reshape(-1, 2).astype(np.float32)
        anchors[:, 0] = (anchors[:, 0] + 0.5) * stride
        anchors[:, 1] = (anchors[:, 1] + 0.5) * stride

        N = len(anchors)

        scores = np.sqrt(np.clip(cls, 0, 1) * np.clip(obj, 0, 1))

        bw = np.exp(bbox[:, 2]) * stride
        bh = np.exp(bbox[:, 3]) * stride
        bx = bbox[:, 0] * stride + anchors[:, 0]
        by = bbox[:, 1] * stride + anchors[:, 1]
        decoded_boxes = np.stack([bx - bw / 2, by - bh / 2, bw, bh], axis=1)

        decoded_kps = np.zeros_like(kps)
        for i in range(5):
            decoded_kps[:, i*2] = kps[:, i*2] * stride + anchors[:, 0]
            decoded_kps[:, i*2+1] = kps[:, i*2+1] * stride + anchors[:, 1]

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


def visualize(image, results):
    output = image.copy()
    colors = [(255, 0, 0), (0, 0, 255), (0, 255, 0), (255, 0, 255), (0, 255, 255)]
    for det in results:
        bbox = det[0:4].astype(np.int32)
        cv.rectangle(output, (bbox[0], bbox[1]), (bbox[0] + bbox[2], bbox[1] + bbox[3]), (0, 255, 0), 2)
        cv.putText(output, '{:.4f}'.format(det[-1]), (bbox[0], bbox[1] + 12), cv.FONT_HERSHEY_DUPLEX, 0.5, (0, 0, 255))
        landmarks = det[4:14].astype(np.int32).reshape((5, 2))
        for i, lm in enumerate(landmarks):
            cv.circle(output, lm, 2, colors[i], 2)
    return output


if __name__ == '__main__':
    net = cv.dnn.readNetFromONNX(args.model)
    img = cv.imread(args.input)
    h, w = img.shape[:2]

    padded, orig_h, orig_w = pad_to_mult32(img)
    pad_h, pad_w = padded.shape[:2]

    blob = cv.dnn.blobFromImage(padded, scalefactor=1.0, size=(pad_w, pad_h), swapRB=False, crop=False)
    net.setInput(blob)
    raw_outputs = net.forward(OUTPUT_NAMES)

    faces = decode(raw_outputs, pad_w, pad_h, args.conf_threshold, args.nms_threshold, args.top_k)

    scale_x = w / pad_w
    scale_y = h / pad_h
    for face in faces:
        face[0] *= scale_x
        face[1] *= scale_y
        face[2] *= scale_x
        face[3] *= scale_y
        for i in range(5):
            face[4 + i*2] *= scale_x
            face[4 + i*2 + 1] *= scale_y

    print('{} faces detected.'.format(len(faces)))
    for idx, det in enumerate(faces):
        print('{}: {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f} {:.0f}'.format(
            idx, *det[:-1]))

    if args.save:
        cv.imwrite('result.jpg', visualize(img, faces))
        print('Results saved to result.jpg')

    if args.vis:
        cv.imshow('YuNet', visualize(img, faces))
        cv.waitKey(0)
