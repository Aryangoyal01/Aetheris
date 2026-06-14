import cv2
import mediapipe as mp
import socket
import math
import os
import urllib.request
import time
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

# ==========================================
# 1. CONFIGURATION & RESOLUTION VARIABLES
# ==========================================

# Network settings for the C++ Engine
UDP_IP = "127.0.0.1"
UDP_PORT = 8080
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Camera Resolution Settings (Easily change these to experiment)
# Options: HD (1280x720), FHD (1920x1080), QHD (2560x1440)
# Note: 1920x1080 is the sweet spot for MediaPipe to maintain 60 FPS.
CAMERA_WIDTH = 1280
CAMERA_HEIGHT = 720

# Display Window Settings (How big it appears on your physical monitor)
# This lets the camera run at 1080p or 1440p, but shrinks the window so you can actually see it.
DISPLAY_WIDTH = 1280
DISPLAY_HEIGHT = 720


# ==========================================
# 2. MODEL INITIALIZATION
# ==========================================

MODEL_PATH = "hand_landmarker.task"
if not os.path.exists(MODEL_PATH):
    print("[!] Downloading hand detection model...")
    url = "https://storage.googleapis.com/mediapipe-models/hand_landmarker/hand_landmarker/float16/latest/hand_landmarker.task"
    urllib.request.urlretrieve(url, MODEL_PATH)

# Set up MediaPipe detector options
base_options = python.BaseOptions(model_asset_path=MODEL_PATH)
options = vision.HandLandmarkerOptions(
    base_options=base_options,
    running_mode=vision.RunningMode.IMAGE,
    num_hands=1, # We only track one hand for this engine
    min_hand_detection_confidence=0.5
)
detector = vision.HandLandmarker.create_from_options(options)


# ==========================================
# 3. CAMERA SETUP & WINDOW SCALING
# ==========================================

# cap = cv2.VideoCapture(1) accesses your phone webcam (0 is usually built-in laptop cam)
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT)
cap.set(cv2.CAP_PROP_FPS, 60)
prev_frame_time = 0

# --- THE WINDOW FIX ---
# cv2.WINDOW_NORMAL allows the window to be resized. Without this, a 1440p camera 
# feed will create a massive 1440p window that spills off your monitor.
cv2.namedWindow("Aetheris Vision Deck", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Aetheris Vision Deck", DISPLAY_WIDTH, DISPLAY_HEIGHT)

print(f"[+] Tracker Live. Camera set to {CAMERA_WIDTH}x{CAMERA_HEIGHT}.")
print(f"[+] Sending data to port {UDP_PORT}")


# ==========================================
# 4. MAIN VISION LOOP
# ==========================================

try:
    while cap.isOpened():
        success, frame = cap.read()
        if not success: 
            continue
            
        # --- THE FIX: GET ACTUAL HARDWARE DIMENSIONS ---
        frame_height, frame_width, _ = frame.shape

        # Calculate FPS
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
            
            # MULTIPLY BY ACTUAL FRAME SIZE, NOT THE CONSTANTS
            pixel_x = int(palm.x * frame_width)
            pixel_y = int(palm.y * frame_height)
            
            norm_x, norm_y = round(palm.x, 4), round(palm.y, 4)

            dist = math.sqrt((hand_landmarks[8].x - hand_landmarks[4].x)**2 + 
                             (hand_landmarks[8].y - hand_landmarks[4].y)**2)
            is_pinch = 1 if dist < 0.03 else 0
                
            for lm in hand_landmarks:
                # DRAW USING ACTUAL FRAME SIZE
                cx = int(lm.x * frame_width)
                cy = int(lm.y * frame_height)
                cv2.circle(frame, (cx, cy), 4, (0, 255, 0), -1)
            
            cv2.circle(frame, (pixel_x, pixel_y), 12, (0, 0, 255), 2)

        cv2.putText(frame, f"FPS: {fps} | PINCH: {is_pinch}", (20, 50), 
                    cv2.FONT_HERSHEY_SIMPLEX, 1.5, (255, 255, 0), 3)
        
        # Send data packet to C++ Engine
        packet = f"{detected},{pixel_x},{pixel_y},{norm_x},{norm_y},{is_pinch}"
        sock.sendto(packet.encode(), (UDP_IP, UDP_PORT))

        # Show the scaled frame
        cv2.imshow("Aetheris Vision Deck", frame)
        
        # Exit when 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'): 
            break

finally:
    # Cleanup resources safely
    cap.release()
    cv2.destroyAllWindows()