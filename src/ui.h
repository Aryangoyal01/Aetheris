#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "settings.h"
#include "ui_layout.h"

class EngineUI {
public:
    static const int PANEL_WIDTH = 220;

    static UILayout::EditorRegions GetEditorRegions();

    static void DrawEditor(EngineSettings& settings, Vector2 handPos, bool isHandVisible, bool isNetworkOnline, bool isPinching, int gpuParticleCount, int fps);

    static void HandleSpatialInteractivity(EngineSettings& settings, Vector2 handPos, bool isPinching, bool isHandVisible);
};

#endif // UI_H
