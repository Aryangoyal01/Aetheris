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

}
