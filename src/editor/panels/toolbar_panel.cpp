#include "toolbar_panel.h"
#include <imgui.h>
#include <raylib.h>
#include <string>

void ToolbarPanel::Render()
{
    // Make the menu bar slightly taller with vertical padding
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 0));
    
    if (ImGui::BeginMainMenuBar())
    {
        // Branding
        ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "  A E T H E R I S  ");
        
        // Centered playback controls
        float centerOffset = ImGui::GetWindowWidth() / 2.0f - 100.0f;
        ImGui::SameLine(centerOffset);
        
        if (ImGui::Button("Play", ImVec2(60, 0))) {}
        if (ImGui::Button("Pause", ImVec2(60, 0))) {}
        if (ImGui::Button("Stop", ImVec2(60, 0))) {}
        
        // Right-aligned status indicators
        float rightOffset = 280.0f;
        ImGui::SameLine(ImGui::GetWindowWidth() - rightOffset);
        
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.55f, 1.0f), "FPS: %d", GetFPS());
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.55f, 1.0f), "| GPU: Active");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.55f, 1.0f), "| AI: Idle");
        
        ImGui::EndMainMenuBar();
    }
    
    ImGui::PopStyleVar(2);
}