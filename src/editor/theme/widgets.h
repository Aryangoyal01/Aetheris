#pragma once

#include <imgui.h>
#include <string>

namespace Theme {
namespace Widgets {

    // A unified button for the floating toolbar and top toolbar
    bool ToolbarButton(const char* id, const char* label, bool isActive = false, ImVec2 size = ImVec2(0, 0));

    // An item in the left navigation rail
    bool SidebarItem(const char* id, const char* icon, const char* label, bool isActive, ImFont* font = nullptr);

    // A chip widget for status/telemetry (non-interactive)
    void StatusChip(const char* label, const char* value, ImVec4 color);

    // A standard section header with font support
    void SectionHeader(const char* title, ImFont* font);

    // A property row for the Inspector (Label on left, value on right aligned)
    void PropertyRow(const char* label, const char* value);

    // A toggle switch widget
    bool ToggleSwitch(const char* id, bool& value, ImVec2 size = ImVec2(0, 0));

    // A collapsible section with custom styling
    bool CollapsibleSection(const char* id, const char* title, bool defaultOpen = true, ImFont* font = nullptr);

    // A styled input field displaying a value
    void PropertyValue(const char* label, const char* value, const char* unit = nullptr);

    // A dropdown selector
    void PropertyDropdown(const char* label, const char* value);

    // A 3-component vector input row (X, Y, Z)
    void Vector2Property(const char* label, const char* x, const char* y);
    void Vector3Property(const char* label, const char* x, const char* y, const char* z);

    // A quick action button with icon
    bool QuickAction(const char* id, const char* icon, const char* label, ImVec2 size = ImVec2(0, 0));

    // A console log entry with timestamp and severity badge
    void ConsoleEntry(const char* timestamp, const char* severity, ImVec4 severityColor, const char* message, ImFont* badgeFont = nullptr);

    // A performance metric card
    void MetricCard(const char* label, const char* value, const char* unit, ImVec4 color, float width);

}
}
