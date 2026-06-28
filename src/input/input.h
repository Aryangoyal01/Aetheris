#pragma once

#include "core_math/bounds.h"

class Input
{
public:
    static void Update();

    static Point2 MousePosition();
    static Point2 MouseDelta();
    static float MouseWheel();

    static bool MouseButtonDown(int button);
    static bool MouseButtonPressed(int button);
    static bool MouseButtonReleased(int button);

    static bool KeyDown(int key);
    static bool KeyPressed(int key);
    static bool KeyReleased(int key);
};
