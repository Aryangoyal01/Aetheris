#pragma once

struct Point2
{
    float x = 0.0f;
    float y = 0.0f;
};

struct Size2
{
    float width = 0.0f;
    float height = 0.0f;
};

struct Rect
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    bool Contains(const Point2& point) const;
    float Width() const;
    float Height() const;
    Point2 Center() const;
};
