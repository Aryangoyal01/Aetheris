#pragma once

#include "core_math/bounds.h"
#include "raylib.h" // For Color struct

struct SpawnRequest
{
    Point2 position;
    Point2 velocity;
    float radius;
    float lifetime;
    Color color;
};
