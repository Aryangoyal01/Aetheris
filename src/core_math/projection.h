#pragma once

#include "bounds.h"

struct ProjectionState
{
    float width = 0.0f;
    float height = 0.0f;
    float zoom = 1.0f;
    float originX = 0.0f;
    float originY = 0.0f;
};

namespace Projection
{
    Point2 WorldToScreen(const ProjectionState& state, Point2 world);
    Point2 ScreenToWorld(const ProjectionState& state, Point2 screen);
}
