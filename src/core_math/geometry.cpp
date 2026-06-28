#include "geometry.h"
#include <cmath>

namespace Math
{
    float Distance(Point2 a, Point2 b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float DistanceSquared(Point2 a, Point2 b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return dx * dx + dy * dy;
    }

    float Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }
}
