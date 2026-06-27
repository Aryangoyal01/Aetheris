#pragma once

#include "raylib.h"
#include "ui_layout.h"

namespace UIWidgets {

struct InteractionState {
    bool hovered;
    bool pressed;
    bool justClicked;
};

InteractionState GetInteractionState(Rectangle rect, Vector2 cursorPos, bool inputActive, bool inputJustPressed);

void DrawPanelBackground(const UILayout::LayoutRect& rect);
void DrawPanelBorder(const UILayout::LayoutRect& rect);

bool DrawTab(const UILayout::TabLayout& layout, const char* text, bool active, const InteractionState& state, bool pinchActive);

void DrawSlider(const UILayout::SliderLayout& layout, const char* label, float value, float minVal, float maxVal, const InteractionState& state);

bool DrawEntityButton(const UILayout::EntityButtonLayout& layout, const char* label, Color accentColor, bool selected, const InteractionState& state, bool pinchActive);

void DrawLabel(float x, float y, const char* text, int fontSize, Color color);

void DrawStatusIndicator(float x, float y, const char* text, bool active);

bool DrawToolbarButton(float x, float y, float width, float height, const char* text, const InteractionState& state);

bool DrawSidebarTab(float x, float y, float width, float height, const char* text, bool active, const InteractionState& state);

void DrawSectionHeader(float x, float y, float width, const char* text);

void DrawRegionBackground(const UILayout::LayoutRect& rect, Color bgColor);

}
