#include "viewport_panel.h"
#include "rendering/renderer.h"
#include "viewport/viewport.h"
#include <imgui.h>
#include <rlImGui.h>
#include <cmath>

ViewportPanel::ViewportPanel(Renderer* renderer, Viewport* viewport)
    : m_renderer(renderer), m_viewport(viewport)
{
}

void ViewportPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    ImVec2 contentAvail = ImGui::GetContentRegionAvail();
    ImVec2 viewportPanelSize = ImVec2(std::floor(contentAvail.x), std::floor(contentAvail.y));
    m_isHovered = ImGui::IsWindowHovered();
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    m_screenPos = { cursorScreenPos.x, cursorScreenPos.y };
    
    // Resize target if panel size changes
    if (m_renderer)
    {
        m_renderer->ResizeTarget(static_cast<int>(viewportPanelSize.x), static_cast<int>(viewportPanelSize.y));
        if (m_viewport)
        {
            m_viewport->Resize(viewportPanelSize.x, viewportPanelSize.y);
        }

        // Draw the render texture
        const RenderTexture2D& target = m_renderer->GetTarget();
        if (target.id != 0)
        {
            // UVs map (0,1) -> (1,0) to properly match OpenGL Y-up texture coordinates
            ImGui::Image((ImTextureID)(intptr_t)target.texture.id, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        }
    }

    if (m_isHovered)
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
        } else {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

Point2 ViewportPanel::GetLocalMousePos() const
{
    ImVec2 mousePos = ImGui::GetMousePos();
    return { mousePos.x - m_screenPos.x, mousePos.y - m_screenPos.y };
}
