"""
Simple MJPEG streamer — run on your laptop.
Usage: python3 stream.py [port]

Serves webcam at http://YOUR_IP:8080/stream
Find YOUR_IP with: ip addr | grep "inet "
"""
import cv2, sys, time
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse

PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8080

class StreamHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path != '/stream':
            self.send_response(404); self.end_headers(); return
        self.send_response(200)
        self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=frame')
        self.send_header('Cache-Control', 'no-cache')
        self.send_header('Connection', 'close')
        self.end_headers()
        cap = cv2.VideoCapture(0)
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        try:
            while True:
                ret, frame = cap.read()
                if not ret: break
                _, buf = cv2.imencode('.jpg', frame, [cv2.IMWRITE_JPEG_QUALITY, 70])
                self.wfile.write(b'--frame\r\n')
                self.wfile.write(f'Content-Length: {len(buf)}\r\n'.encode())
                self.wfile.write(b'Content-Type: image/jpeg\r\n\r\n')
                self.wfile.write(buf.tobytes())
                self.wfile.flush()
        finally:
            cap.release()

print(f"Streaming at http://0.0.0.0:{PORT}/stream")
print(f"Find your IP: ip addr | grep 'inet '")
HTTPServer(('0.0.0.0', PORT), StreamHandler).serve_forever()
