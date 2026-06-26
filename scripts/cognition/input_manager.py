import threading
import queue
from abc import ABC, abstractmethod


class InputSource(ABC):
    @abstractmethod
    def start(self):
        pass

    @abstractmethod
    def stop(self):
        pass

    @abstractmethod
    def is_running(self) -> bool:
        pass


class TerminalInput(InputSource):
    def __init__(self, prompt_queue: queue.Queue):
        self._queue = prompt_queue
        self._running = False
        self._thread = None

    def start(self):
        self._running = True
        self._thread = threading.Thread(target=self._loop, daemon=True)
        self._thread.start()

    def stop(self):
        self._running = False

    def is_running(self) -> bool:
        return self._running

    def _loop(self):
        print("[Cognition] Terminal input active. Type a prompt and press Enter.")
        print("[Cognition] Type 'quit' to stop.")
        while self._running:
            try:
                user_input = input("> ").strip()
                if user_input.lower() == 'quit':
                    self._running = False
                    break
                if user_input:
                    self._queue.put(user_input)
            except EOFError:
                break
            except Exception as e:
                print(f"[Cognition] Input error: {e}")


class InputManager:
    def __init__(self):
        self._prompt_queue = queue.Queue()
        self._sources = []

    def add_source(self, source: InputSource):
        self._sources.append(source)

    def start_all(self):
        for source in self._sources:
            source.start()

    def stop_all(self):
        for source in self._sources:
            source.stop()

    def get_prompt(self):
        try:
            return self._prompt_queue.get_nowait()
        except queue.Empty:
            return None

    def has_prompt(self) -> bool:
        return not self._prompt_queue.empty()
