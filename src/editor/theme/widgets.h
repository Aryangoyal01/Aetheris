#pragma once

#include <cstdint>
#include <imgui.h>
#include <string>

namespace Theme {
namespace Widgets {

// Helper to format large numbers with commas
std::string FormatNumber(uint32_t value);

// A unified button for the floating toolbar and top toolbar
bool ToolbarButton(const char *id, const char *label, bool isActive = false,
                   ImVec2 size = ImVec2(0, 0));

// An item in the left navigation rail
bool SidebarItem(const char *id, const char *icon, const char *label,
                 bool isActive, ImFont *font = nullptr);

// A chip widget for status/telemetry (non-interactive)
void StatusChip(const char *label, const char *value, ImVec4 color);

// A standard section header with font support
void SectionHeader(const char *title, ImFont *font);

// A property row for the Inspector (Label on left, value on right aligned)
void PropertyRow(const char *label, const char *value);

// A toggle switch widget
bool ToggleSwitch(const char *id, bool &value, ImVec2 size = ImVec2(0, 0));

// A collapsible section with custom styling
bool CollapsibleSection(const char *id, const char *title,
                        bool defaultOpen = true, ImFont *font = nullptr);

// A styled input field displaying a value
void PropertyValue(const char *label, const char *value,
                   const char *unit = nullptr);

// A dropdown selector
void PropertyDropdown(const char *label, const char *value);

void Vector3Property(const char *label, const char *x, const char *y,
                     const char *z, const char *unit = nullptr);

// A fully featured Vector2 property with Cartesian and Polar editing, and an
// interactive gizmo
bool Vector2Control(const char *label, float *v, float resetValue = 0.0f,
                    bool disabled = false, const char *unit = "");

// A quick action button with icon
bool QuickAction(const char *id, const char *icon, const char *label,
                 ImVec2 size = ImVec2(0, 0), bool isPrimary = true);

// A console log entry with timestamp and severity badge
void ConsoleEntry(const char *timestamp, const char *severity,
                  ImVec4 severityColor, const char *message,
                  ImFont *badgeFont = nullptr);

// A performance metric card
void MetricCard(const char *label, const char *value, const char *unit,
                ImVec4 color, float width);

} // namespace Widgets
} // namespace Theme
