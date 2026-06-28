#include "input.h"
#include "raylib.h"

void Input::Update()
{
    // Polling is handled by Raylib each frame.
    // This function exists as a placeholder for future
    // input processing (e.g., accumulating deltas).
}

Point2 Input::MousePosition()
{
    Vector2 pos = GetMousePosition();
    return { pos.x, pos.y };
}

Point2 Input::MouseDelta()
{
    Vector2 delta = GetMouseDelta();
    return { delta.x, delta.y };
}

float Input::MouseWheel()
{
    return GetMouseWheelMove();
}

bool Input::MouseButtonDown(int button)
{
    return IsMouseButtonDown(button);
}

bool Input::MouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

bool Input::MouseButtonReleased(int button)
{
    return IsMouseButtonReleased(button);
}

bool Input::KeyDown(int key)
{
    return IsKeyDown(key);
}

bool Input::KeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool Input::KeyReleased(int key)
{
    return IsKeyReleased(key);
}
