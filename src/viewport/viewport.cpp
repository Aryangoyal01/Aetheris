#include "viewport.h"
#include "core_math/geometry.h"
#include "world/world.h"
#include <cmath>

void Viewport::Resize(float width, float height)
{
    m_state.width = width;
    m_state.height = height;
    m_state.zoom = Math::Clamp(m_state.zoom, MinZoom(), MAX_ZOOM);
    ClampOrigin();
}

void Viewport::ZoomAtPoint(float screenX, float screenY, float factor)
{
    float targetZoom = m_state.zoom * factor;
    targetZoom = Math::Clamp(targetZoom, MinZoom(), MAX_ZOOM);

    Point2 worldBefore = ScreenToWorld({ screenX, screenY });

    m_state.zoom = targetZoom;

    m_state.originX = worldBefore.x - (screenX - m_state.width * 0.5f) / m_state.zoom;
    m_state.originY = worldBefore.y - (m_state.height * 0.5f - screenY) / m_state.zoom;

    ClampOrigin();
}

void Viewport::Pan(float screenDeltaX, float screenDeltaY)
{
    m_state.originX -= screenDeltaX / m_state.zoom;
    m_state.originY += screenDeltaY / m_state.zoom;

    ClampOrigin();
}

void Viewport::SetOrigin(float x, float y)
{
    m_state.originX = x;
    m_state.originY = y;
    ClampOrigin();
}

Point2 Viewport::WorldToScreen(Point2 world) const
{
    return Projection::WorldToScreen(m_state, world);
}

Point2 Viewport::ScreenToWorld(Point2 screen) const
{
    return Projection::ScreenToWorld(m_state, screen);
}

bool Viewport::VerifyRoundtrip() const
{
    Point2 tests[] = {
        { 0.0f, 0.0f },
        { 100.0f, 50.0f },
        { -200.0f, 300.0f },
        { 640.0f, 360.0f },
    };

    for (const auto& p : tests)
    {
        Point2 s = WorldToScreen(p);
        Point2 w = ScreenToWorld(s);

        if (std::fabs(w.x - p.x) > 0.001f || std::fabs(w.y - p.y) > 0.001f)
            return false;
    }

    Point2 mouse = { 400.0f, 300.0f };
    Point2 worldBefore = ScreenToWorld(mouse);

    float savedZoom = m_state.zoom;
    float savedOX = m_state.originX;
    float savedOY = m_state.originY;

    const_cast<Viewport*>(this)->ZoomAtPoint(mouse.x, mouse.y, 2.0f);

    Point2 worldAfter = ScreenToWorld(mouse);

    const_cast<Viewport*>(this)->m_state.zoom = savedZoom;
    const_cast<Viewport*>(this)->m_state.originX = savedOX;
    const_cast<Viewport*>(this)->m_state.originY = savedOY;

    if (std::fabs(worldAfter.x - worldBefore.x) > 0.001f ||
        std::fabs(worldAfter.y - worldBefore.y) > 0.001f)
        return false;

    return true;
}

float Viewport::Width() const { return m_state.width; }
float Viewport::Height() const { return m_state.height; }
float Viewport::Zoom() const { return m_state.zoom; }
float Viewport::OriginX() const { return m_state.originX; }
float Viewport::OriginY() const { return m_state.originY; }

float Viewport::MinZoom() const
{
    float worldWidth = World::BOUNDS_RIGHT - World::BOUNDS_LEFT;
    float worldHeight = World::BOUNDS_TOP - World::BOUNDS_BOTTOM;

    // Frame the world with 10% padding
    float zoomX = m_state.width / (worldWidth * 1.1f);
    float zoomY = m_state.height / (worldHeight * 1.1f);

    return std::fmin(zoomX, zoomY);
}

Rect Viewport::GetWorldBounds() const
{
    return { World::BOUNDS_LEFT, World::BOUNDS_BOTTOM, World::BOUNDS_RIGHT - World::BOUNDS_LEFT, World::BOUNDS_TOP - World::BOUNDS_BOTTOM };
}

void Viewport::ClampOrigin()
{
    float halfViewW = m_state.width * 0.5f / m_state.zoom;
    float halfViewH = m_state.height * 0.5f / m_state.zoom;

    float minOriginX = World::BOUNDS_LEFT + halfViewW;
    float maxOriginX = World::BOUNDS_RIGHT - halfViewW;
    float minOriginY = World::BOUNDS_BOTTOM + halfViewH;
    float maxOriginY = World::BOUNDS_TOP - halfViewH;

    if (minOriginX > maxOriginX)
    {
        minOriginX = (World::BOUNDS_LEFT + World::BOUNDS_RIGHT) * 0.5f;
        maxOriginX = minOriginX;
    }
    if (minOriginY > maxOriginY)
    {
        minOriginY = (World::BOUNDS_BOTTOM + World::BOUNDS_TOP) * 0.5f;
        maxOriginY = minOriginY;
    }

    m_state.originX = Math::Clamp(m_state.originX, minOriginX, maxOriginX);
    m_state.originY = Math::Clamp(m_state.originY, minOriginY, maxOriginY);
}

const ProjectionState& Viewport::GetState() const
{
    return m_state;
}
