#include "projection.h"

Point2 Projection::WorldToScreen(const ProjectionState& state, Point2 world)
{
    float screenX = (world.x - state.originX) * state.zoom + state.width * 0.5f;
    float screenY = (state.height * 0.5f) - (world.y - state.originY) * state.zoom;
    return { screenX, screenY };
}

Point2 Projection::ScreenToWorld(const ProjectionState& state, Point2 screen)
{
    float worldX = (screen.x - state.width * 0.5f) / state.zoom + state.originX;
    float worldY = (state.height * 0.5f - screen.y) / state.zoom + state.originY;
    return { worldX, worldY };
}
