#include "bounds.h"

bool Rect::Contains(const Point2& point) const
{
    return point.x >= x && point.x <= x + width
        && point.y >= y && point.y <= y + height;
}

float Rect::Width() const
{
    return width;
}

float Rect::Height() const
{
    return height;
}

Point2 Rect::Center() const
{
    return { x + width * 0.5f, y + height * 0.5f };
}
