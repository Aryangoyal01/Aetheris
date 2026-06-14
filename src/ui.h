#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "settings.h"

class EngineUI {
public:
    static const int PANEL_WIDTH = 450;

    // Added 'isPinching' to the Draw function for reactive visuals
    static void DrawDashboard(EngineSettings& settings, Vector2 handPos, bool isHandVisible, bool isNetworkOnline, bool isPinching);

    static void HandleSpatialInteractivity(EngineSettings& settings, Vector2 handPos, bool isPinching, bool isHandVisible);
};

#endif // UI_H