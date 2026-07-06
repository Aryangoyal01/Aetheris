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
    // 35px tall menu bar (15px font + 10px * 2 padding)
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Theme::Metrics::Spacing16, 10.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Theme::Metrics::Spacing8, 0));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Theme::Colors::BackgroundDark);

    if (ImGui::BeginMainMenuBar())
    {
        float menuBarH = 35.0f; 
        float w = ImGui::GetWindowWidth();
        
        // ----------------------------------------------------
        // LEFT: Branding (Logo + Name)
        // ----------------------------------------------------
        if (m_state->fontTitle) ImGui::PushFont(m_state->fontTitle);
        float titleFontSize = ImGui::GetFontSize();
        ImGui::SetCursorPosY((menuBarH - titleFontSize) * 0.5f);
        ImGui::TextColored(Theme::Colors::AccentPurpleBright, " " ICON_FA_DIAMOND " ");
        
        ImGui::SameLine(0, 0);
        ImGui::SetCursorPosY((menuBarH - titleFontSize) * 0.5f);
        ImGui::TextColored(Theme::Colors::Text, "AETHERIS");
        if (m_state->fontTitle) ImGui::PopFont();

        ImGui::SameLine(0, 16.0f);
        
        // Version / subtitle
        if (m_state->fontCaption) ImGui::PushFont(m_state->fontCaption);
        float captionFontSize = ImGui::GetFontSize();
        ImGui::SetCursorPosY((menuBarH - captionFontSize) * 0.5f);
        ImGui::TextColored(Theme::Colors::TextDim, "Scientific Editor");
        
        ImGui::SameLine(0, 6.0f);
        ImGui::SetCursorPosY((menuBarH - captionFontSize) * 0.5f);
        ImGui::TextColored(Theme::Colors::AccentPurpleDim, "v4.4.0");
        if (m_state->fontCaption) ImGui::PopFont();

        // ----------------------------------------------------
        // CENTER: Playback Controls
        // ----------------------------------------------------
        ImVec2 btnSize(100, 26);
        float totalPlayControlsW = btnSize.x * 3.0f + 8.0f * 2.0f;
        float centerOffset = (w - totalPlayControlsW) * 0.5f;
        
        ImGui::SameLine(centerOffset);

        bool isEdit = m_state->settings && m_state->settings->state == SimulationState::Edit;
        bool isPlay = m_state->settings && m_state->settings->state == SimulationState::Play;
        bool isPause = m_state->settings && m_state->settings->state == SimulationState::Pause;

        float btnY = (menuBarH - btnSize.y) * 0.5f;

        ImGui::SetCursorPosY(btnY);
        if (Theme::Widgets::ToolbarButton("edit_btn", ICON_FA_PEN " Edit", isEdit, btnSize) && m_state->settings) {
            m_state->settings->state = SimulationState::Edit;
        }
        
        ImGui::SameLine(0, 4.0f);
        ImGui::SetCursorPosY(btnY);
        if (isPlay) {
            if (Theme::Widgets::ToolbarButton("play_pause_btn", ICON_FA_PAUSE " Pause", true, btnSize) && m_state->settings) {
                m_state->settings->state = SimulationState::Pause;
            }
        } else {
            if (Theme::Widgets::ToolbarButton("play_pause_btn", ICON_FA_PLAY " Play", false, btnSize) && m_state->settings) {
                m_state->settings->state = SimulationState::Play;
            }
        }

        ImGui::SameLine(0, 4.0f);
        ImGui::SetCursorPosY(btnY);
        if (Theme::Widgets::ToolbarButton("stop_btn", ICON_FA_STOP " Stop", false, btnSize) && m_state->settings) {
            m_state->settings->state = SimulationState::Edit;
            m_state->clearParticlesRequested = true;
        }

        // ----------------------------------------------------
        // RIGHT: Status chips & toggles
        // ----------------------------------------------------
        float rightSideW = 460.0f; 
        float currentX = ImGui::GetCursorPosX();
        if (w - rightSideW > currentX + 20.0f) { 
            ImGui::SameLine(w - rightSideW);
        } else {
            ImGui::SameLine(0, 20.0f);
        }

        float chipY = (menuBarH - 26.0f) * 0.5f; 
        
        auto RenderChip = [&](const char* label, const char* value, ImVec4 color) {
            ImGui::SetCursorPosY(chipY); 
            Theme::Widgets::StatusChip(label, value, color);
            ImGui::SameLine(0, 10.0f);
        };

        char fpsBuf[32];
        snprintf(fpsBuf, sizeof(fpsBuf), "%d", m_state->fps);
        RenderChip("FPS", fpsBuf, Theme::Colors::Success);
        
        RenderChip("GPU", "ACTIVE", Theme::Colors::AccentCyan);
        RenderChip("SIM", "ACTIVE", Theme::Colors::TextDim);
        
        char partsBuf[32];
        snprintf(partsBuf, sizeof(partsBuf), "%u", m_state->particleCount);
        RenderChip("PARTS", partsBuf, Theme::Colors::Warning);
        
        if (m_state->settings) {
            ImGui::SetCursorPosY(chipY);
            if (m_state->settings->useComputeShader) {
                if (Theme::Widgets::ToolbarButton("gpu_btn", ICON_FA_MICROCHIP " GPU", false, ImVec2(70, 26))) {
                    m_state->settings->useComputeShader = false;
                }
            } else {
                if (Theme::Widgets::ToolbarButton("cpu_btn", ICON_FA_MICROCHIP " CPU", false, ImVec2(70, 26))) {
                    m_state->settings->useComputeShader = true;
                }
            }
            ImGui::SameLine(0, 16.0f);
        }

        ImGui::SetCursorPosY(chipY);
        ImVec2 gearSize(26, 26);
        Theme::Widgets::ToolbarButton("settings_btn", ICON_FA_GEAR, false, gearSize);

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}
