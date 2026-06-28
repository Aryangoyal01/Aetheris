#include "widgets.h"
#include "colors.h"
#include "theme.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <extras/IconsFontAwesome6.h>
#include <cstring>

namespace Theme {
namespace Widgets {

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
    // SectionHeader - Purple title with subtle separator
    // ============================================================
    void SectionHeader(const char* title, ImFont* font)
    {
        if (font) ImGui::PushFont(font);
        ImGui::TextColored(Colors::AccentPurple, "%s", title);
        if (font) ImGui::PopFont();

        ImGui::PushStyleColor(ImGuiCol_Separator, Colors::BorderSubtle);
        ImGui::Separator();
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0.0f, Metrics::Spacing8));
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
        ImVec4 titleColor = isOpen ? Colors::Text : Colors::TextDim;
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
    bool QuickAction(const char* id, const char* icon, const char* label, ImVec2 size)
    {
        bool clicked = false;
        if (size.x == 0) size.x = ImGui::GetContentRegionAvail().x;
        if (size.y == 0) size.y = 40.0f;

        ImGui::PushID(id);
        ImVec2 pos = ImGui::GetCursorScreenPos();

        ImGui::InvisibleButton("##btn", size);
        bool isHovered = ImGui::IsItemHovered();
        bool isHeld = ImGui::IsItemActive();
        if (ImGui::IsItemClicked()) clicked = true;

        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Background with smooth state transitions
        ImU32 bgCol = ImGui::GetColorU32(Colors::Surface);
        if (isHeld) {
            bgCol = ImGui::GetColorU32(Colors::AccentCyanDim);
        } else if (isHovered) {
            bgCol = ImGui::GetColorU32(Colors::SurfaceHover);
        }

        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), bgCol, Radii::Button);

        // Subtle border
        ImVec4 borderColor = isHovered ? Colors::BorderStrong : Colors::Border;
        dl->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y),
            ImGui::GetColorU32(borderColor), Radii::Button);

        // Icon + Label centered
        ImVec2 iconSize = ImGui::CalcTextSize(icon);
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        float totalW = iconSize.x + Metrics::Spacing10 + labelSize.x;
        float startX = pos.x + (size.x - totalW) * 0.5f;
        float centerY = pos.y + (size.y - iconSize.y) * 0.5f;

        ImVec4 iconColor = isHovered ? Colors::Text : Colors::TextDim;
        ImVec4 textColor = isHovered ? Colors::Text : Colors::Text;

        dl->AddText(ImVec2(startX, centerY), ImGui::GetColorU32(iconColor), icon);
        dl->AddText(ImVec2(startX + iconSize.x + Metrics::Spacing10, centerY),
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
