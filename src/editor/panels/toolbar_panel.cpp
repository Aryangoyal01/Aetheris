#include "toolbar_panel.h"
#include "../theme/theme.h"
#include "../theme/colors.h"
#include "../editor_state.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include <raylib.h>
#include "../theme/widgets.h"

ToolbarPanel::ToolbarPanel(EditorState* state) : m_state(state) {}

void ToolbarPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Theme::Metrics::Spacing16, Theme::Metrics::Spacing12));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Theme::Metrics::Spacing4, 0));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Theme::Colors::BackgroundDark);

    if (ImGui::BeginMainMenuBar())
    {
        // Left: Diamond icon + Branding
        if (m_state->fontTitle) ImGui::PushFont(m_state->fontTitle);
        ImGui::TextColored(Theme::Colors::AccentPurple, "  " ICON_FA_DIAMOND "  ");
        ImGui::SameLine();
        ImGui::TextColored(Theme::Colors::AccentCyan, "AETHERIS");
        if (m_state->fontTitle) ImGui::PopFont();

        ImGui::SameLine();
        ImGui::TextColored(Theme::Colors::TextMuted, "   |   ");

        ImGui::SameLine();
        if (m_state->fontCaption) ImGui::PushFont(m_state->fontCaption);
        ImGui::TextColored(Theme::Colors::TextDim, "Scientific Editor  ");
        ImGui::SameLine();
        ImGui::TextColored(Theme::Colors::AccentPurpleDim, "v4.4.0");
        if (m_state->fontCaption) ImGui::PopFont();

        // Center: Segmented Playback Controls
        float centerOffset = ImGui::GetWindowWidth() / 2.0f - 260.0f;
        ImGui::SameLine(centerOffset);

        ImVec2 btnSize(130, 40);

        Theme::Widgets::ToolbarButton("edit_btn", ICON_FA_PEN "  Edit", false, btnSize);
        ImGui::SameLine(0, 2);
        Theme::Widgets::ToolbarButton("play_btn", ICON_FA_PLAY "  Play", false, btnSize);
        ImGui::SameLine(0, 2);
        Theme::Widgets::ToolbarButton("pause_btn", ICON_FA_PAUSE "  Pause", false, btnSize);
        ImGui::SameLine(0, 2);
        Theme::Widgets::ToolbarButton("stop_btn", ICON_FA_STOP "  Stop", false, btnSize);

        // Right: Status chips — use tighter SameLine spacing
        ImGui::SameLine(ImGui::GetWindowWidth() - 700.0f);

        auto RenderChip = [](const char* label, const char* value, ImVec4 color) {
            Theme::Widgets::StatusChip(label, value, color);
            ImGui::SameLine(0, 8);
        };

        RenderChip("FPS", std::to_string(GetFPS()).c_str(), Theme::Colors::Success);
        RenderChip("GPU", "ACTIVE", Theme::Colors::AccentCyan);
        RenderChip("SIM", "IDLE", Theme::Colors::TextDim);
        RenderChip("MEM", "1.2 GB", Theme::Colors::Warning);

        // Settings gear on far right
        ImGui::SameLine(ImGui::GetWindowWidth() - 52.0f);
        ImVec2 gearSize(34, 34);
        Theme::Widgets::ToolbarButton("settings_btn", ICON_FA_GEAR, false, gearSize);

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}
