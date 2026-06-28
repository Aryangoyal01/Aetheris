#include "left_sidebar.h"
#include "../editor_state.h"
#include <imgui.h>

LeftSidebar::LeftSidebar(EditorState* state) : m_state(state) {}

void LeftSidebar::Render()
{
    // Navigation column should have tight padding and no scrollbar
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 16));
    
    // Hide standard window elements for a clean icon-strip look
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(80, -1), ImVec2(80, -1));
    ImGui::Begin(m_name.c_str(), nullptr, flags);
    
    ImVec2 btnSize = ImVec2(ImGui::GetContentRegionAvail().x, 60);
    
    // Selectable navigation "icons/buttons"
    auto RenderNavButton = [&](const char* label, InspectorMode mode) {
        bool isActive = (m_state->activeMode == mode);
        if (isActive) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        }
        
        if (ImGui::Button(label, btnSize)) {
            m_state->activeMode = mode;
        }
        
        if (isActive) {
            ImGui::PopStyleColor();
        }
        ImGui::Spacing();
    };

    RenderNavButton("Scene", InspectorMode::Scene);
    RenderNavButton("Physx", InspectorMode::Physics);
    RenderNavButton("Render", InspectorMode::Rendering);
    RenderNavButton("Sim", InspectorMode::Simulation);
    RenderNavButton("Export", InspectorMode::Export);

    ImGui::End();
    ImGui::PopStyleVar();
}