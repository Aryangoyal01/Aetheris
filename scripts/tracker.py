import cv2
import mediapipe as mp
import socket
import math
import os
import struct
import urllib.request
import time
import threading
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

from cognition import InputManager, TerminalInput, GeminiClient


UDP_IP = "127.0.0.1"
UDP_PORT = 8080
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

CAMERA_WIDTH = 1280
CAMERA_HEIGHT = 720
DISPLAY_WIDTH = 1280
DISPLAY_HEIGHT = 720

PACKET_VERSION = 1
PACKET_TYPE_TRACKING = 0x01


MODEL_PATH = "hand_landmarker.task"
if not os.path.exists(MODEL_PATH):
    print("[!] Downloading hand detection model...")
    url = "https://storage.googleapis.com/mediapipe-models/hand_landmarker/hand_landmarker/float16/latest/hand_landmarker.task"
    urllib.request.urlretrieve(url, MODEL_PATH)

base_options = python.BaseOptions(model_asset_path=MODEL_PATH)
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    running_mode=vision.RunningMode.IMAGE,
    num_hands=1,
    min_hand_detection_confidence=0.5
)
detector = vision.HandLandmarker.create_from_options(options)

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
cap.set(cv2.CAP_PROP_FPS, 60)
prev_frame_time = 0

cv2.namedWindow("Aetheris Vision Deck", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Aetheris Vision Deck", DISPLAY_WIDTH, DISPLAY_HEIGHT)

input_manager = InputManager()
input_manager.add_source(TerminalInput(input_manager._prompt_queue))

gemini_client = GeminiClient(input_manager)
gemini_client.start()
input_manager.start_all()

sequence_counter = 0

print(f"[+] Tracker Live. Camera set to {CAMERA_WIDTH}x{CAMERA_HEIGHT}.")
print(f"[+] Sending data to port {UDP_PORT}")
print(f"[+] Cognition layer active (type prompts in terminal)")

try:
    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            continue

        frame_height, frame_width, _ = frame.shape

        new_frame_time = time.time()
        fps = int(1 / (new_frame_time - prev_frame_time + 0.0001))
        prev_frame_time = new_frame_time

        frame = cv2.flip(frame, 1)
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb_frame)

        detection_result = detector.detect(mp_image)

        detected, pixel_x, pixel_y, is_pinch = 0, 0, 0, 0
        norm_x, norm_y = 0.0, 0.0

        if detection_result.hand_landmarks:
            detected = 1
            hand_landmarks = detection_result.hand_landmarks[0]
            palm = hand_landmarks[0]

            pixel_x = int(palm.x * frame_width)
            pixel_y = int(palm.y * frame_height)
            norm_x, norm_y = round(palm.x, 4), round(palm.y, 4)

            dist = math.sqrt((hand_landmarks[8].x - hand_landmarks[4].x)**2 +
                             (hand_landmarks[8].y - hand_landmarks[4].y)**2)
            is_pinch = 1 if dist < 0.03 else 0

            for lm in hand_landmarks:
                cx = int(lm.x * frame_width)
                cy = int(lm.y * frame_height)
                cv2.circle(frame, (cx, cy), 4, (0, 255, 0), -1)

            cv2.circle(frame, (pixel_x, pixel_y), 12, (0, 0, 255), 2)

        cv2.putText(frame, f"FPS: {fps} | PINCH: {is_pinch}", (20, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255, 255, 0), 3)

        sequence_counter += 1
        csv_payload = f"{detected},{pixel_x},{pixel_y},{norm_x},{norm_y},{is_pinch}"
        csv_data = csv_payload.encode('utf-8')

        header = struct.pack('<BBHI', PACKET_VERSION, PACKET_TYPE_TRACKING, len(csv_data), sequence_counter)
        packet = header + csv_data
        sock.sendto(packet, (UDP_IP, UDP_PORT))

        cv2.imshow("Aetheris Vision Deck", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    gemini_client.stop()
    input_manager.stop_all()
    cap.release()
    cv2.destroyAllWindows()
    sock.close()
