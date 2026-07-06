#include "widgets.h"
#include "colors.h"
#include "theme.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <extras/IconsFontAwesome6.h>
#include <cstring>
#include <cmath>

namespace Theme {
namespace Widgets {

    // ============================================================
    // Format large numbers with commas
    // ============================================================
    std::string FormatNumber(uint32_t value) {
        std::string s = std::to_string(value);
        int n = static_cast<int>(s.length()) - 3;
        while (n > 0) {
            s.insert(n, ",");
            n -= 3;
        }
        return s;
    }

    // ============================================================
    // Helper: Draw a rounded rect with optional rounding corners
    // ============================================================
    static void DrawRoundedRect(ImDrawList* dl, ImVec2 min, ImVec2 max, ImU32 col, float r, ImDrawFlags flags = ImDrawFlags_RoundCornersAll)
    {
        dl->AddRectFilled(min, max, col, r, flags);
    }

    // ============================================================
    // ToolbarButton - Used in main toolbar and viewport toolbar
    // ============================================================
    bool ToolbarButton(const char* id, const char* label, bool isActive, ImVec2 size)
    {
        bool clicked = false;
        if (size.x == 0 && size.y == 0) {
            size = ImVec2(Metrics::ToolbarHeight * 0.75f, Metrics::ToolbarHeight * 0.75f);
        }

        ImGui::PushID(id);
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton("##btn", size);
        bool isHovered = ImGui::IsItemHovered();
        bool isHeld = ImGui::IsItemActive();
        if (ImGui::IsItemClicked()) clicked = true;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Background with clear state hierarchy
        ImU32 bgCol = 0;
        if (isActive) {
            bgCol = ImGui::GetColorU32(Colors::AccentPurple);
        } else if (isHeld) {
            bgCol = ImGui::GetColorU32(Colors::AccentCyanDim);
        } else if (isHovered) {
            bgCol = ImGui::GetColorU32(Colors::SurfaceHover);
        }

        if (bgCol) {
            dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgCol, Radii::Button);
        }

        // Text with clear active/hover distinction
        ImVec4 textColor = isActive ? Colors::Text : (isHovered ? Colors::Text : Colors::TextDim);
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos(pos.x + (size.x - textSize.x) * 0.5f, pos.y + (size.y - textSize.y) * 0.5f);
        dl->AddText(textPos, ImGui::GetColorU32(textColor), label);

        ImGui::PopID();
        return clicked;
    }

    // ============================================================
    // SidebarItem - Navigation rail item
    // ============================================================
    bool SidebarItem(const char* id, const char* icon, const char* label, bool isActive, ImFont* font)
    {
        bool clicked = false;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float availW = ImGui::GetContentRegionAvail().x;
        ImVec2 size = ImVec2(availW, Metrics::SidebarItemHeight);

        ImGui::PushID(id);

        if (font) ImGui::PushFont(font);

        ImGui::InvisibleButton("##btn", size);
        bool isHovered = ImGui::IsItemHovered();
        bool isHeld = ImGui::IsItemActive();
        if (ImGui::IsItemClicked()) clicked = true;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Background
        if (isActive) {
            dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                ImGui::GetColorU32(Colors::Surface), Radii::Button);
            // Strong purple left accent bar — 5px for authority
            dl->AddRectFilled(
                ImVec2(pos.x, pos.y + Metrics::Spacing10),
                ImVec2(pos.x + 5.0f, pos.y + size.y - Metrics::Spacing10),
                ImGui::GetColorU32(Colors::AccentPurple), 2.5f);
        } else if (isHovered) {
            dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
                ImGui::GetColorU32(Colors::SurfaceHover), Radii::Button);
        }

        // Icon color
        ImVec4 iconColor = isActive ? Colors::AccentPurple : (isHovered ? Colors::Text : Colors::TextDim);
        ImVec4 textColor = isActive ? Colors::Text : (isHovered ? Colors::Text : Colors::TextDim);

        // Calculate layout: icon + label side by side, centered vertically
        ImVec2 iconSize = ImGui::CalcTextSize(icon);
        ImVec2 labelSize = ImGui::CalcTextSize(label);

        float totalW = iconSize.x + Metrics::Spacing12 + labelSize.x;
        float startX = pos.x + (size.x - totalW) * 0.5f;
        float centerY = pos.y + (size.y - iconSize.y) * 0.5f;

        // Draw icon
        dl->AddText(ImVec2(startX, centerY), ImGui::GetColorU32(iconColor), icon);

        // Draw label
        dl->AddText(ImVec2(startX + iconSize.x + Metrics::Spacing12, centerY), ImGui::GetColorU32(textColor), label);

        if (font) ImGui::PopFont();

        ImGui::PopID();
        return clicked;
    }

    // ============================================================
    // StatusChip - Non-interactive pill for telemetry
    // ============================================================
    void StatusChip(const char* label, const char* value, ImVec4 color)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        ImVec2 valueSize = (value && value[0] != '\0') ? ImGui::CalcTextSize(value) : ImVec2(0, 0);

        float chipH = labelSize.y + Metrics::Spacing12;
        float chipW;
        if (valueSize.x > 0) {
            chipW = labelSize.x + valueSize.x + Metrics::Spacing24 + Metrics::Spacing8;
        } else {
            chipW = labelSize.x + Metrics::Spacing24 * 2;
        }
        if (chipW < 96.0f) chipW = 96.0f;

        ImGui::Dummy(ImVec2(chipW, chipH));

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Pill background
        dl->AddRectFilled(pos, ImVec2(pos.x + chipW, pos.y + chipH),
            ImGui::GetColorU32(Colors::PanelBackground), Radii::Pill);

        // Left color indicator dot
        dl->AddCircleFilled(
            ImVec2(pos.x + Metrics::Spacing12, pos.y + chipH * 0.5f),
            3.0f,
            ImGui::GetColorU32(color));

        // Label text
        float textY = pos.y + (chipH - labelSize.y) * 0.5f;
        dl->AddText(ImVec2(pos.x + Metrics::Spacing20, textY),
            ImGui::GetColorU32(Colors::TextDim), label);

        // Value text
        if (valueSize.x > 0) {
            float valueX = pos.x + Metrics::Spacing20 + labelSize.x + Metrics::Spacing8;
            dl->AddText(ImVec2(valueX, textY),
                ImGui::GetColorU32(color), value);
        }
    }

    // ============================================================
    // SectionHeader - White title with subtle separator
    // ============================================================
    void SectionHeader(const char* title, ImFont* font)
    {
        if (font) ImGui::PushFont(font);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", title);
        if (font) ImGui::PopFont();

        ImGui::PushStyleColor(ImGuiCol_Separator, Colors::BorderSubtle);
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
    }

    // ============================================================
    // PropertyRow - Two-column aligned property
    // ============================================================
    void PropertyRow(const char* label, const char* value)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float contentWidth = ImGui::GetContentRegionAvail().x;
        float labelWidth = contentWidth * 0.38f;

        ImVec2 labelSize = ImGui::CalcTextSize(label);
        ImVec2 valueSize = ImGui::CalcTextSize(value);

        float rowHeight = labelSize.y + Metrics::Spacing8;
        ImGui::Dummy(ImVec2(contentWidth, rowHeight));

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Label
        float textY = pos.y + Metrics::Spacing4;
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextDim), label);

        // Value
        dl->AddText(ImVec2(pos.x + labelWidth, textY), ImGui::GetColorU32(Colors::Text), value);
    }

    // ============================================================
    // ToggleSwitch - Custom drawn toggle
    // ============================================================
    bool ToggleSwitch(const char* id, bool& value, ImVec2 size)
    {
        bool clicked = false;

        if (size.x == 0) size.x = 44.0f;
        if (size.y == 0) size.y = 22.0f;

        ImGui::PushID(id);
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton("##toggle", size);
        if (ImGui::IsItemClicked()) {
            value = !value;
            clicked = true;
        }

        bool isHovered = ImGui::IsItemHovered();
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Track
        ImVec4 trackColor = value ? Colors::AccentPurple : Colors::SurfaceActive;
        if (isHovered) {
            trackColor = value ? Colors::AccentPurpleDim : Colors::SurfaceHover;
        }
        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y),
            ImGui::GetColorU32(trackColor), Radii::Pill);

        // Thumb
        float thumbR = (size.y - 4.0f) * 0.5f;
        float thumbX = value ? (pos.x + size.x - thumbR - 2.0f) : (pos.x + thumbR + 2.0f);
        float thumbY = pos.y + size.y * 0.5f;

        ImVec4 thumbColor = value ? Colors::Text : Colors::TextDim;
        dl->AddCircleFilled(ImVec2(thumbX, thumbY), thumbR, ImGui::GetColorU32(thumbColor));

        ImGui::PopID();
        return clicked;
    }

    // ============================================================
    // CollapsibleSection - Custom collapsible with arrow
    // ============================================================
    bool CollapsibleSection(const char* id, const char* title, bool defaultOpen, ImFont* font)
    {
        ImGui::PushID(id);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float availW = ImGui::GetContentRegionAvail().x;
        float headerH = Metrics::SidebarItemHeight * 0.72f;

        // Header button
        ImGui::InvisibleButton("##header", ImVec2(availW, headerH));
        bool isHovered = ImGui::IsItemHovered();
        bool clicked = ImGui::IsItemClicked();

        // State storage
        ImGuiID storageID = ImGui::GetID("open");
        ImGuiStorage* storage = ImGui::GetStateStorage();
        if (storage->GetInt(storageID, -1) == -1) {
            storage->SetInt(storageID, defaultOpen ? 1 : 0);
        }
        bool isOpen = storage->GetInt(storageID, defaultOpen ? 1 : 0) != 0;
        if (clicked) {
            isOpen = !isOpen;
            storage->SetInt(storageID, isOpen ? 1 : 0);
        }

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Hover background
        if (isHovered) {
            dl->AddRectFilled(pos, ImVec2(pos.x + availW, pos.y + headerH),
                ImGui::GetColorU32(Colors::SurfaceHover), Radii::Button);
        }

        // Bottom separator line for visual weight
        dl->AddLine(
            ImVec2(pos.x, pos.y + headerH - 1.0f),
            ImVec2(pos.x + availW, pos.y + headerH - 1.0f),
            ImGui::GetColorU32(Colors::BorderSubtle), 1.0f);

        // Arrow indicator — push font for icon
        if (font) ImGui::PushFont(font);
        const char* arrow = isOpen ? ICON_FA_CHEVRON_DOWN : ICON_FA_CHEVRON_RIGHT;
        ImVec2 arrowSize = ImGui::CalcTextSize(arrow);
        float arrowY = pos.y + (headerH - arrowSize.y) * 0.5f;
        dl->AddText(ImVec2(pos.x + Metrics::Spacing8, arrowY),
            ImGui::GetColorU32(Colors::TextDim), arrow);

        // Title — use header font for more weight
        ImVec2 titleSize = ImGui::CalcTextSize(title);
        float titleY = pos.y + (headerH - titleSize.y) * 0.5f;
        ImVec4 titleColor = isOpen ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : Colors::TextDim;
        dl->AddText(ImVec2(pos.x + Metrics::Spacing20 + arrowSize.x + Metrics::Spacing6, titleY),
            ImGui::GetColorU32(titleColor), title);
        if (font) ImGui::PopFont();

        ImGui::PopID();

        return isOpen;
    }

    // ============================================================
    // PropertyValue - Label + read-only value field
    // ============================================================
    void PropertyValue(const char* label, const char* value, const char* unit)
    {
        float availW = ImGui::GetContentRegionAvail().x;
        float labelW = availW * 0.38f;
        float valueW = availW - labelW - Metrics::Spacing12;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float rowH = Metrics::SidebarItemHeight * 0.68f;

        // Draw label
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        float textY = pos.y + (rowH - labelSize.y) * 0.5f;
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextDim), label);

        // Draw value field background
        ImVec2 fieldMin = ImVec2(pos.x + labelW, pos.y + Metrics::Spacing6);
        ImVec2 fieldMax = ImVec2(pos.x + labelW + valueW, pos.y + rowH - Metrics::Spacing6);
        dl->AddRectFilled(fieldMin, fieldMax, ImGui::GetColorU32(Colors::Surface), Radii::Input);

        // Draw value text
        ImVec2 valueSize = ImGui::CalcTextSize(value);
        float valueTextY = fieldMin.y + ((fieldMax.y - fieldMin.y) - valueSize.y) * 0.5f;
        dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing12, valueTextY),
            ImGui::GetColorU32(Colors::Text), value);

        // Draw unit text if present
        if (unit && unit[0] != '\0') {
            ImVec2 unitSize = ImGui::CalcTextSize(unit);
            float unitTextY = fieldMin.y + ((fieldMax.y - fieldMin.y) - unitSize.y) * 0.5f;
            dl->AddText(ImVec2(fieldMax.x - unitSize.x - Metrics::Spacing12, unitTextY),
                ImGui::GetColorU32(Colors::TextMuted), unit);
        }

        ImGui::Dummy(ImVec2(0, rowH));
    }

    // ============================================================
    // PropertyDropdown - Label + dropdown field
    // ============================================================
    void PropertyDropdown(const char* label, const char* value)
    {
        float availW = ImGui::GetContentRegionAvail().x;
        float labelW = availW * 0.38f;
        float valueW = availW - labelW - Metrics::Spacing8;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float rowH = Metrics::SidebarItemHeight * 0.72f;

        // Label
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        float textY = pos.y + (rowH - labelSize.y) * 0.5f;
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextDim), label);

        // Value field
        ImVec2 fieldMin = ImVec2(pos.x + labelW, pos.y + Metrics::Spacing4);
        ImVec2 fieldMax = ImVec2(pos.x + labelW + valueW, pos.y + rowH - Metrics::Spacing4);
        dl->AddRectFilled(fieldMin, fieldMax, ImGui::GetColorU32(Colors::Surface), Radii::Input);

        // Value text
        ImVec2 valueSize = ImGui::CalcTextSize(value);
        float valueTextY = fieldMin.y + ((fieldMax.y - fieldMin.y) - valueSize.y) * 0.5f;
        dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing10, valueTextY),
            ImGui::GetColorU32(Colors::Text), value);

        // Dropdown arrow
        const char* arrow = ICON_FA_CHEVRON_DOWN;
        ImVec2 arrowSize = ImGui::CalcTextSize(arrow);
        float arrowY = fieldMin.y + ((fieldMax.y - fieldMin.y) - arrowSize.y) * 0.5f;
        dl->AddText(ImVec2(fieldMax.x - arrowSize.x - Metrics::Spacing10, arrowY),
            ImGui::GetColorU32(Colors::TextDim), arrow);

        ImGui::Dummy(ImVec2(0, rowH));
    }

    // ============================================================
    // Vector2Control - Generic Cartesian/Polar control with Gizmo
    // ============================================================
    bool Vector2Control(const char* label, float* v, float resetValue, bool disabled)
    {
        bool changed = false;
        ImGui::PushID(label);

        ImGui::BeginGroup();
        
        // Mode State
        ImGuiID modeID = ImGui::GetID("vector_mode");
        ImGuiID angleID = ImGui::GetID("polar_angle");
        ImGuiStorage* storage = ImGui::GetStateStorage();
        // 0 = Cartesian, 1 = Polar
        int mode = storage->GetInt(modeID, 0);

        float availW = ImGui::GetContentRegionAvail().x;
        
        ImGui::Dummy(ImVec2(0, 10.0f));
        ImGui::BeginDisabled(disabled);
        
        // --- Segmented Control for Mode Selection ---
        ImGui::BeginGroup();
        float btnW = (availW - 1.0f) * 0.5f;
        ImVec2 btnSize(btnW, 0);
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, Theme::Radii::Button); 
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        
        btnW = availW * 0.5f;
        btnSize = ImVec2(btnW, 0);
        
        auto RenderSegment = [&](const char* label, int targetMode) {
            bool isActive = (mode == targetMode);
            
            // Subtle transition colors
            ImVec4 btnColor = isActive ? Colors::AccentPurple : Colors::BackgroundDark;
            ImVec4 btnHover = isActive ? Colors::AccentPurpleBright : Colors::Surface;
            ImVec4 btnActive = isActive ? Colors::AccentPurple : Colors::SurfaceHover;
            ImVec4 borderColor = isActive ? Colors::AccentPurple : ImVec4(Colors::BorderSubtle.x, Colors::BorderSubtle.y, Colors::BorderSubtle.z, 0.5f);
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetColorU32(btnColor));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetColorU32(btnHover));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetColorU32(btnActive));
            ImGui::PushStyleColor(ImGuiCol_Border, ImGui::GetColorU32(borderColor));
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(isActive ? ImVec4(1,1,1,1) : Colors::TextDim));
            
            if (ImGui::Button(label, btnSize)) {
                mode = targetMode;
                storage->SetInt(modeID, mode);
            }
            ImGui::PopStyleColor(5);
        };
        
        RenderSegment("Cartesian", 0);
        ImGui::SameLine();
        RenderSegment("Polar", 1);
        
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
        
        ImGui::Dummy(ImVec2(0, 12.0f));
        
        if (mode == 0) // Cartesian Mode
        {
            float inputW = availW * 0.7f;
            
            ImGui::TextColored(Colors::TextDim, "X");
            ImGui::Dummy(ImVec2(0, 4.0f));
            ImGui::PushItemWidth(inputW);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
            float valX = v[0];
            if (ImGui::DragFloat("##x", &valX, 0.1f, 0.0f, 0.0f, "%.3f")) { v[0] = valX; changed = true; }
            ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::SameLine(0, 12.0f);
            ImGui::TextColored(Colors::TextDim, "m/s");
            
            ImGui::Dummy(ImVec2(0, 12.0f));
            
            ImGui::TextColored(Colors::TextDim, "Y");
            ImGui::Dummy(ImVec2(0, 4.0f));
            ImGui::PushItemWidth(inputW);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
            float valY = v[1];
            if (ImGui::DragFloat("##y", &valY, 0.1f, 0.0f, 0.0f, "%.3f")) { v[1] = valY; changed = true; }
            ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::SameLine(0, 12.0f);
            ImGui::TextColored(Colors::TextDim, "m/s");
            
            ImGui::EndDisabled();
            ImGui::EndGroup();
            ImGui::PopID();
            return changed;
        }
        else // Polar Mode
        {
            float magnitude = std::hypot(v[0], v[1]);
            float rawAngle = std::atan2(v[1], v[0]); 
            
            // Persist angle in UI storage so we don't lose it when Magnitude = 0
            float angleRad = storage->GetFloat(angleID, 0.0f);
            if (magnitude > 0.0001f) {
                angleRad = rawAngle;
                storage->SetFloat(angleID, angleRad);
            }
            
            float angleDeg = angleRad * (180.0f / 3.1415926535f);
            if (angleDeg < 0.0f) angleDeg += 360.0f; 
            
            float inputW = availW * 0.7f;
            
            float magEdit = magnitude;
            float angEdit = angleDeg;
            
            ImGui::TextColored(Colors::TextDim, "Magnitude");
            ImGui::Dummy(ImVec2(0, 4.0f));
            ImGui::PushItemWidth(inputW);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
            if (ImGui::DragFloat("##mag", &magEdit, 0.1f, 0.0f, 0.0f, "%.3f")) {
                if (magEdit < 0.0f) magEdit = 0.0f;
                v[0] = magEdit * std::cos(angleRad);
                v[1] = magEdit * std::sin(angleRad);
                changed = true;
            }
            ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::SameLine(0, 12.0f);
            ImGui::TextColored(Colors::TextDim, "m/s");
            
            ImGui::Dummy(ImVec2(0, 12.0f));
            
            ImGui::TextColored(Colors::TextDim, "Angle");
            ImGui::Dummy(ImVec2(0, 4.0f));
            ImGui::PushItemWidth(inputW);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y - 1.0f));
            if (ImGui::DragFloat("##ang", &angEdit, 1.0f, 0.0f, 0.0f, "%.1f\xc2\xb0")) {
                angEdit = std::fmod(angEdit, 360.0f);
                if (angEdit < 0.0f) angEdit += 360.0f;
                float newRad = angEdit * (3.1415926535f / 180.0f);
                v[0] = magnitude * std::cos(newRad);
                v[1] = magnitude * std::sin(newRad);
                storage->SetFloat(angleID, newRad); // Save explicitly
                angleRad = newRad;
                changed = true;
            }
            ImGui::PopStyleVar();
            ImGui::PopItemWidth();
            ImGui::SameLine(0, 12.0f);
            ImGui::TextColored(Colors::TextDim, "\xc2\xb0");
            
            ImGui::EndDisabled(); 
            
            ImGui::Dummy(ImVec2(0, 20.0f));
            
            // --- Large Direction Dial (Premium Engineering Style) ---
            float padding = availW * 0.05f; 
            float gizmoSize = availW - padding * 2.0f;
            float totalGizmoSize = gizmoSize + padding * 2.0f;
            
            float offsetX = (ImGui::GetContentRegionAvail().x - totalGizmoSize) * 0.5f;
            if (offsetX < 0) offsetX = 0;
            
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
            
            ImVec2 gizmoPos = ImGui::GetCursorScreenPos();
            ImVec2 center = ImVec2(gizmoPos.x + totalGizmoSize * 0.5f, gizmoPos.y + totalGizmoSize * 0.5f);
            float radius = gizmoSize * 0.5f;
            
            ImGui::InvisibleButton("##dial", ImVec2(totalGizmoSize, totalGizmoSize));
            bool gizmoHovered = ImGui::IsItemHovered();
            bool gizmoActive = ImGui::IsItemActive();
            
            // Simple animation interpolation
            float targetAnim = gizmoActive ? 1.0f : (gizmoHovered ? 0.5f : 0.0f);
            float currentAnim = storage->GetFloat(ImGui::GetID("DialAnim"), 0.0f);
            currentAnim += (targetAnim - currentAnim) * (ImGui::GetIO().DeltaTime * 15.0f); // ~100ms
            storage->SetFloat(ImGui::GetID("DialAnim"), currentAnim);
            
            float activeSnapDeg = -1.0f; 
            
            if (!disabled && (gizmoActive || ImGui::IsItemClicked())) {
                ImVec2 mousePos = ImGui::GetIO().MousePos;
                
                float newRad = std::atan2(-(mousePos.y - center.y), mousePos.x - center.x);
                float newDeg = newRad * (180.0f / 3.1415926535f);
                if (newDeg < 0.0f) newDeg += 360.0f;
                
                float testDeg = newDeg;
                if (testDeg > 350.0f) testDeg -= 360.0f; 
                
                float nearest5 = std::round(testDeg / 5.0f) * 5.0f;
                if (std::abs(testDeg - nearest5) <= 0.5f) { 
                    float snapVal = nearest5;
                    if (snapVal < 0.0f) snapVal += 360.0f;
                    if (snapVal >= 360.0f) snapVal -= 360.0f;
                    newDeg = snapVal;
                    newRad = newDeg * (3.1415926535f / 180.0f);
                    activeSnapDeg = newDeg;
                }
                
                v[0] = magnitude * std::cos(newRad);
                v[1] = magnitude * std::sin(newRad);
                storage->SetFloat(angleID, newRad); 
                
                changed = true;
                angleRad = newRad; 
                angleDeg = newDeg;
            }
            
            ImDrawList* dl = ImGui::GetWindowDrawList();
            
            // Colors
            ImU32 bgColor = ImGui::GetColorU32(Colors::Surface);
            ImU32 bgInnerColor = ImGui::GetColorU32(Colors::BackgroundDark);
            
            // Brighter outer ring
            ImU32 borderOuterColor = ImGui::GetColorU32(ImVec4(Colors::BorderStrong.x + 0.1f, Colors::BorderStrong.y + 0.1f, Colors::BorderStrong.z + 0.1f, 1.0f));
            ImU32 borderInnerColor = ImGui::GetColorU32(Colors::BorderSubtle);
            
            // High contrast Tick Hierarchy
            ImU32 tickMajorColor = ImGui::GetColorU32(ImVec4(Colors::Text.x, Colors::Text.y, Colors::Text.z, 0.9f));
            ImU32 tickMediumColor = ImGui::GetColorU32(ImVec4(Colors::TextDim.x, Colors::TextDim.y, Colors::TextDim.z, 0.6f));
            ImU32 tickMinorColor = ImGui::GetColorU32(ImVec4(Colors::BorderStrong.x, Colors::BorderStrong.y, Colors::BorderStrong.z, 0.3f));
            
            if (disabled) {
                borderOuterColor = ImGui::GetColorU32(ImVec4(Colors::BorderSubtle.x, Colors::BorderSubtle.y, Colors::BorderSubtle.z, 0.3f));
                borderInnerColor = ImGui::GetColorU32(ImVec4(Colors::BorderSubtle.x, Colors::BorderSubtle.y, Colors::BorderSubtle.z, 0.15f));
                tickMajorColor = ImGui::GetColorU32(ImVec4(Colors::TextDim.x, Colors::TextDim.y, Colors::TextDim.z, 0.3f));
                tickMediumColor = ImGui::GetColorU32(ImVec4(Colors::BorderStrong.x, Colors::BorderStrong.y, Colors::BorderStrong.z, 0.3f));
                tickMinorColor = ImGui::GetColorU32(ImVec4(Colors::BorderSubtle.x, Colors::BorderSubtle.y, Colors::BorderSubtle.z, 0.2f));
            }
            
            // Subtle radial falloff
            dl->AddCircleFilled(center, radius, bgInnerColor, 64);
            dl->AddCircleFilled(center, radius * 0.85f, ImGui::GetColorU32(ImVec4(Colors::Surface.x, Colors::Surface.y, Colors::Surface.z, 0.2f)), 64);
            dl->AddCircleFilled(center, radius * 0.60f, ImGui::GetColorU32(ImVec4(Colors::Surface.x, Colors::Surface.y, Colors::Surface.z, 0.4f)), 64);
            dl->AddCircleFilled(center, radius * 0.35f, bgColor, 64);
            
            // Double Outer Ring
            // +20% brightness for stronger contrast
            ImU32 borderOuterColorBright = ImGui::GetColorU32(ImVec4(Colors::BorderStrong.x + 0.2f, Colors::BorderStrong.y + 0.2f, Colors::BorderStrong.z + 0.2f, 1.0f));
            dl->AddCircle(center, radius, disabled ? borderOuterColor : borderOuterColorBright, 64, 2.0f);
            dl->AddCircle(center, radius - 2.0f, borderInnerColor, 64, 1.0f);
            
            // Tick Marks
            for (int d = 0; d < 360; d += 5) {
                float r = d * (3.1415926535f / 180.0f);
                float c = std::cos(r);
                float s = std::sin(r);
                
                float innerTickR = radius - 6.0f;
                float outerTickR = radius - 2.0f;
                ImU32 tColor = tickMinorColor;
                float tickThick = 1.0f;
                
                if (d % 90 == 0) {
                    innerTickR = radius - 20.0f; 
                    tColor = tickMajorColor;
                    tickThick = 2.0f;
                }
                else if (d % 30 == 0) {
                    innerTickR = radius - 12.0f; 
                    tColor = tickMediumColor;
                    tickThick = 1.5f;
                }
                
                // Active snapped tick highlight
                if (gizmoActive && std::abs(activeSnapDeg - d) < 0.1f) {
                    tColor = ImGui::GetColorU32(Colors::AccentCyan);
                    tickThick += 1.5f;
                    innerTickR -= 4.0f; 
                }
                
                ImVec2 p1(center.x + c * innerTickR, center.y - s * innerTickR);
                ImVec2 p2(center.x + c * outerTickR, center.y - s * outerTickR);
                dl->AddLine(p1, p2, tColor, tickThick);
            }
            
            // Arrow Colors & Proportions (No bloom)
            ImU32 arrowColor = ImGui::GetColorU32(Colors::AccentPurple);
            if (disabled) {
                arrowColor = ImGui::GetColorU32(Colors::TextDim);
            } else if (gizmoActive) {
                arrowColor = ImGui::GetColorU32(Colors::AccentCyan); // Subtle change on drag, no bloom
            } else if (gizmoHovered) {
                arrowColor = ImGui::GetColorU32(Colors::AccentPurpleBright);
            }
            
            float shaftThickness = 7.0f;
            float arrowLen = radius * 0.97f; // Closer to outer ring
            float arrowHeadLen = 36.0f;
            float arrowHeadWidth = 28.0f;
            float pivotRadius = 13.0f;
            
            ImVec2 arrowTip = ImVec2(center.x + std::cos(angleRad) * arrowLen, center.y - std::sin(angleRad) * arrowLen);
            ImVec2 arrowBase = ImVec2(center.x + std::cos(angleRad) * (arrowLen - arrowHeadLen), center.y - std::sin(angleRad) * (arrowLen - arrowHeadLen));
            
            float rightRad = angleRad - (3.1415926535f / 2.0f);
            float leftRad = angleRad + (3.1415926535f / 2.0f);
            ImVec2 headRight = ImVec2(arrowBase.x + std::cos(rightRad) * (arrowHeadWidth * 0.5f), arrowBase.y - std::sin(rightRad) * (arrowHeadWidth * 0.5f));
            ImVec2 headLeft = ImVec2(arrowBase.x + std::cos(leftRad) * (arrowHeadWidth * 0.5f), arrowBase.y - std::sin(leftRad) * (arrowHeadWidth * 0.5f));
            
            // Draw Shaft extending to the tip for flawless join
            ImVec2 shaftStart = ImVec2(center.x + std::cos(angleRad) * pivotRadius, center.y - std::sin(angleRad) * pivotRadius);
            dl->AddLine(shaftStart, arrowTip, arrowColor, shaftThickness);
            
            // Draw Filled Arrowhead
            dl->AddTriangleFilled(arrowTip, headRight, headLeft, arrowColor);
            
            // Draw Central Circular Pivot (Premium Center Hub)
            ImU32 hubOutlineColor = ImGui::GetColorU32(Colors::BorderStrong); // More defined border
            dl->AddCircleFilled(center, pivotRadius * 1.2f, ImGui::GetColorU32(Colors::BackgroundDark), 32); 
            dl->AddCircle(center, pivotRadius * 1.2f, hubOutlineColor, 32, 2.0f);
            dl->AddCircleFilled(center, pivotRadius * 0.45f, arrowColor, 16); 
            
            ImGui::EndGroup();
            ImGui::PopID();
            return changed;
        }
    }

    // ============================================================
    // Vector3Property - X Y Z inputs in a row
    // ============================================================
    void Vector2Property(const char* label, const char* x, const char* y)
    {
        float availW = ImGui::GetContentRegionAvail().x;
        float labelW = availW * 0.38f;
        float fieldW = (availW - labelW - Metrics::Spacing8) / 2.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float rowH = Metrics::SidebarItemHeight * 0.72f;

        ImVec2 labelSize = ImGui::CalcTextSize(label);
        float textY = pos.y + (rowH - labelSize.y) * 0.5f;
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextDim), label);

        const char* components[2] = { x, y };
        ImVec4 labelColors[2] = { Colors::Error, Colors::Success };
        const char* compLabels[2] = { "X", "Y" };

        for (int i = 0; i < 2; i++) {
            float fieldX = pos.x + labelW + (fieldW + Metrics::Spacing8) * i;
            ImVec2 fieldMin = ImVec2(fieldX, pos.y + Metrics::Spacing4);
            ImVec2 fieldMax = ImVec2(fieldX + fieldW, pos.y + rowH - Metrics::Spacing4);

            dl->AddRectFilled(fieldMin, fieldMax, ImGui::GetColorU32(Colors::Surface), Radii::Input);

            ImVec2 compSize = ImGui::CalcTextSize(components[i]);
            float compY = fieldMin.y + ((fieldMax.y - fieldMin.y) - compSize.y) * 0.5f;

            ImVec2 compLabelSize = ImGui::CalcTextSize(compLabels[i]);
            dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing6, compY),
                ImGui::GetColorU32(labelColors[i]), compLabels[i]);

            dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing6 + compLabelSize.x + Metrics::Spacing6, compY),
                ImGui::GetColorU32(Colors::Text), components[i]);
        }

        ImGui::Dummy(ImVec2(0, rowH));
    }

    void Vector3Property(const char* label, const char* x, const char* y, const char* z)
    {
        float availW = ImGui::GetContentRegionAvail().x;
        float labelW = availW * 0.38f;
        float fieldW = (availW - labelW - Metrics::Spacing8 - Metrics::Spacing8 * 2) / 3.0f;

        ImVec2 pos = ImGui::GetCursorScreenPos();
        float rowH = Metrics::SidebarItemHeight * 0.72f;

        // Label
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        float textY = pos.y + (rowH - labelSize.y) * 0.5f;
        ImDrawList* dl = ImGui::GetWindowDrawList();
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextDim), label);

        // Three fields
        const char* components[3] = { x, y, z };
        ImVec4 labelColors[3] = { Colors::Error, Colors::Success, Colors::AccentCyan };

        for (int i = 0; i < 3; i++) {
            float fieldX = pos.x + labelW + (fieldW + Metrics::Spacing8) * i;
            ImVec2 fieldMin = ImVec2(fieldX, pos.y + Metrics::Spacing4);
            ImVec2 fieldMax = ImVec2(fieldX + fieldW, pos.y + rowH - Metrics::Spacing4);

            dl->AddRectFilled(fieldMin, fieldMax, ImGui::GetColorU32(Colors::Surface), Radii::Input);

            // Component label
            ImVec2 compSize = ImGui::CalcTextSize(components[i]);
            float compY = fieldMin.y + ((fieldMax.y - fieldMin.y) - compSize.y) * 0.5f;

            // Small colored label
            const char* compLabels[3] = { "X", "Y", "Z" };
            ImVec2 compLabelSize = ImGui::CalcTextSize(compLabels[i]);
            dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing6, compY),
                ImGui::GetColorU32(labelColors[i]), compLabels[i]);

            dl->AddText(ImVec2(fieldMin.x + Metrics::Spacing6 + compLabelSize.x + Metrics::Spacing6, compY),
                ImGui::GetColorU32(Colors::Text), components[i]);
        }

        ImGui::Dummy(ImVec2(0, rowH));
    }

    // ============================================================
    // QuickAction - Button for quick actions
    // ============================================================
    bool QuickAction(const char* id, const char* icon, const char* label, ImVec2 size, bool isPrimary)
    {
        bool clicked = false;
        if (size.x == 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y == 0) size.y = 36.0f; // Uniform height

        ImGui::PushID(id);
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton("##btn", size);
        bool isHovered = ImGui::IsItemHovered();
        bool isHeld = ImGui::IsItemActive();
        if (ImGui::IsItemClicked()) clicked = true;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // 8px rounded corners
        float cornerRadius = 8.0f;

        // Background with smooth state transitions
        ImU32 bgCol = ImGui::GetColorU32(Colors::Surface);
        if (isPrimary) {
            bgCol = ImGui::GetColorU32(Colors::Surface);
            if (isHeld) bgCol = ImGui::GetColorU32(Colors::SurfaceActive);
            else if (isHovered) bgCol = ImGui::GetColorU32(Colors::SurfaceHover);
        } else {
            bgCol = ImGui::GetColorU32(Colors::BackgroundDark);
            if (isHeld) bgCol = ImGui::GetColorU32(Colors::Surface);
            else if (isHovered) bgCol = ImGui::GetColorU32(ImVec4(Colors::SurfaceHover.x, Colors::SurfaceHover.y, Colors::SurfaceHover.z, 0.5f));
        }

        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgCol, cornerRadius);

        // Subtle border
        ImVec4 borderColor = isHovered ? Colors::BorderStrong : (isPrimary ? Colors::Border : Colors::BorderSubtle);
        dl->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), ImGui::GetColorU32(borderColor), cornerRadius, 0, isHovered ? 1.5f : 1.0f);

        // Icon + Label centered with identical spacing
        ImVec2 iconSize = ImGui::CalcTextSize(icon);
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        
        // Ensure icon sizing is consistent across all buttons (use a fixed width box for the icon)
        float fixedIconW = 20.0f;
        float spacing = 12.0f; 
        float totalW = fixedIconW + spacing + labelSize.x;
        
        float startX = pos.x + (size.x - totalW) * 0.5f;
        float centerY = pos.y + (size.y - iconSize.y) * 0.5f;

        ImVec4 iconColor = isHovered ? Colors::Text : (isPrimary ? Colors::TextDim : Colors::TextMuted);
        ImVec4 textColor = isHovered ? Colors::Text : (isPrimary ? Colors::Text : Colors::TextDim);

        // Center icon inside its fixed width
        float iconOffsetX = (fixedIconW - iconSize.x) * 0.5f;
        dl->AddText(ImVec2(startX + iconOffsetX, centerY), ImGui::GetColorU32(iconColor), icon);
        
        float textY = pos.y + (size.y - labelSize.y) * 0.5f;
        dl->AddText(ImVec2(startX + fixedIconW + spacing, textY),
            ImGui::GetColorU32(textColor), label);

        ImGui::PopID();
        return clicked;
    }

    // ============================================================
    // ConsoleEntry - Timestamped log entry with severity badge
    // ============================================================
    void ConsoleEntry(const char* timestamp, const char* severity, ImVec4 severityColor, const char* message, ImFont* badgeFont)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float availW = ImGui::GetContentRegionAvail().x;
        float rowH = Metrics::FontSizeDefault + Metrics::Spacing10;

        ImDrawList* dl = ImGui::GetWindowDrawList();
        float textY = pos.y + (rowH - Metrics::FontSizeDefault) * 0.5f;

        // Timestamp
        ImVec2 tsSize = ImGui::CalcTextSize(timestamp);
        dl->AddText(ImVec2(pos.x, textY), ImGui::GetColorU32(Colors::TextMuted), timestamp);

        // Severity badge
        float badgeX = pos.x + tsSize.x + Metrics::Spacing16;
        ImVec2 sevSize = ImGui::CalcTextSize(severity);
        float badgeW = sevSize.x + Metrics::Spacing16;
        float badgeH = sevSize.y + Metrics::Spacing8;
        float badgeY = pos.y + (rowH - badgeH) * 0.5f;

        // Badge background
        ImVec4 badgeBg = ImVec4(severityColor.x, severityColor.y, severityColor.z, 0.15f);
        dl->AddRectFilled(
            ImVec2(badgeX, badgeY),
            ImVec2(badgeX + badgeW, badgeY + badgeH),
            ImGui::GetColorU32(badgeBg), Radii::Input);

        // Badge text
        dl->AddText(
            ImVec2(badgeX + Metrics::Spacing8, badgeY + Metrics::Spacing4),
            ImGui::GetColorU32(severityColor), severity);

        // Message
        float msgX = badgeX + badgeW + Metrics::Spacing16;
        dl->AddText(ImVec2(msgX, textY), ImGui::GetColorU32(Colors::Text), message);

        ImGui::Dummy(ImVec2(0, rowH));
    }

    // ============================================================
    // MetricCard - Performance metric card
    // ============================================================
    void MetricCard(const char* label, const char* value, const char* unit, ImVec4 color, float width)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float cardH = 76.0f;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Card background with subtle border
        dl->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + cardH),
            ImGui::GetColorU32(Colors::Surface), Radii::Card);
        dl->AddRect(pos, ImVec2(pos.x + width, pos.y + cardH),
            ImGui::GetColorU32(Colors::BorderSubtle), Radii::Card);

        // Top color accent line — 2px
        dl->AddRectFilled(
            ImVec2(pos.x + Radii::Card, pos.y),
            ImVec2(pos.x + width - Radii::Card, pos.y + 2.0f),
            ImGui::GetColorU32(color));

        // Label
        dl->AddText(ImVec2(pos.x + Metrics::Spacing16, pos.y + Metrics::Spacing12),
            ImGui::GetColorU32(Colors::TextDim), label);

        // Value
        ImVec2 valueSize = ImGui::CalcTextSize(value);
        float valueY = pos.y + cardH - valueSize.y - Metrics::Spacing16;
        dl->AddText(ImVec2(pos.x + Metrics::Spacing16, valueY),
            ImGui::GetColorU32(color), value);

        // Unit
        if (unit && unit[0] != '\0') {
            ImVec2 unitSize = ImGui::CalcTextSize(unit);
            dl->AddText(ImVec2(pos.x + Metrics::Spacing16 + valueSize.x + Metrics::Spacing6, valueY + (valueSize.y - unitSize.y) * 0.5f),
                ImGui::GetColorU32(Colors::TextMuted), unit);
        }

        ImGui::Dummy(ImVec2(0, cardH));
    }

}
}
