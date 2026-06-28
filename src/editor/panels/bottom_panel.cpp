#include "bottom_panel.h"
#include <imgui.h>

void BottomPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 150), ImVec2(-1, 400));
    ImGui::Begin(m_name.c_str(), nullptr, flags);
    
    if (ImGui::BeginTabBar("BottomDockTabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem(" Console "))
        {
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.0f, 0.80f, 0.85f, 1.0f), "[SYS]");
            ImGui::SameLine();
            ImGui::Text("Aetheris Console initialized...");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(" Logs "))
        {
            ImGui::Spacing();
            ImGui::Text("[INFO] System running...");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(" Profiler "))
        {
            ImGui::Spacing();
            ImGui::Text("Profiler data...");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem(" Network "))
        {
            ImGui::Spacing();
            ImGui::Text("Network disconnected.");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
    ImGui::End();
    ImGui::PopStyleVar();
}