#pragma once

#include "raylib.h"

namespace UILayout {

constexpr int TOOLBAR_HEIGHT = 40;
constexpr int LEFT_SIDEBAR_WIDTH = 220;
constexpr int RIGHT_INSPECTOR_WIDTH = 280;
constexpr int BOTTOM_DOCK_HEIGHT = 200;

constexpr int MARGIN = 12;
constexpr int INNER_MARGIN = 16;

constexpr int TITLE_Y = 40;
constexpr int SUBTITLE_Y = 70;

constexpr int TAB_Y = 110;
constexpr int TAB_WIDTH = 160;
constexpr int TAB_HEIGHT = 45;
constexpr int TAB_SPACING = 180;
constexpr int TAB_LABEL_OFFSET_X = 35;
constexpr int TAB_LABEL_OFFSET_Y = 16;
constexpr int TAB_TEXT_SIZE = 14;

constexpr int DIVIDER_Y = 175;

constexpr int SECTION_HEADER_Y = 200;
constexpr int SECTION_HEADER_SIZE = 12;

constexpr int SLIDER_START_Y = 240;
constexpr int SLIDER_ROW_HEIGHT = 70;
constexpr int SLIDER_LABEL_SIZE = 13;
constexpr int SLIDER_TRACK_HEIGHT = 6;
constexpr int SLIDER_TRACK_Y_OFFSET = 25;
constexpr int SLIDER_THUMB_RADIUS = 8;
constexpr int SLIDER_THUMB_RADIUS_HOVER = 10;
constexpr int SLIDER_HIT_Y_OFFSET = -5;
constexpr int SLIDER_HIT_HEIGHT = 40;

constexpr int ENTITY_START_Y = 240;
constexpr int ENTITY_ROW_HEIGHT = 65;
constexpr int ENTITY_HEIGHT = 50;
constexpr int ENTITY_LABEL_SIZE = 14;
constexpr int ENTITY_LABEL_OFFSET_X = 20;
constexpr int ENTITY_LABEL_OFFSET_Y = 18;

constexpr float CORNER_RADIUS = 0.2f;
constexpr int CORNER_SEGMENTS = 8;

struct LayoutRect {
    float x, y, width, height;
};

struct SliderLayout {
    LayoutRect labelRect;
    LayoutRect trackRect;
    LayoutRect hitRect;
};

struct TabLayout {
    LayoutRect rect;
};

struct EntityButtonLayout {
    LayoutRect rect;
};

struct EditorRegions {
    LayoutRect toolbar;
    LayoutRect leftSidebar;
    LayoutRect viewport;
    LayoutRect rightInspector;
    LayoutRect bottomDock;
};

EditorRegions ComputeEditorRegions(int screenWidth, int screenHeight);

LayoutRect GetPanelRect();
LayoutRect GetTabRect(int tabIndex);
LayoutRect GetDividerRect();
LayoutRect GetSectionHeaderRect(int yOffset);
SliderLayout GetSliderLayout(int index);
EntityButtonLayout GetEntityButtonLayout(int index);

}
