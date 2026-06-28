#pragma once

#include "bounds.h"

namespace Math
{
    float Distance(Point2 a, Point2 b);
    float DistanceSquared(Point2 a, Point2 b);
    float Clamp(float value, float min, float max);
    float Lerp(float a, float b, float t);
}
