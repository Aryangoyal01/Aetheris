#include "left_sidebar.h"
#include "../editor_state.h"
#include "../theme/colors.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include "../theme/widgets.h"
#include "../theme/theme.h"

LeftSidebar::LeftSidebar(EditorState* state) : m_state(state) {}

void LeftSidebar::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, Theme::Metrics::Spacing12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, Theme::Metrics::Spacing4));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::SetNextWindowSizeConstraints(ImVec2(Theme::Metrics::NavWidth, -1), ImVec2(Theme::Metrics::NavWidth, -1));
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    // Top spacing
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));

    auto RenderNavButton = [&](const char* icon, const char* label, InspectorMode mode) {
        bool isActive = (m_state->activeMode == mode);
        if (Theme::Widgets::SidebarItem(label, icon, label, isActive, m_state->fontHeader)) {
            m_state->activeMode = mode;
        }
    };

    RenderNavButton(ICON_FA_CUBE, "Physics", InspectorMode::Physics);
    RenderNavButton(ICON_FA_ATOM, "Particles", InspectorMode::Particles);
    RenderNavButton(ICON_FA_CLOUD, "Environment", InspectorMode::Environment);
    RenderNavButton(ICON_FA_LAYER_GROUP, "Scene", InspectorMode::Scene);
    RenderNavButton(ICON_FA_CAMERA, "Camera", InspectorMode::Camera);
    RenderNavButton(ICON_FA_FOLDER_OPEN, "Assets", InspectorMode::Assets);
    RenderNavButton(ICON_FA_TERMINAL, "Console", InspectorMode::Console);
    RenderNavButton(ICON_FA_GEAR, "Settings", InspectorMode::Settings);

    ImGui::End();
    ImGui::PopStyleVar(2);
}
