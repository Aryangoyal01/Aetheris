#include "ui.h"
#include "raylib.h"
#include "raymath.h"

const int MARGIN = 40;

void EngineUI::DrawDashboard(EngineSettings& settings, Vector2 handPos, bool isHandVisible, bool isNetworkOnline, bool isPinching) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();

    // 1. Sleek "Obsidian" Panel
    DrawRectangleGradientEx(Rectangle{0, 0, (float)EngineUI::PANEL_WIDTH, (float)GetScreenHeight()}, 
        GetColor(0x0A0A12FA), GetColor(0x05050AFA), GetColor(0x05050AFA), GetColor(0x0A0A12FA));
    DrawRectangle(EngineUI::PANEL_WIDTH - 2, 0, 2, GetScreenHeight(), GetColor(0x00D2FF44));

    // 2. Branding
    DrawText("AETHERIS // MATRIX", MARGIN, 40, 24, GetColor(0xE0F0FFFF));
    DrawText("SPATIAL TELEMETRY LINK", MARGIN, 70, 10, GetColor(0x00D2FF88));

    // Helper: UI Hover Detection
    auto IsHovered = [&](Rectangle rect) {
        return CheckCollisionPointRec(activePos, rect);
    };

    // 3. Reactive Tab Selection
    auto DrawTab = [&](int x, int y, const char* text, bool active) {
        Rectangle tabRect = {(float)x, (float)y, 160, 45};
        bool hovered = IsHovered(tabRect);
        
        Color baseCol = active ? GetColor(0x00D2FFFF) : (hovered ? RAYWHITE : Fade(RAYWHITE, 0.3f));
        float expand = (hovered && isPinching) ? -2.0f : 0.0f; // Shrink click effect
        
        Rectangle drawRect = {tabRect.x - expand, tabRect.y - expand, tabRect.width + (expand*2), tabRect.height + (expand*2)};
        
        DrawRectangleRounded(drawRect, 0.2f, 8, active ? Fade(baseCol, 0.15f) : (hovered ? Fade(baseCol, 0.05f) : BLANK));
        DrawRectangleRoundedLines(drawRect, 0.2f, 8, baseCol);
        DrawText(text, x + 35, y + 16, 14, baseCol);
    };

    DrawTab(MARGIN, 110, "PHYSICS", settings.activeTab == TAB_PARAMETERS);
    DrawTab(MARGIN + 180, 110, "ENTITIES", settings.activeTab == TAB_ENTITIES);
    DrawLine(MARGIN, 175, EngineUI::PANEL_WIDTH - MARGIN, 175, Fade(RAYWHITE, 0.1f));

    // 4. Content Area
    if (settings.activeTab == TAB_PARAMETERS) {
        DrawText("SYSTEM DYNAMICS", MARGIN, 200, 12, GetColor(0x00D2FF88));

        auto DrawSlider = [&](int y, const char* label, float value, float min, float max) {
            float pct = (value - min) / (max - min);
            Rectangle sliderRect = {(float)MARGIN, (float)y + 15, EngineUI::PANEL_WIDTH - (MARGIN * 2.0f), 20};
            bool hovered = IsHovered(sliderRect);
            Color accent = hovered ? GetColor(0x00F0FFFF) : GetColor(0x00D2FFFF);

            DrawText(TextFormat("%s: %0.2f", label, value), MARGIN, y, 13, hovered ? RAYWHITE : Fade(RAYWHITE, 0.7f));
            DrawRectangle(MARGIN, y + 25, EngineUI::PANEL_WIDTH - (MARGIN * 2), 6, GetColor(0x202035FF)); 
            DrawRectangle(MARGIN, y + 25, (int)((EngineUI::PANEL_WIDTH - (MARGIN * 2)) * pct), 6, accent); 
            DrawCircle(MARGIN + (int)((EngineUI::PANEL_WIDTH - (MARGIN * 2)) * pct), y + 28, hovered ? 10.0f : 8.0f, accent); 
        };

        DrawSlider(240, "GRAVITY", settings.gravityStrength, -20.0f, 20.0f);
        DrawSlider(310, "FRICTION", settings.frictionCoefficient, 0.0f, 1.0f);
        DrawSlider(380, "ELASTICITY", settings.elasticityValue, 0.0f, 1.0f);

    } else if (settings.activeTab == TAB_ENTITIES) {
        DrawText("MATTER SPECIFICATION", MARGIN, 200, 12, GetColor(0x00D2FF88));
        const char* labels[] = { "SINGULARITY WELL", "RECURSIVE REPELLER", "ORBITAL PULSAR" };
        Color colors[] = { SKYBLUE, PURPLE, GOLD };

        for (int i = 0; i < 3; i++) {
            int y = 240 + (i * 65);
            Rectangle btnRect = {(float)MARGIN, (float)y, EngineUI::PANEL_WIDTH - (MARGIN*2.0f), 50};
            bool hovered = IsHovered(btnRect);
            bool selected = (settings.activeObjectType == i);
            
            float expand = (hovered && isPinching) ? -2.0f : 0.0f;
            Rectangle drawRect = {btnRect.x - expand, btnRect.y - expand, btnRect.width + (expand*2), btnRect.height + (expand*2)};

            DrawRectangleRounded(drawRect, 0.2f, 8, selected ? Fade(colors[i], 0.25f) : (hovered ? Fade(RAYWHITE, 0.1f) : Fade(RAYWHITE, 0.02f)));
            DrawRectangleRoundedLines(drawRect, 0.2f, 8, selected ? colors[i] : (hovered ? RAYWHITE : BLANK));
            DrawText(labels[i], MARGIN + 20, y + 18, 14, selected ? colors[i] : (hovered ? RAYWHITE : Fade(RAYWHITE, 0.5f)));
        }
    }

    // 5. Dynamic Reticle (Cursor)
    if (activePos.x > 0) {
        Color cursorCol = isPinching ? GetColor(0x00FF88FF) : GetColor(0x00D2FFFF);
        float innerRad = isPinching ? 6.0f : 4.0f;
        float outerRad = isPinching ? 10.0f : 16.0f; // Snaps inward on pinch
        
        DrawCircleV(activePos, innerRad, cursorCol);
        DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad, Fade(cursorCol, 0.8f));
        if (isPinching) DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad + 4.0f, Fade(cursorCol, 0.3f));
    }
}

void EngineUI::HandleSpatialInteractivity(EngineSettings& settings, Vector2 handPos, bool isPinching, bool isHandVisible) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();
    bool activeGrip = isHandVisible ? isPinching : IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool activeTriggered = isHandVisible ? isPinching : IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    static bool wasTriggered = false;
    bool justClicked = (activeTriggered && !wasTriggered);
    wasTriggered = activeTriggered;

    if (justClicked && activePos.y >= 110 && activePos.y <= 155) {
        if (activePos.x >= MARGIN && activePos.x <= MARGIN + 160) settings.activeTab = TAB_PARAMETERS;
        if (activePos.x >= MARGIN + 180 && activePos.x <= MARGIN + 340) settings.activeTab = TAB_ENTITIES;
    }

    if (activePos.x > 0 && activePos.x < EngineUI::PANEL_WIDTH) {
        if (settings.activeTab == TAB_PARAMETERS && activeGrip) {
            float relativeX = activePos.x - MARGIN;
            float sliderWidth = EngineUI::PANEL_WIDTH - (MARGIN * 2);
            float pct = Clamp(relativeX / sliderWidth, 0.0f, 1.0f);

            if (activePos.y >= 235 && activePos.y <= 275) settings.gravityStrength = Lerp(-20.0f, 20.0f, pct);
            if (activePos.y >= 305 && activePos.y <= 345) settings.frictionCoefficient = pct;
            if (activePos.y >= 375 && activePos.y <= 415) settings.elasticityValue = pct;
        } 
        else if (settings.activeTab == TAB_ENTITIES && justClicked) {
            for (int i = 0; i < 3; i++) {
                int itemY = 240 + (i * 65);
                if (activePos.y >= itemY && activePos.y < itemY + 50) settings.activeObjectType = i;
            }
        }
    }
}