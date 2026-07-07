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

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    ImGui::SetNextWindowSizeConstraints(ImVec2(300, -1), ImVec2(520, -1));
    ImGui::Begin(m_name.c_str(), nullptr, flags);

    bool isSim = m_state && m_state->settings && (m_state->settings->state == SimulationState::Play || m_state->settings->state == SimulationState::Pause);
    auto AddDisabledTooltip = [&]() {
        if (isSim && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Pause or switch to Edit mode to modify settings.");
        }
    };

    float quickActionsH = 305.0f;
    ImGui::BeginChild("InspectorContent", ImVec2(0, ImGui::GetContentRegionAvail().y - quickActionsH), false, ImGuiWindowFlags_None);

    switch (m_state->activeMode)
    {
    case InspectorMode::Physics:
    {
        Theme::Widgets::SectionHeader("PHYSICS", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("grav", "GRAVITY", true, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing8));
                Theme::Widgets::Vector2Control("Gravity", m_state->settings->gravity, 0.0f, isSim, "m/s²");
            }
        }
        ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));

        if (Theme::Widgets::CollapsibleSection("solver", "SOLVER", false, m_state->fontHeader)) {
            if (m_state->settings) {
                float inputW = ImGui::GetContentRegionAvail().x * 0.7f;
                
                ImGui::TextColored(Theme::Colors::TextDim, "Substeps");
                ImGui::Dummy(ImVec2(0, 4.0f));
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                ImGui::DragInt("##Substeps", &m_state->settings->substeps, 0.2f, 1, 16);
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                
                ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));
                
                ImGui::TextColored(Theme::Colors::TextDim, "Solver Iterations");
                ImGui::Dummy(ImVec2(0, 4.0f));
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                ImGui::DragInt("##SolverIterations", &m_state->settings->solverIterations, 0.2f, 1, 64);
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
            }
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
            if (m_state->settings) {
                float inputW = ImGui::GetContentRegionAvail().x * 0.7f;
                
                ImGui::TextColored(Theme::Colors::TextDim, "Restitution");
                ImGui::Dummy(ImVec2(0, 4.0f));
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                ImGui::DragFloat("##Restitution", &m_state->settings->restitution, 0.01f, 0.0f, 1.0f);
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                
                ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing12));
                
                ImGui::TextColored(Theme::Colors::TextDim, "Mode");
                ImGui::Dummy(ImVec2(0, 4.0f));
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                int mode = static_cast<int>(m_state->settings->collisionMode);
                const char* modes[] = { "None", "Bounce", "Wrap", "Destroy" };
                if (ImGui::Combo("##Mode", &mode, modes, 4)) {
                    m_state->settings->collisionMode = static_cast<CollisionMode>(mode);
                }
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
            }
        }
        if (m_state->fontHeader) ImGui::PopFont();
        break;
    }

    case InspectorMode::Particles:
    {
        Theme::Widgets::SectionHeader("PARTICLES", m_state->fontHeader);
        if (m_state->fontHeader) ImGui::PushFont(m_state->fontHeader);

        if (Theme::Widgets::CollapsibleSection("capacity", "CAPACITY", true, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, 10.0f));
                
                uint32_t capacity = m_state->settings->maxParticles;
                uint32_t current = m_state->particleCount;
                
                float fraction = capacity > 0 ? static_cast<float>(current) / capacity : 0.0f;
                std::string progressText = Theme::Widgets::FormatNumber(current) + " / " + Theme::Widgets::FormatNumber(capacity);
                
                ImVec4 progColor = Theme::Colors::Success;
                if (fraction >= 1.0f) progColor = Theme::Colors::Error;
                else if (fraction >= 0.9f) progColor = Theme::Colors::Orange;
                else if (fraction >= 0.7f) progColor = Theme::Colors::Warning;

                ImGui::TextColored(Theme::Colors::TextDim, "Current");
                ImGui::Dummy(ImVec2(0, 4.0f));
                
                m_smoothFraction += (fraction - m_smoothFraction) * (ImGui::GetIO().DeltaTime * 15.0f);
                
                ImVec2 pos = ImGui::GetCursorScreenPos();
                
                // Slightly wider than typical content available, centered
                float barW = ImGui::GetWindowWidth() * 0.90f;
                float availW = ImGui::GetContentRegionAvail().x;
                float offsetX = (availW - barW) * 0.5f;
                if (offsetX < 0) offsetX = 0;
                
                pos.x += offsetX;
                float barH = 50.0f;
                float radius = 4.5f;
                
                ImDrawList* dl = ImGui::GetWindowDrawList();
                
                // Background
                dl->AddRectFilled(pos, ImVec2(pos.x + barW, pos.y + barH), ImGui::GetColorU32(Theme::Colors::Surface), radius);
                
                // Fill
                float fillW = barW * m_smoothFraction;
                if (fillW > 0.1f) {
                    dl->AddRectFilled(pos, ImVec2(pos.x + fillW, pos.y + barH), ImGui::GetColorU32(progColor), radius);
                }
                
                ImVec2 textSize = ImGui::CalcTextSize(progressText.c_str());
                ImVec2 textPos = ImVec2(pos.x + (barW - textSize.x) * 0.5f, pos.y + (barH - textSize.y) * 0.5f);
                dl->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), progressText.c_str());
                
                ImGui::Dummy(ImVec2(0, barH));

                ImGui::Dummy(ImVec2(0, 12.0f));
                ImGui::TextColored(Theme::Colors::TextDim, "Max Particles");
                ImGui::Dummy(ImVec2(0, 4.0f));
                
                if (!m_isEditingMaxParticles) {
                    std::string formatted = Theme::Widgets::FormatNumber(m_state->settings->maxParticles);
                    strncpy(m_capacityInputBuffer, formatted.c_str(), sizeof(m_capacityInputBuffer));
                }
                
                ImGui::PushItemWidth(-1.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                bool enterPressed = ImGui::InputText("##MaxParticles", m_capacityInputBuffer, sizeof(m_capacityInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
                AddDisabledTooltip();
                
                if (ImGui::IsItemEdited()) {
                    m_isEditingMaxParticles = true;
                }

                if (enterPressed || ImGui::IsItemDeactivatedAfterEdit()) {
                    std::string raw = m_capacityInputBuffer;
                    raw.erase(std::remove(raw.begin(), raw.end(), ','), raw.end());
                    int64_t parsed = std::strtoll(raw.c_str(), nullptr, 10);
                    bool clamped = false;
                    
                    if (parsed > static_cast<int64_t>(SimulationSettings::MAX_PARTICLES_LIMIT)) {
                        parsed = static_cast<int64_t>(SimulationSettings::MAX_PARTICLES_LIMIT);
                        m_clampMessage = "Maximum allowed capacity is " + Theme::Widgets::FormatNumber(SimulationSettings::MAX_PARTICLES_LIMIT) + " particles. Value clamped.";
                        clamped = true;
                    } else if (parsed < static_cast<int64_t>(SimulationSettings::MIN_PARTICLES_LIMIT)) {
                        parsed = static_cast<int64_t>(SimulationSettings::MIN_PARTICLES_LIMIT);
                        m_clampMessage = "Minimum allowed capacity is " + Theme::Widgets::FormatNumber(SimulationSettings::MIN_PARTICLES_LIMIT) + " particles. Value clamped.";
                        clamped = true;
                    }
                    
                    m_state->settings->maxParticles = static_cast<uint32_t>(parsed);
                    m_isEditingMaxParticles = false;
                    
                    if (clamped) {
                        m_showClampWarning = true;
                        m_clampWarningTimer = 3.0f;
                    }
                } else if (ImGui::IsItemDeactivated()) {
                    m_isEditingMaxParticles = false;
                }
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                
                if (m_showClampWarning) {
                    ImGui::Dummy(ImVec2(0, 10.0f));
                    ImVec2 warnPos = ImGui::GetCursorScreenPos();
                    
                    ImGui::PushTextWrapPos(0.0f);
                    ImVec2 warnTextSize = ImGui::CalcTextSize((std::string(ICON_FA_CIRCLE_INFO) + " " + m_clampMessage).c_str(), nullptr, false, availW - 12.0f);
                    
                    dl->AddRectFilled(warnPos, ImVec2(warnPos.x + 3.0f, warnPos.y + warnTextSize.y), ImGui::GetColorU32(Theme::Colors::Warning), 1.5f);
                    
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12.0f);
                    ImGui::TextColored(Theme::Colors::Warning, "%s %s", ICON_FA_CIRCLE_INFO, m_clampMessage.c_str());
                    ImGui::PopTextWrapPos();
                    
                    m_clampWarningTimer -= ImGui::GetIO().DeltaTime;
                    if (m_clampWarningTimer <= 0.0f) m_showClampWarning = false;
                } else if (m_state->particleCount >= m_state->settings->maxParticles) {
                    ImGui::Dummy(ImVec2(0, 10.0f));
                    ImVec2 warnPos = ImGui::GetCursorScreenPos();
                    std::string msg = std::string(ICON_FA_TRIANGLE_EXCLAMATION) + " Particle buffer full. Increase Max Particles or reduce Spawn Rate.";
                    
                    ImGui::PushTextWrapPos(0.0f);
                    ImVec2 warnTextSize = ImGui::CalcTextSize(msg.c_str(), nullptr, false, availW - 12.0f);
                    
                    dl->AddRectFilled(warnPos, ImVec2(warnPos.x + 3.0f, warnPos.y + warnTextSize.y), ImGui::GetColorU32(Theme::Colors::Warning), 1.5f);
                    
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 12.0f);
                    ImGui::TextColored(Theme::Colors::Warning, "%s", msg.c_str());
                    ImGui::PopTextWrapPos();
                }
            }
        }
        ImGui::Dummy(ImVec2(0, 20.0f));

        if (Theme::Widgets::CollapsibleSection("emission", "EMISSION", true, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, 10.0f));
                
                float inputW = ImGui::GetContentRegionAvail().x * 0.7f;
                ImGui::TextColored(Theme::Colors::TextDim, "Spawn Rate");
                ImGui::Dummy(ImVec2(0, 4.0f));
                
                if (!m_isEditingSpawnRate) {
                    std::string formatted = Theme::Widgets::FormatNumber(m_state->settings->spawnRate);
                    strncpy(m_spawnRateInputBuffer, formatted.c_str(), sizeof(m_spawnRateInputBuffer));
                }
                
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                bool enterPressed = ImGui::InputText("##SpawnRate", m_spawnRateInputBuffer, sizeof(m_spawnRateInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue);
                AddDisabledTooltip();
                
                if (ImGui::IsItemEdited()) {
                    m_isEditingSpawnRate = true;
                }
                
                if (enterPressed || ImGui::IsItemDeactivatedAfterEdit()) {
                    std::string raw = m_spawnRateInputBuffer;
                    raw.erase(std::remove(raw.begin(), raw.end(), ','), raw.end());
                    int64_t parsed = std::strtoll(raw.c_str(), nullptr, 10);
                    if (parsed < 0) parsed = 0;
                    if (parsed > 1000000) parsed = 1000000;
                    m_state->settings->spawnRate = static_cast<uint32_t>(parsed);
                    m_isEditingSpawnRate = false;
                } else if (ImGui::IsItemDeactivated()) {
                    m_isEditingSpawnRate = false;
                }
                
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                ImGui::SameLine(0, 12.0f);
                ImGui::TextColored(Theme::Colors::TextDim, "/ sec");
            }
        }
        ImGui::Dummy(ImVec2(0, 20.0f));

        if (Theme::Widgets::CollapsibleSection("lifetime", "LIFETIME", false, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, 10.0f));
                float inputW = ImGui::GetContentRegionAvail().x * 0.7f;
                ImGui::TextColored(Theme::Colors::TextDim, "Lifetime");
                ImGui::Dummy(ImVec2(0, 4.0f));
                
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                ImGui::DragFloat("##Lifetime", &m_state->settings->defaultLifetime, 0.1f, 0.1f, 1000.0f);
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                ImGui::SameLine(0, 12.0f);
                ImGui::TextColored(Theme::Colors::TextDim, "sec");
            }
        }
        ImGui::Dummy(ImVec2(0, 20.0f));

        if (Theme::Widgets::CollapsibleSection("psize", "SIZE", false, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, 10.0f));
                float inputW = ImGui::GetContentRegionAvail().x * 0.7f;
                ImGui::TextColored(Theme::Colors::TextDim, "Radius");
                ImGui::Dummy(ImVec2(0, 4.0f));
                ImGui::PushItemWidth(inputW);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
                ImGui::BeginDisabled(isSim);
                ImGui::DragFloat("##Radius", &m_state->settings->defaultRadius, 0.1f, 0.1f, 100.0f);
                AddDisabledTooltip();
                ImGui::EndDisabled();
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                ImGui::SameLine(0, 12.0f);
                ImGui::TextColored(Theme::Colors::TextDim, "m");
            }
        }
        ImGui::Dummy(ImVec2(0, 20.0f));
        
        if (Theme::Widgets::CollapsibleSection("pvel", "VELOCITY", false, m_state->fontHeader)) {
            if (m_state->settings) {
                ImGui::Dummy(ImVec2(0, 10.0f));
                Theme::Widgets::Vector2Control("Speed", m_state->settings->defaultVelocity, 0.0f, isSim, "m/s");
            }
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

    ImGui::EndChild();

    // Quick Actions at bottom
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - quickActionsH + 10.0f);
    Theme::Widgets::SectionHeader("QUICK ACTIONS", m_state->fontHeader);

    ImVec2 actionBtnSize(ImGui::GetContentRegionAvail().x, 36);

    Theme::Widgets::QuickAction("qa_reset", ICON_FA_CAMERA_ROTATE, "Reset Camera", actionBtnSize, true);
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    if (Theme::Widgets::QuickAction("qa_clear", ICON_FA_TRASH, "Clear Particles", actionBtnSize, false)) {
        if (m_state) m_state->clearParticlesRequested = true;
    }
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    Theme::Widgets::QuickAction("qa_screenshot", ICON_FA_IMAGE, "Screenshot", actionBtnSize, true);
    ImGui::Dummy(ImVec2(0, Theme::Metrics::Spacing6));
    Theme::Widgets::QuickAction("qa_export", ICON_FA_FILE_EXPORT, "Export Scene", actionBtnSize, true);

    ImGui::End();
    ImGui::PopStyleVar(2);
}
