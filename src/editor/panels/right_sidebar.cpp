#include "right_sidebar.h"
#include "../editor_state.h"
#include "../theme/colors.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include <implot.h>
#include "../theme/widgets.h"
#include "../theme/theme.h"

RightSidebar::RightSidebar(EditorState* state) : m_state(state) {}

void RightSidebar::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Theme::Metrics::Spacing20, Theme::Metrics::Spacing16));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Theme::Metrics::Spacing12, Theme::Metrics::Spacing8));

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, -1), ImVec2(520, -1));
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    switch (m_state->activeMode)
    {
    case InspectorMode::Physics:
    {
        Theme::Widgets::SectionHeader("PHYSICS", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("grav", "GRAVITY", true, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Gravity", "9.810", "m/s\u00B2");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::Vector2Property("Direction", "0.00", "-1.00");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("solver", "SOLVER", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyDropdown("Substeps", "4");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyDropdown("Solver Iterations", "8");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("bounds", "BOUNDS", false, m_state->fontHeader)) {
            static bool boundsEnabled = true;
            Theme::Widgets::ToggleSwitch("bounds_en", boundsEnabled);
            ImGui::SameLine();
            ImGui::TextColored(Theme::Colors::TextDim, "Enabled");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyDropdown("Boundary", "AABB");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("collision", "COLLISION", false, m_state->fontHeader)) {
            static bool collisionEnabled = true;
            Theme::Widgets::ToggleSwitch("coll_en", collisionEnabled);
            ImGui::SameLine();
            ImGui::TextColored(Theme::Colors::TextDim, "Enabled");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyValue("Restitution", "0.25");
        }
        if (m_state->fontHeader) ImGui::PopFont();
        break;
    }

    case InspectorMode::Particles:
    {
        Theme::Widgets::SectionHeader("PARTICLES", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("spawn", "SPAWN RATE", true, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Rate", "10,000", "/ sec");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("lifetime", "LIFETIME", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Min", "1.0", "s");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyValue("Max", "5.0", "s");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("psize", "SIZE", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Radius", "0.05");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("pvel", "VELOCITY", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Speed", "2.5", "m/s");
        }
        if (m_state->fontHeader) ImGui::PopFont();
        break;
    }

    case InspectorMode::Environment:
    {
        Theme::Widgets::SectionHeader("ENVIRONMENT", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("light", "LIGHTING", true, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Sun Intensity", "1.2");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));
        if (Theme::Widgets::CollapsibleSection("fog", "FOG", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Density", "0.02");
        }
        if (m_state->fontHeader) ImGui::PopFont();
        break;
    }

    case InspectorMode::Scene:
    {
        Theme::Widgets::SectionHeader("PERFORMANCE", m_state->fontHeader);

        // Metric cards in a 2x2 grid
        float cardW = (ImGui::GetContentRegionAvail().x - Theme::Metrics::Spacing8) / 2.0f;

        Theme::Widgets::MetricCard("FPS", "60", nullptr, Theme::Colors::Success, cardW);
        ImGui::SameLine();
        Theme::Widgets::MetricCard("Frame Time", "16.6", "ms", Theme::Colors::AccentCyan, cardW);

        Theme::Widgets::MetricCard("GPU Time", "5.2", "ms", Theme::Colors::AccentPurple, cardW);
        ImGui::SameLine();
        Theme::Widgets::MetricCard("Particles", "1.02", "M", Theme::Colors::Success, cardW);

        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        Theme::Widgets::MetricCard("Memory", "1.2", "GB", Theme::Colors::Warning, cardW);

        break;
    }

    case InspectorMode::Camera:
    {
        Theme::Widgets::SectionHeader("CAMERA", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("cam_transform", "TRANSFORM", true, m_state->fontHeader)) {
            Theme::Widgets::Vector3Property("Position", "0", "10", "-20");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));
        if (Theme::Widgets::CollapsibleSection("cam_lens", "LENS", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("FOV", "60", "\u00B0");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyValue("Near", "0.1");
            ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
            Theme::Widgets::PropertyValue("Far", "1000");
        }
        if (m_state->fontHeader) ImGui::PopFont();
        break;
    }

    case InspectorMode::Assets:
        Theme::Widgets::SectionHeader("ASSETS", m_state->fontHeader);
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
        ImGui::TextColored(Theme::Colors::TextDim, "Asset library placeholder.");
        break;

    case InspectorMode::Console:
        Theme::Widgets::SectionHeader("CONSOLE SETTINGS", m_state->fontHeader);
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
        Theme::Widgets::PropertyValue("Log Level", "Info");
        break;

    case InspectorMode::Settings:
    {
        Theme::Widgets::SectionHeader("SETTINGS", m_state->fontHeader);
        if (Theme::Widgets::CollapsibleSection("editor_set", "EDITOR", true, m_state->fontHeader)) {
            Theme::Widgets::PropertyDropdown("Theme", "Aetheris Dark");
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));
        if (Theme::Widgets::CollapsibleSection("project_set", "PROJECT", false, m_state->fontHeader)) {
            Theme::Widgets::PropertyValue("Name", "Untitled");
        }
        break;
    }
    }

    // Quick Actions at bottom
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 320);
    Theme::Widgets::SectionHeader("QUICK ACTIONS", m_state->fontHeader);

    ImVec2 actionBtnSize(ImGui::GetContentRegionAvail().x, 36);

    Theme::Widgets::QuickAction("qa_reset", ICON_FA_CAMERA_ROTATE, "Reset Camera", actionBtnSize);
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    Theme::Widgets::QuickAction("qa_clear", ICON_FA_TRASH, "Clear Particles", actionBtnSize);
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    Theme::Widgets::QuickAction("qa_screenshot", ICON_FA_IMAGE, "Screenshot", actionBtnSize);
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    Theme::Widgets::QuickAction("qa_export", ICON_FA_FILE_EXPORT, "Export Scene", actionBtnSize);

    ImGui::End();
    ImGui::PopStyleVar(2);
}
