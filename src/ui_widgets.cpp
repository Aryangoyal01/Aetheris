#include "ui_widgets.h"
#include "raymath.h"

namespace UIWidgets {

InteractionState GetInteractionState(Rectangle rect, Vector2 cursorPos, bool inputActive, bool inputJustPressed) {
    InteractionState state;
    state.hovered = CheckCollisionPointRec(cursorPos, rect);
    state.pressed = state.hovered && inputActive;
    state.justClicked = state.hovered && inputJustPressed;
    return state;
}

void DrawPanelBackground(const UILayout::LayoutRect& rect) {
    DrawRectangleGradientEx(
        {rect.x, rect.y, rect.width, rect.height},
        GetColor(0x0A0A12FA),
        GetColor(0x05050AFA),
        GetColor(0x05050AFA),
        GetColor(0x0A0A12FA)
    );
}

void DrawPanelBorder(const UILayout::LayoutRect& rect) {
    DrawRectangle((int)(rect.x + rect.width - 2), 0, 2, (int)rect.height, GetColor(0x00D2FF44));
}

bool DrawTab(const UILayout::TabLayout& layout, const char* text, bool active, const InteractionState& state, bool pinchActive) {
    Rectangle drawRect = {layout.rect.x, layout.rect.y, layout.rect.width, layout.rect.height};
    float expand = (state.hovered && pinchActive) ? -2.0f : 0.0f;

    Rectangle visualRect = {drawRect.x - expand, drawRect.y - expand, drawRect.width + (expand * 2), drawRect.height + (expand * 2)};

    Color baseCol = active ? GetColor(0x00D2FFFF) : (state.hovered ? RAYWHITE : Fade(RAYWHITE, 0.3f));

    DrawRectangleRounded(visualRect, UILayout::CORNER_RADIUS, UILayout::CORNER_SEGMENTS,
        active ? Fade(baseCol, 0.15f) : (state.hovered ? Fade(baseCol, 0.05f) : BLANK));
    DrawRectangleRoundedLines(visualRect, UILayout::CORNER_RADIUS, UILayout::CORNER_SEGMENTS, baseCol);
    DrawText(text, (int)layout.rect.x + UILayout::TAB_LABEL_OFFSET_X,
             (int)layout.rect.y + UILayout::TAB_LABEL_OFFSET_Y,
             UILayout::TAB_TEXT_SIZE, baseCol);

    return state.justClicked;
}

void DrawSlider(const UILayout::SliderLayout& layout, const char* label, float value, float minVal, float maxVal, const InteractionState& state) {
    float pct = (value - minVal) / (maxVal - minVal);
    Color accent = state.hovered ? GetColor(0x00F0FFFF) : GetColor(0x00D2FFFF);

    DrawText(TextFormat("%s: %0.2f", label, value), (int)layout.labelRect.x, (int)layout.labelRect.y,
             UILayout::SLIDER_LABEL_SIZE, state.hovered ? RAYWHITE : Fade(RAYWHITE, 0.7f));
    DrawRectangle((int)layout.trackRect.x, (int)layout.trackRect.y, (int)layout.trackRect.width, (int)layout.trackRect.height,
                  GetColor(0x202035FF));
    DrawRectangle((int)layout.trackRect.x, (int)layout.trackRect.y, (int)(layout.trackRect.width * pct), (int)layout.trackRect.height, accent);

    float thumbRadius = state.hovered ? (float)UILayout::SLIDER_THUMB_RADIUS_HOVER : (float)UILayout::SLIDER_THUMB_RADIUS;
    DrawCircle((int)(layout.trackRect.x + layout.trackRect.width * pct),
               (int)(layout.trackRect.y + layout.trackRect.height / 2), thumbRadius, accent);
}

bool DrawEntityButton(const UILayout::EntityButtonLayout& layout, const char* label, Color accentColor, bool selected, const InteractionState& state, bool pinchActive) {
    Rectangle drawRect = {layout.rect.x, layout.rect.y, layout.rect.width, layout.rect.height};
    float expand = (state.hovered && pinchActive) ? -2.0f : 0.0f;

    Rectangle visualRect = {drawRect.x - expand, drawRect.y - expand, drawRect.width + (expand * 2), drawRect.height + (expand * 2)};

    DrawRectangleRounded(visualRect, UILayout::CORNER_RADIUS, UILayout::CORNER_SEGMENTS,
        selected ? Fade(accentColor, 0.25f) : (state.hovered ? Fade(RAYWHITE, 0.1f) : Fade(RAYWHITE, 0.02f)));
    DrawRectangleRoundedLines(visualRect, UILayout::CORNER_RADIUS, UILayout::CORNER_SEGMENTS,
        selected ? accentColor : (state.hovered ? RAYWHITE : BLANK));
    DrawText(label, (int)layout.rect.x + UILayout::ENTITY_LABEL_OFFSET_X,
             (int)layout.rect.y + UILayout::ENTITY_LABEL_OFFSET_Y,
             UILayout::ENTITY_LABEL_SIZE,
             selected ? accentColor : (state.hovered ? RAYWHITE : Fade(RAYWHITE, 0.5f)));

    return state.justClicked;
}

void DrawLabel(float x, float y, const char* text, int fontSize, Color color) {
    DrawText(text, (int)x, (int)y, fontSize, color);
}

void DrawStatusIndicator(float x, float y, const char* text, bool active) {
    Color col = active ? GetColor(0x00FF88FF) : GetColor(0x88888888);
    DrawCircle((int)x + 4, (int)y + 6, 4.0f, col);
    DrawText(text, (int)x + 14, (int)y, 12, col);
}

}
