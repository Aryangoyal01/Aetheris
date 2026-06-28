#pragma once

#include "i_panel.h"
#include "core_math/geometry.h"
#include <raylib.h>

class Renderer;
class Viewport;

class ViewportPanel : public IPanel
{
public:
    ViewportPanel(Renderer* renderer, Viewport* viewport);
    
    void Render() override;
    const std::string& GetName() const override { return m_name; }

    bool IsHovered() const { return m_isHovered; }
    
    // Converts a global screen coordinate to local viewport panel coordinate
    Point2 GetLocalMousePos() const;

private:
    std::string m_name = "Viewport";
    Renderer* m_renderer;
    Viewport* m_viewport;

    bool m_isHovered = false;
    Point2 m_screenPos = {0.0f, 0.0f};
};
