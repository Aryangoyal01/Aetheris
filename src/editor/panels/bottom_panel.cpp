#include "bottom_panel.h"
#include "../theme/colors.h"
#include "../editor_state.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include "../theme/widgets.h"
#include "../theme/theme.h"

BottomPanel::BottomPanel(EditorState* state) : m_state(state) {}

void BottomPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Theme::Metrics::Spacing20, Theme::Metrics::Spacing8));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Theme::Metrics::Spacing16, Theme::Metrics::Spacing10));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 180), ImVec2(-1, 400));
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    // Tab bar with default font (icons already merged)
    if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);
    if (ImGui::BeginTabBar("BottomDockTabs", ImGuiTabBarFlags_None))
    {
        if (m_state->fontDefault) ImGui::PopFont();

        if (ImGui::BeginTabItem(" Console "))
        {
            ImGui::Spacing();

            // Clear button on the right
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - 96);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Theme::Metrics::Spacing16, Theme::Metrics::Spacing6));
            ImGui::PushStyleColor(ImGuiCol_Button, Theme::Colors::Surface);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Theme::Colors::SurfaceHover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, Theme::Colors::SurfaceActive);
            if (ImGui::Button("Clear", ImVec2(96, 32))) {}
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - Theme::Metrics::Spacing4);

            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);

            Theme::Widgets::ConsoleEntry("12:45:10", "SYSTEM", Theme::Colors::System, "Aetheris Engine Initialized");
            Theme::Widgets::ConsoleEntry("12:45:10", "INFO", Theme::Colors::Info, "GPU: NVIDIA GeForce RTX 4050");
            Theme::Widgets::ConsoleEntry("12:45:10", "INFO", Theme::Colors::Info, "Compute Shaders: Supported");
            Theme::Widgets::ConsoleEntry("12:45:10", "WARNING", Theme::Colors::Warning, "Max Particles clamped to 2,000,000");
            Theme::Widgets::ConsoleEntry("12:45:10", "ERROR", Theme::Colors::Error, "Failed to load environment map: not found");
            Theme::Widgets::ConsoleEntry("12:45:10", "SYSTEM", Theme::Colors::System, "Simulation Ready");

            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(" Logs "))
        {
            ImGui::Spacing();
            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);
            Theme::Widgets::ConsoleEntry("12:45:10", "SYSTEM", Theme::Colors::System, "System starting up.");
            Theme::Widgets::ConsoleEntry("12:45:11", "SYSTEM", Theme::Colors::System, "All modules loaded.");
            Theme::Widgets::ConsoleEntry("12:45:12", "SYSTEM", Theme::Colors::System, "Editor initialized.");
            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(" Profiler "))
        {
            ImGui::Spacing();
            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);
            Theme::Widgets::ConsoleEntry("12:45:10", "SYSTEM", Theme::Colors::System, "Waiting for profiler data...");
            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(" Network "))
        {
            ImGui::Spacing();
            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);
            Theme::Widgets::ConsoleEntry("12:45:10", "INFO", Theme::Colors::Info, "Network disconnected. Standalone mode active.");
            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    else {
        if (m_state->fontDefault) ImGui::PopFont();
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}
