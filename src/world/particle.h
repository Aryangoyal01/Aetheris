#pragma once

#include "core_math/bounds.h"

struct Particle
{
    Point2 position = { 0.0f, 0.0f };
    Point2 velocity = { 0.0f, 0.0f };
    float lifetime = 0.0f;
    float maxLifetime = 1.0f;
    float size = 1.0f;
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255;
    bool active = false;
};
