#include "right_sidebar.h"
#include "../editor_state.h"
#include <imgui.h>

RightSidebar::RightSidebar(EditorState* state) : m_state(state) {}

void RightSidebar::Render()
{
    // Provide padding and breathing room for inspector elements
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 12));
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(250, -1), ImVec2(500, -1));
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    switch (m_state->activeMode)
    {
    case InspectorMode::Scene:
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "SCENE");
        ImGui::Separator();
        ImGui::Text("Global scene parameters go here.");
        break;
    case InspectorMode::Physics:
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "PHYSICS");
        ImGui::Separator();
        ImGui::Text("Gravity: -9.81");
        ImGui::Text("Restitution: 0.8");
        break;
    case InspectorMode::Rendering:
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "RENDERING");
        ImGui::Separator();
        ImGui::Text("Color Maps, Opacity, Shaders.");
        break;
    case InspectorMode::Simulation:
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "SIMULATION");
        ImGui::Separator();
        ImGui::Text("Timestep, Solver Iterations.");
        break;
    case InspectorMode::Export:
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "EXPORT");
        ImGui::Separator();
        if (ImGui::Button("Export to Alembic", ImVec2(ImGui::GetContentRegionAvail().x, 30))) {}
        break;
    }

    ImGui::Spacing();
    ImGui::Spacing();
    
    // Quick Actions are always available at the bottom
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100);
    ImGui::TextColored(ImVec4(0.60f, 0.20f, 0.80f, 1.0f), "QUICK ACTIONS");
    ImGui::Separator();
    if (ImGui::Button("Reset View", ImVec2(ImGui::GetContentRegionAvail().x, 30))) {}
    if (ImGui::Button("Clear Particles", ImVec2(ImGui::GetContentRegionAvail().x, 30))) {}

    ImGui::End();
    ImGui::PopStyleVar(2);
}