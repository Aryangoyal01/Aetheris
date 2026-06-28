#pragma once

#include "core_math/bounds.h"
#include "core_math/projection.h"

class Viewport
{
public:
    void Resize(float width, float height);
    void ZoomAtPoint(float screenX, float screenY, float factor);
    void Pan(float screenDeltaX, float screenDeltaY);
    void SetOrigin(float x, float y);

    Point2 WorldToScreen(Point2 world) const;
    Point2 ScreenToWorld(Point2 screen) const;

    bool VerifyRoundtrip() const;

    float Width() const;
    float Height() const;
    float Zoom() const;
    float OriginX() const;
    float OriginY() const;
    float MinZoom() const;

    Rect GetWorldBounds() const;

    const ProjectionState& GetState() const;

private:
    ProjectionState m_state;

    void ClampOrigin();

    static constexpr float MAX_ZOOM = 100.0f;
};
