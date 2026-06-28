#include "grid_renderer.h"
#include "viewport/viewport.h"
#include "raylib.h"
#include <cmath>

static constexpr float TARGET_SCREEN_SPACING = 75.0f;

static constexpr Color MINOR_LINE = { 50, 50, 50, 255 };
static constexpr Color MAJOR_LINE = { 70, 70, 70, 255 };
static constexpr Color AXIS_COLOR = { 0, 200, 200, 255 };
static constexpr Color BORDER_COLOR = { 80, 80, 80, 255 };

static constexpr float PREFERRED_SPACINGS[] = {
    0.01f, 0.02f, 0.05f,
    0.1f, 0.2f, 0.5f,
    1.0f, 2.0f, 5.0f,
    10.0f, 20.0f, 50.0f,
    100.0f, 200.0f, 500.0f,
    1000.0f, 2000.0f, 5000.0f
};
static constexpr int PREFERRED_COUNT = sizeof(PREFERRED_SPACINGS) / sizeof(PREFERRED_SPACINGS[0]);

float GridRenderer::ChooseSpacing(float worldSpacing) const
{
    for (int i = 0; i < PREFERRED_COUNT - 1; i++)
    {
        float mid = std::sqrt(PREFERRED_SPACINGS[i] * PREFERRED_SPACINGS[i + 1]);
        if (worldSpacing < mid)
            return PREFERRED_SPACINGS[i];
    }
    return PREFERRED_SPACINGS[PREFERRED_COUNT - 1];
}

void GridRenderer::Render(const Viewport& viewport)
{
    float w = viewport.Width();
    float h = viewport.Height();
    float zoom = viewport.Zoom();

    float desiredWorldSpacing = TARGET_SCREEN_SPACING / zoom;
    float minorSpacing = ChooseSpacing(desiredWorldSpacing);
    float majorSpacing = minorSpacing * 5.0f;

    Point2 topLeft = viewport.ScreenToWorld({ 0.0f, 0.0f });
    Point2 bottomRight = viewport.ScreenToWorld({ w, h });

    float worldLeft = topLeft.x;
    float worldRight = bottomRight.x;
    float worldTop = topLeft.y;
    float worldBottom = bottomRight.y;

    // Minor grid lines
    float startX = std::floor(worldLeft / minorSpacing) * minorSpacing;
    for (float x = startX; x <= worldRight; x += minorSpacing)
    {
        Point2 s1 = viewport.WorldToScreen({ x, worldTop });
        Point2 s2 = viewport.WorldToScreen({ x, worldBottom });
        DrawLine(static_cast<int>(s1.x), static_cast<int>(s1.y),
                 static_cast<int>(s2.x), static_cast<int>(s2.y), MINOR_LINE);
    }

    float startY = std::floor(worldBottom / minorSpacing) * minorSpacing;
    for (float y = startY; y <= worldTop; y += minorSpacing)
    {
        Point2 s1 = viewport.WorldToScreen({ worldLeft, y });
        Point2 s2 = viewport.WorldToScreen({ worldRight, y });
        DrawLine(static_cast<int>(s1.x), static_cast<int>(s1.y),
                 static_cast<int>(s2.x), static_cast<int>(s2.y), MINOR_LINE);
    }

    // Major grid lines
    float startMX = std::floor(worldLeft / majorSpacing) * majorSpacing;
    for (float x = startMX; x <= worldRight; x += majorSpacing)
    {
        Point2 s1 = viewport.WorldToScreen({ x, worldTop });
        Point2 s2 = viewport.WorldToScreen({ x, worldBottom });
        DrawLine(static_cast<int>(s1.x), static_cast<int>(s1.y),
                 static_cast<int>(s2.x), static_cast<int>(s2.y), MAJOR_LINE);
    }

    float startMY = std::floor(worldBottom / majorSpacing) * majorSpacing;
    for (float y = startMY; y <= worldTop; y += majorSpacing)
    {
        Point2 s1 = viewport.WorldToScreen({ worldLeft, y });
        Point2 s2 = viewport.WorldToScreen({ worldRight, y });
        DrawLine(static_cast<int>(s1.x), static_cast<int>(s1.y),
                 static_cast<int>(s2.x), static_cast<int>(s2.y), MAJOR_LINE);
    }

    // Origin axes
    Point2 originScreen = viewport.WorldToScreen({ 0.0f, 0.0f });

    DrawLine(static_cast<int>(originScreen.x), 0,
             static_cast<int>(originScreen.x), static_cast<int>(h), AXIS_COLOR);
    DrawLine(0, static_cast<int>(originScreen.y),
             static_cast<int>(w), static_cast<int>(originScreen.y), AXIS_COLOR);

    // Sandbox border
    RenderBorder(viewport);
}

void GridRenderer::RenderBorder(const Viewport& viewport)
{
    Rect bounds = viewport.GetWorldBounds();

    Point2 tl = viewport.WorldToScreen({ bounds.x, bounds.y + bounds.height });
    Point2 tr = viewport.WorldToScreen({ bounds.x + bounds.width, bounds.y + bounds.height });
    Point2 br = viewport.WorldToScreen({ bounds.x + bounds.width, bounds.y });
    Point2 bl = viewport.WorldToScreen({ bounds.x, bounds.y });

    DrawLine(static_cast<int>(tl.x), static_cast<int>(tl.y),
             static_cast<int>(tr.x), static_cast<int>(tr.y), BORDER_COLOR);
    DrawLine(static_cast<int>(tr.x), static_cast<int>(tr.y),
             static_cast<int>(br.x), static_cast<int>(br.y), BORDER_COLOR);
    DrawLine(static_cast<int>(br.x), static_cast<int>(br.y),
             static_cast<int>(bl.x), static_cast<int>(bl.y), BORDER_COLOR);
    DrawLine(static_cast<int>(bl.x), static_cast<int>(bl.y),
             static_cast<int>(tl.x), static_cast<int>(tl.y), BORDER_COLOR);
}
