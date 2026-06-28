#include "viewport_panel.h"
#include "rendering/renderer.h"
#include "viewport/viewport.h"
#include "../theme/colors.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include "../theme/widgets.h"
#include "../theme/theme.h"
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
        
        // Floating Viewport Toolbar
        ImGui::SetCursorScreenPos(ImVec2(cursorScreenPos.x + Theme::Metrics::Spacing16, cursorScreenPos.y + Theme::Metrics::Spacing16));

        // Shadow layer
        ImVec2 toolbarSize(420, 56);
        ImVec2 shadowPos = ImVec2(cursorScreenPos.x + Theme::Metrics::Spacing16 + 2, cursorScreenPos.y + Theme::Metrics::Spacing16 + 3);
        ImDrawList* windowDl = ImGui::GetWindowDrawList();
        windowDl->AddRectFilled(shadowPos, ImVec2(shadowPos.x + toolbarSize.x, shadowPos.y + toolbarSize.y),
            IM_COL32(0, 0, 0, 60), Theme::Radii::Floating);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.09f, 0.92f));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, Theme::Radii::Floating);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Theme::Metrics::Spacing8, Theme::Metrics::Spacing6));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.18f, 0.18f, 0.20f, 0.8f));

        if (ImGui::BeginChild("ViewportToolbar", toolbarSize, true, ImGuiWindowFlags_NoScrollbar))
        {
            ImVec2 bSize(40, 40);
            Theme::Widgets::ToolbarButton("vp_sel", ICON_FA_ARROW_POINTER, true, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_pan", ICON_FA_HAND, false, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_add", ICON_FA_PLUS, false, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_rot", ICON_FA_ROTATE, false, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_del", ICON_FA_ERASER, false, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_grid", ICON_FA_TABLE_CELLS, false, bSize);
            ImGui::SameLine();
            Theme::Widgets::ToolbarButton("vp_cfg", ICON_FA_GEAR, false, bSize);
        }
        ImGui::EndChild();

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(3);
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
