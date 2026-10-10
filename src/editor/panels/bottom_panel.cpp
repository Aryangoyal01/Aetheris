#include "bottom_panel.h"
#include "../theme/colors.h"
#include "../editor_state.h"
#include "../../core/log.h"
#include <extras/IconsFontAwesome6.h>
#include <imgui.h>
#include "../theme/widgets.h"
#include "../theme/theme.h"
#include <cctype>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>

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
            if (ImGui::Button("Clear", ImVec2(96, 32)))
            {
                Log::Clear();
                m_lastLogCount = 0;
            }
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - Theme::Metrics::Spacing4);

            float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing() + Theme::Metrics::Spacing8;
            ImGui::BeginChild("ConsoleScrollRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);

            const auto& entries = Log::GetEntries();
            if (entries.empty())
            {
                ImGui::TextColored(Theme::Colors::TextDim, "No log entries recorded.");
            }
            else
            {
                for (const auto& entry : entries)
                {
                    const char* sevStr = "INFO";
                    ImVec4 sevCol = Theme::Colors::Info;
                    switch (entry.level)
                    {
                    case LogLevel::Info:    sevStr = "INFO";    sevCol = Theme::Colors::Info; break;
                    case LogLevel::Warning: sevStr = "WARNING"; sevCol = Theme::Colors::Warning; break;
                    case LogLevel::Error:   sevStr = "ERROR";   sevCol = Theme::Colors::Error; break;
                    case LogLevel::System:  sevStr = "SYSTEM";  sevCol = Theme::Colors::System; break;
                    }
                    Theme::Widgets::ConsoleEntry(entry.timestamp.c_str(), sevStr, sevCol, entry.message.c_str(), m_state->fontDefault);
                }
            }

            if (entries.size() > m_lastLogCount)
            {
                if (m_autoScroll)
                {
                    ImGui::SetScrollHereY(1.0f);
                }
                m_lastLogCount = entries.size();
            }

            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndChild();

            // Interactive command input row
            ImGui::PushItemWidth(-1);
            if (ImGui::InputTextWithHint("##ConsoleInput", "Enter command ('help', 'gravity <x> <y>', 'play', 'pause', 'sync', 'clear particles')...", m_inputBuf, sizeof(m_inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string cmd = m_inputBuf;
                while (!cmd.empty() && std::isspace(static_cast<unsigned char>(cmd.front()))) cmd.erase(cmd.begin());
                while (!cmd.empty() && std::isspace(static_cast<unsigned char>(cmd.back()))) cmd.pop_back();

                if (!cmd.empty())
                {
                    std::vector<std::string> tokens;
                    std::istringstream iss(cmd);
                    std::string token;
                    while (iss >> token)
                    {
                        tokens.push_back(token);
                    }

                    std::string primary = tokens.empty() ? "" : tokens[0];
                    for (char& c : primary) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

                    if (primary == "help")
                    {
                        Log::System("Available console commands:");
                        Log::Info("  help                  - List available console commands");
                        Log::Info("  clear                 - Clear console log view");
                        Log::Info("  clear particles       - Clear all active simulation particles");
                        Log::Info("  gravity <x> <y>       - Set simulation gravity (e.g. gravity 0 -50)");
                        Log::Info("  pause                 - Pause simulation execution");
                        Log::Info("  play                  - Resume simulation execution");
                        Log::Info("  sync                  - Download GPU particles to CPU state");
                        Log::Info("  stats                 - Show real-time performance telemetry");
                        Log::Info("  reset                 - Reset simulation (clear particles)");
                    }
                    else if (primary == "clear")
                    {
                        if (tokens.size() > 1)
                        {
                            std::string sub = tokens[1];
                            for (char& c : sub) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
                            if (sub == "particles")
                            {
                                if (m_state)
                                {
                                    m_state->clearParticlesRequested = true;
                                    Log::System("Clear particles request dispatched.");
                                }
                                else
                                {
                                    Log::Warning("EditorState is not available.");
                                }
                            }
                            else
                            {
                                Log::Warning(("Unknown clear target: '" + tokens[1] + "'. Did you mean 'clear particles'?").c_str());
                            }
                        }
                        else
                        {
                            Log::Clear();
                            m_lastLogCount = 0;
                        }
                    }
                    else if (primary == "clear_particles" || primary == "reset")
                    {
                        if (m_state)
                        {
                            m_state->clearParticlesRequested = true;
                            Log::System("Clear particles request dispatched.");
                        }
                        else
                        {
                            Log::Warning("EditorState is not available.");
                        }
                    }
                    else if (primary == "gravity")
                    {
                        if (tokens.size() != 3)
                        {
                            Log::Warning("Usage: gravity <x> <y> (e.g. gravity 0 -50)");
                        }
                        else
                        {
                            try
                            {
                                size_t idxX = 0, idxY = 0;
                                float gx = std::stof(tokens[1], &idxX);
                                float gy = std::stof(tokens[2], &idxY);
                                if (idxX != tokens[1].size() || idxY != tokens[2].size() || !std::isfinite(gx) || !std::isfinite(gy))
                                {
                                    Log::Warning("Invalid gravity values. Both X and Y must be finite numbers.");
                                }
                                else
                                {
                                    if (m_state && m_state->settings)
                                    {
                                        m_state->settings->gravity[0] = gx;
                                        m_state->settings->gravity[1] = gy;
                                        Log::Info(("Gravity set to (" + std::to_string(gx) + ", " + std::to_string(gy) + ") m/s^2").c_str());
                                    }
                                    else
                                    {
                                        Log::Warning("Simulation settings unavailable.");
                                    }
                                }
                            }
                            catch (const std::exception&)
                            {
                                Log::Warning("Invalid numeric arguments for gravity.");
                            }
                        }
                    }
                    else if (primary == "pause")
                    {
                        if (m_state && m_state->settings)
                        {
                            m_state->settings->state = SimulationState::Pause;
                            Log::Info("Simulation state: Paused.");
                        }
                        else
                        {
                            Log::Warning("Simulation settings unavailable.");
                        }
                    }
                    else if (primary == "play")
                    {
                        if (m_state && m_state->settings)
                        {
                            m_state->settings->state = SimulationState::Play;
                            Log::Info("Simulation state: Playing.");
                        }
                        else
                        {
                            Log::Warning("Simulation settings unavailable.");
                        }
                    }
                    else if (primary == "sync")
                    {
                        if (m_state && m_state->settings)
                        {
                            if (!m_state->settings->useComputeShader)
                            {
                                Log::Warning("Sync is only available when compute shader simulation is enabled.");
                            }
                            else
                            {
                                m_state->syncRequested = true;
                                Log::System("GPU to CPU particle sync requested.");
                            }
                        }
                        else
                        {
                            Log::Warning("EditorState is not available.");
                        }
                    }
                    else if (primary == "stats")
                    {
                        if (m_state)
                        {
                            std::string statMsg = "FPS: " + std::to_string(m_state->fps) + 
                                                  " | Frame Time: " + std::to_string(m_state->frameTimeMs) + " ms" +
                                                  " | GPU Time: N/A (unmetered)" +
                                                  " | Particles: " + std::to_string(m_state->particleCount);
                            Log::Info(statMsg.c_str());
                        }
                    }
                    else
                    {
                        std::string unknownMsg = "Unknown command: '" + cmd + "'. Type 'help' for available commands.";
                        Log::Warning(unknownMsg.c_str());
                    }
                }
                m_inputBuf[0] = '\0';
                ImGui::SetKeyboardFocusHere(-1);
            }
            ImGui::PopItemWidth();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(" Logs "))
        {
            ImGui::Spacing();
            ImGui::BeginChild("LogsScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (m_state->fontDefault) ImGui::PushFont(m_state->fontDefault);

            const auto& entries = Log::GetEntries();
            if (entries.empty())
            {
                ImGui::TextColored(Theme::Colors::TextDim, "No log entries recorded.");
            }
            else
            {
                for (const auto& entry : entries)
                {
                    const char* sevStr = "INFO";
                    ImVec4 sevCol = Theme::Colors::Info;
                    switch (entry.level)
                    {
                    case LogLevel::Info:    sevStr = "INFO";    sevCol = Theme::Colors::Info; break;
                    case LogLevel::Warning: sevStr = "WARNING"; sevCol = Theme::Colors::Warning; break;
                    case LogLevel::Error:   sevStr = "ERROR";   sevCol = Theme::Colors::Error; break;
                    case LogLevel::System:  sevStr = "SYSTEM";  sevCol = Theme::Colors::System; break;
                    }
                    Theme::Widgets::ConsoleEntry(entry.timestamp.c_str(), sevStr, sevCol, entry.message.c_str(), m_state->fontDefault);
                }
            }

            if (m_state->fontDefault) ImGui::PopFont();
            ImGui::EndChild();
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
