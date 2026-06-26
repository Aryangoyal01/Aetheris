import json
import os
import struct
import socket
import threading
import time
from typing import Optional

from .input_manager import InputManager, TerminalInput


SYSTEM_PROMPT = """You are a physics simulation controller for the Aetheris Engine.
Given a natural language description, output ONLY a valid JSON object with these keys:
- gravity (float, range -20.0 to 20.0): gravitational acceleration
- friction (float, range 0.0 to 1.0): surface friction coefficient
- elasticity (float, range 0.0 to 1.0): bounce elasticity
- time_scale (float, range 0.1 to 5.0): time multiplier
- spawn_particles (string, one of "SINGULARITY", "REPELLER", "PULSAR"): particle type

Example: {"gravity": 0.0, "friction": 0.5, "elasticity": 0.9, "time_scale": 2.0, "spawn_particles": "PULSAR"}

Output ONLY the JSON object. No explanation, no markdown, no code fences."""


UDP_IP = "127.0.0.1"
UDP_PORT = 8080

PACKET_VERSION = 1
PACKET_TYPE_AI = 0x02


class GeminiClient:
    def __init__(self, input_manager: InputManager):
        self._input_manager = input_manager
        self._running = False
        self._thread = None
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sequence = 0
        self._model = None

    def _init_gemini(self):
        try:
            api_key = os.environ.get("GEMINI_API_KEY")
            if not api_key:
                print("[Gemini] WARNING: GEMINI_API_KEY not set. Using mock responses.")
                return False

            from google import genai
            self._model = genai.Client(api_key=api_key)
            print("[Gemini] Client initialized successfully.")
            return True
        except ImportError:
            print("[Gemini] WARNING: google-genai not installed. Using mock responses.")
            return False
        except Exception as e:
            print(f"[Gemini] WARNING: Init failed ({e}). Using mock responses.")
            return False

    def start(self):
        self._running = True
        self._init_gemini()
        self._thread = threading.Thread(target=self._loop, daemon=True)
        self._thread.start()

    def stop(self):
        self._running = False

    def _loop(self):
        print("[Cognition] Gemini client loop active.")
        while self._running:
            prompt = self._input_manager.get_prompt()
            if prompt:
                response_json = self._query_gemini(prompt)
                if response_json:
                    self._send_to_engine(response_json)
            time.sleep(0.05)

    def _query_gemini(self, prompt: str) -> Optional[str]:
        if self._model is None:
            return self._mock_response(prompt)

        try:
            response = self._model.models.generate_content(
                model="gemini-2.0-flash",
                contents=prompt,
                config={
                    "system_instruction": SYSTEM_PROMPT,
                    "temperature": 0.3,
                    "max_output_tokens": 256,
                }
            )
            text = response.text.strip()
            if text.startswith("```"):
                text = text.split("\n", 1)[1]
                if text.endswith("```"):
                    text = text[:-3]
                text = text.strip()
            json.loads(text)
            print(f"[Gemini] Response: {text}")
            return text
        except Exception as e:
            print(f"[Gemini] Error: {e}")
            return self._mock_response(prompt)

    def _mock_response(self, prompt: str) -> str:
        prompt_lower = prompt.lower()
        response = {
            "gravity": 9.81,
            "friction": 0.15,
            "elasticity": 0.75,
            "time_scale": 1.0,
            "spawn_particles": "SINGULARITY"
        }

        if "zero gravity" in prompt_lower or "weightless" in prompt_lower:
            response["gravity"] = 0.0
        elif "heavy" in prompt_lower or "strong gravity" in prompt_lower:
            response["gravity"] = 19.6
        elif "light" in prompt_lower or "weak gravity" in prompt_lower:
            response["gravity"] = 2.0

        if "friction" in prompt_lower:
            if "high" in prompt_lower or "much" in prompt_lower:
                response["friction"] = 0.8
            elif "low" in prompt_lower or "no" in prompt_lower or "ice" in prompt_lower:
                response["friction"] = 0.02

        if "elastic" in prompt_lower or "bouncy" in prompt_lower:
            response["elasticity"] = 0.95
        elif "dead" in prompt_lower or "no bounce" in prompt_lower:
            response["elasticity"] = 0.1

        if "slow" in prompt_lower or "slow motion" in prompt_lower:
            response["time_scale"] = 0.2
        elif "fast" in prompt_lower or "speed up" in prompt_lower:
            response["time_scale"] = 3.0

        if "repel" in prompt_lower or "push" in prompt_lower or "repeller" in prompt_lower:
            response["spawn_particles"] = "REPELLER"
        elif "pulse" in prompt_lower or "orbital" in prompt_lower or "pulsar" in prompt_lower:
            response["spawn_particles"] = "PULSAR"

        result = json.dumps(response)
        print(f"[Gemini-Mock] Prompt: '{prompt}' -> {result}")
        return result

    def _send_to_engine(self, json_str: str):
        self._sequence += 1
        data = json_str.encode('utf-8')

        header = struct.pack('<BBHI', PACKET_VERSION, PACKET_TYPE_AI, len(data), self._sequence)
        packet = header + data

        self._sock.sendto(packet, (UDP_IP, UDP_PORT))
        print(f"[Cognition] Sent AI config (seq={self._sequence}, {len(packet)} bytes)")
