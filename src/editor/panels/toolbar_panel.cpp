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
    // Push WindowPadding to 0 so the clipping region perfectly matches the visual bar height
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // Menu bar height will be 72px (26px + 23px * 2)
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(Theme::Metrics::Spacing16, 23.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Theme::Metrics::Spacing8, 0));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Theme::Colors::BackgroundDark);

    if (ImGui::BeginMainMenuBar())
    {
        float menuBarH = ImGui::GetWindowHeight(); 
        float w = ImGui::GetWindowWidth();
        float winY = ImGui::GetWindowPos().y;
        
        // ----------------------------------------------------
        // LEFT: Branding (Logo + Name)
        // ----------------------------------------------------
        
        // 1. Custom Vector Symbol: Geometric Energy Core
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        float iconRadius = 18.0f; // Scale up to match 56px font
        // True absolute center (72px / 2)
        float iconCenterY = winY + 36.0f; 
        ImVec2 center(ImGui::GetCursorScreenPos().x + 10.0f + iconRadius, iconCenterY);
        
        ImU32 colAccent = ImGui::GetColorU32(Theme::Colors::AccentPurpleBright);
        ImU32 colDim = ImGui::GetColorU32(Theme::Colors::AccentPurple);
        
        // Outer ring (thicker stroke)
        drawList->AddCircle(center, iconRadius, colDim, 0, 2.0f); 
        // 3 interconnected nodes
        ImVec2 p1(center.x, center.y - iconRadius);
        ImVec2 p2(center.x + iconRadius * 0.866f, center.y + iconRadius * 0.5f);
        ImVec2 p3(center.x - iconRadius * 0.866f, center.y + iconRadius * 0.5f);
        drawList->AddLine(p1, p2, colDim, 2.0f);
        drawList->AddLine(p2, p3, colDim, 2.0f);
        drawList->AddLine(p3, p1, colDim, 2.0f);
        drawList->AddCircleFilled(p1, 3.0f, colAccent); // slightly larger dots
        drawList->AddCircleFilled(p2, 3.0f, colAccent);
        drawList->AddCircleFilled(p3, 3.0f, colAccent);
        // Central energy core
        drawList->AddCircleFilled(center, 2.5f, ImGui::GetColorU32(ImVec4(1, 1, 1, 1)));
        
        // Advance cursor past the icon
        ImGui::SetCursorScreenPos(ImVec2(center.x + iconRadius + 20.0f, ImGui::GetCursorScreenPos().y));
        
        // 2. Typography: Custom letter spacing and emphasis
        if (m_state->fontTitle) ImGui::PushFont(m_state->fontTitle); // 56px font
        float titleFontSize = ImGui::GetFontSize();
        
        const char* title = "AETHERIS";
        // Adjust the text upward by 14 pixels to compensate for font baseline/descent
        float textY = winY - 14.0f;        
        for (int i = 0; title[i] != '\0'; ++i) {
            char buf[2] = { title[i], '\0' };
            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, textY));
            if (i == 0) {
                // Emphasize first letter
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", buf);
            } else {
                ImGui::TextColored(Theme::Colors::Text, "%s", buf);
            }
            if (title[i+1] != '\0') {
                ImGui::SameLine(0, 4.0f); // Increased letter spacing
            }
        }
        if (m_state->fontTitle) ImGui::PopFont();

        ImGui::SameLine(0, 16.0f);
        
        // 3. Subtitle / Metadata (Reduced visual weight)
        if (m_state->fontCaption) ImGui::PushFont(m_state->fontCaption);
        // Push caption upward
        float captionY = winY + 16.0f;
        
        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, captionY));
        ImGui::TextColored(Theme::Colors::TextMuted, "PHYSICS SIMULATION");
        
        ImGui::SameLine(0, 8.0f);
        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, captionY));
        ImGui::TextColored(Theme::Colors::AccentCyan, "v4.4.0");
        
        ImGui::SameLine(0, 8.0f);
        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, captionY));
#ifdef NDEBUG
        ImGui::TextColored(Theme::Colors::AccentPurpleBright, "CORE");
#else
        ImGui::TextColored(Theme::Colors::Orange, "CORE DEV");
#endif
        if (m_state->fontCaption) ImGui::PopFont();

        // ----------------------------------------------------
        // CENTER: Playback Controls
        // ----------------------------------------------------
        ImVec2 btnSize(100, 24); // Give buttons enough width to avoid text overlap
        float spacing = 8.0f; 
        float actualControlsW = btnSize.x * 3.0f + spacing * 2.0f;
        
        // Mathematically center it exactly against the total width of the toolbar
        float targetX = w * 0.5f - actualControlsW * 0.5f;
        
        // Prevent overlap with branding
        float controlsStartX = ImGui::GetCursorScreenPos().x - ImGui::GetWindowPos().x;
        if (targetX < controlsStartX + 20.0f) {
            targetX = controlsStartX + 20.0f;
        }
        
        // True absolute center for 24px tall buttons: (72 - 24) / 2 = 24
        float btnY = winY + 24.0f; 
        
        ImGui::SameLine();

        bool isEdit = m_state->settings && m_state->settings->state == SimulationState::Edit;
        bool isPlay = m_state->settings && m_state->settings->state == SimulationState::Play;
        bool isPause = m_state->settings && m_state->settings->state == SimulationState::Pause;

        // Use the default font for buttons instead of header font to fix text wrapping issues.
        // We just don't push a font here.

        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetWindowPos().x + targetX, btnY));
        if (Theme::Widgets::ToolbarButton("edit_btn", ICON_FA_PEN " Edit", isEdit, btnSize) && m_state->settings) {
            m_state->settings->state = SimulationState::Edit;
        }
        
        ImGui::SameLine(0, spacing);
        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, btnY));
        if (isPlay) {
            if (Theme::Widgets::ToolbarButton("play_pause_btn", ICON_FA_PAUSE " Pause", true, btnSize) && m_state->settings) {
                m_state->settings->state = SimulationState::Pause;
            }
        } else {
            if (Theme::Widgets::ToolbarButton("play_pause_btn", ICON_FA_PLAY " Play", false, btnSize) && m_state->settings) {
                m_state->settings->state = SimulationState::Play;
            }
        }

        ImGui::SameLine(0, spacing);
        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, btnY));
        if (Theme::Widgets::ToolbarButton("stop_btn", ICON_FA_STOP " Stop", false, btnSize) && m_state->settings) {
            m_state->settings->state = SimulationState::Edit;
            m_state->clearParticlesRequested = true;
        }

        // ----------------------------------------------------
        // RIGHT: Status chips & toggles
        // ----------------------------------------------------
        float rightSideW = 700.0f; 
        float currentX = ImGui::GetCursorScreenPos().x - ImGui::GetWindowPos().x;
        if (w - rightSideW > currentX + 20.0f) { 
            ImGui::SameLine(w - rightSideW);
        } else {
            ImGui::SameLine(0, 20.0f);
        }

        // Chips are slightly taller (~28px), so absolute center: (72 - 28) / 2 = 22
        float chipY = winY + 22.0f; 
        
        // Toggles are 24px tall, center: (72 - 24) / 2 = 24
        float toggleY = winY + 24.0f;
        
        auto RenderChip = [&](const char* label, const char* value, ImVec4 color) {
            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, chipY)); 
            Theme::Widgets::StatusChip(label, value, color);
            ImGui::SameLine(0, 10.0f);
        };

        char fpsBuf[32];
        snprintf(fpsBuf, sizeof(fpsBuf), "%d", m_state->fps);
        RenderChip("FPS", fpsBuf, Theme::Colors::Success);
        
        RenderChip("GPU", "ACTIVE", Theme::Colors::AccentCyan);
        RenderChip("SIM", "ACTIVE", Theme::Colors::AccentPurpleBright);
        
        char partsBuf[32];
        snprintf(partsBuf, sizeof(partsBuf), "%u", m_state->particleCount);
        RenderChip("PARTS", partsBuf, Theme::Colors::Warning);
        
        if (m_state->settings) {
            ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, toggleY));
            if (m_state->settings->useComputeShader) {
                if (Theme::Widgets::ToolbarButton("gpu_btn", ICON_FA_MICROCHIP " GPU", false, ImVec2(70, 24))) {
                    m_state->settings->useComputeShader = false;
                }
            } else {
                if (Theme::Widgets::ToolbarButton("cpu_btn", ICON_FA_MICROCHIP " CPU", false, ImVec2(70, 24))) {
                    m_state->settings->useComputeShader = true;
                }
            }
            ImGui::SameLine(0, 16.0f);
        }

        ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, chipY));
        ImVec2 gearSize(24, 24);
        Theme::Widgets::ToolbarButton("settings_btn", ICON_FA_GEAR, false, gearSize);

        ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3); // Popped WindowPadding, FramePadding, ItemSpacing
}
