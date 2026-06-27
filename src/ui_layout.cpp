#include "ui_layout.h"

namespace UILayout {

EditorRegions ComputeEditorRegions(int screenWidth, int screenHeight) {
    EditorRegions regions;

    regions.toolbar = { 0.0f, 0.0f, (float)screenWidth, (float)TOOLBAR_HEIGHT };

    regions.leftSidebar = {
        0.0f,
        (float)TOOLBAR_HEIGHT,
        (float)LEFT_SIDEBAR_WIDTH,
        (float)(screenHeight - TOOLBAR_HEIGHT - BOTTOM_DOCK_HEIGHT)
    };

    regions.rightInspector = {
        (float)(screenWidth - RIGHT_INSPECTOR_WIDTH),
        (float)TOOLBAR_HEIGHT,
        (float)RIGHT_INSPECTOR_WIDTH,
        (float)(screenHeight - TOOLBAR_HEIGHT - BOTTOM_DOCK_HEIGHT)
    };

    regions.viewport = {
        (float)LEFT_SIDEBAR_WIDTH,
        (float)TOOLBAR_HEIGHT,
        (float)(screenWidth - LEFT_SIDEBAR_WIDTH - RIGHT_INSPECTOR_WIDTH),
        (float)(screenHeight - TOOLBAR_HEIGHT - BOTTOM_DOCK_HEIGHT)
    };

    regions.bottomDock = {
        0.0f,
        (float)(screenHeight - BOTTOM_DOCK_HEIGHT),
        (float)screenWidth,
        (float)BOTTOM_DOCK_HEIGHT
    };

    return regions;
}

LayoutRect GetPanelRect() {
    return { 0.0f, 0.0f, (float)LEFT_SIDEBAR_WIDTH, (float)GetScreenHeight() };
}

LayoutRect GetTabRect(int tabIndex) {
    return {
        (float)(MARGIN + tabIndex * TAB_SPACING),
        (float)TAB_Y,
        (float)TAB_WIDTH,
        (float)TAB_HEIGHT
    };
}

LayoutRect GetDividerRect() {
    return {
        (float)MARGIN,
        (float)DIVIDER_Y,
        (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2),
        1.0f
    };
}

LayoutRect GetSectionHeaderRect(int yOffset) {
    return {
        (float)MARGIN,
        (float)(SECTION_HEADER_Y + yOffset),
        (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2),
        16.0f
    };
}

SliderLayout GetSliderLayout(int index) {
    float y = (float)(SLIDER_START_Y + index * SLIDER_ROW_HEIGHT);

    LayoutRect label = { (float)MARGIN, y, (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2), 20.0f };
    LayoutRect track = {
        (float)MARGIN,
        y + SLIDER_TRACK_Y_OFFSET,
        (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2),
        (float)SLIDER_TRACK_HEIGHT
    };
    LayoutRect hit = {
        (float)MARGIN,
        y + SLIDER_HIT_Y_OFFSET,
        (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2),
        (float)SLIDER_HIT_HEIGHT
    };

    return { label, track, hit };
}

EntityButtonLayout GetEntityButtonLayout(int index) {
    float y = (float)(ENTITY_START_Y + index * ENTITY_ROW_HEIGHT);
    return {
        {
            (float)MARGIN,
            y,
            (float)(LEFT_SIDEBAR_WIDTH - MARGIN * 2),
            (float)ENTITY_HEIGHT
        }
    };
}

}
