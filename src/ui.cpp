#include "ui.h"
#include "ui_layout.h"
#include "ui_widgets.h"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

static const char* sidebarTabNames[] = { "PHY", "PAR", "ENV", "PRF", "AI", "SCN" };
static const char* dockTabNames[] = { "CONSOLE", "LOG", "PERF" };

UILayout::EditorRegions EngineUI::GetEditorRegions() {
    return UILayout::ComputeEditorRegions(GetScreenWidth(), GetScreenHeight());
}

void EngineUI::DrawEditor(EngineSettings& settings, Vector2 handPos, bool isHandVisible, bool isNetworkOnline, bool isPinching, int gpuParticleCount, int fps) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();
    bool inputActive = isHandVisible ? isPinching : IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool inputJustPressed = isHandVisible ? isPinching : IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    auto regions = UILayout::ComputeEditorRegions(GetScreenWidth(), GetScreenHeight());

    UIWidgets::DrawRegionBackground(regions.leftSidebar, GetColor(0x08080EFA));
    UIWidgets::DrawRegionBackground(regions.rightInspector, GetColor(0x08080EFA));
    UIWidgets::DrawRegionBackground(regions.bottomDock, GetColor(0x06060CFA));

    UIWidgets::DrawRegionBackground(regions.toolbar, GetColor(0x0C0C16FA));
    DrawLine(0, UILayout::TOOLBAR_HEIGHT, GetScreenWidth(), UILayout::TOOLBAR_HEIGHT, GetColor(0x00D2FF22));

    UIWidgets::DrawLabel(12.0f, 12.0f, "AETHERIS", 16, GetColor(0x00D2FFFF));

    const char* statusText = isNetworkOnline ? "LINK ACTIVE" : "LINK OFFLINE";
    Color statusColor = isNetworkOnline ? GetColor(0x00FF88FF) : GetColor(0xFF4444FF);
    UIWidgets::DrawLabel((float)(GetScreenWidth() - 220), 14.0f, statusText, 11, statusColor);

    const char* fpsText = TextFormat("FPS %d", fps);
    UIWidgets::DrawLabel((float)(GetScreenWidth() - 120), 14.0f, fpsText, 11, Fade(RAYWHITE, 0.5f));

    const char* particleText = TextFormat("Alive %d", gpuParticleCount);
    UIWidgets::DrawLabel((float)(GetScreenWidth() - 50), 14.0f, particleText, 11, Fade(RAYWHITE, 0.5f));

    float sidebarContentX = regions.leftSidebar.x + 42.0f;
    float sidebarContentWidth = regions.leftSidebar.width - 54.0f;
    float sidebarContentY = regions.leftSidebar.y + 8.0f;
    float sidebarContentHeight = regions.leftSidebar.height - 16.0f;

    float tabX = regions.leftSidebar.x + 4.0f;
    float tabWidth = 36.0f;
    float tabHeight = 30.0f;
    float tabSpacing = 34.0f;

    for (int i = 0; i < SIDEBAR_COUNT; i++) {
        float tabY = sidebarContentY + i * tabSpacing;
        auto state = UIWidgets::GetInteractionState(
            { tabX, tabY, tabWidth, tabHeight },
            activePos, inputActive, inputJustPressed);

        if (UIWidgets::DrawSidebarTab(tabX, tabY, tabWidth, tabHeight,
                                       sidebarTabNames[i], settings.activeSidebarTab == i, state)) {
            settings.activeSidebarTab = (SidebarTab)i;
        }
    }

    float contentY = sidebarContentY + 4.0f;

    UIWidgets::DrawSectionHeader(sidebarContentX, contentY, sidebarContentWidth, sidebarTabNames[settings.activeSidebarTab]);
    contentY += 28.0f;

    if (settings.activeSidebarTab == SIDEBAR_PHYSICS) {
        const char* sliderLabels[] = { "GRAVITY", "FRICTION", "ELASTICITY" };
        float sliderMins[] = { -20.0f, 0.0f, 0.0f };
        float sliderMaxs[] = { 20.0f, 1.0f, 1.0f };

        for (int i = 0; i < 3; i++) {
            float sliderY = contentY + i * 62.0f;

            UILayout::SliderLayout sliderLayout;
            sliderLayout.labelRect = { sidebarContentX, sliderY, sidebarContentWidth, 18.0f };
            sliderLayout.trackRect = { sidebarContentX, sliderY + 22.0f, sidebarContentWidth, 5.0f };
            sliderLayout.hitRect = { sidebarContentX, sliderY - 4.0f, sidebarContentWidth, 36.0f };

            auto state = UIWidgets::GetInteractionState(
                { sliderLayout.hitRect.x, sliderLayout.hitRect.y, sliderLayout.hitRect.width, sliderLayout.hitRect.height },
                activePos, inputActive, inputJustPressed);

            float currentVal = (i == 0) ? settings.gravityStrength :
                               (i == 1) ? settings.frictionCoefficient :
                                          settings.elasticityValue;
            UIWidgets::DrawSlider(sliderLayout, sliderLabels[i], currentVal,
                                  sliderMins[i], sliderMaxs[i], state);
        }
    } else if (settings.activeSidebarTab == SIDEBAR_PARTICLES) {
        UIWidgets::DrawSectionHeader(sidebarContentX, contentY, sidebarContentWidth, "SPAWN TYPE");
        contentY += 28.0f;

        const char* entityLabels[] = { "SINGULARITY WELL", "RECURSIVE REPELLER", "ORBITAL PULSAR" };
        Color entityColors[] = { SKYBLUE, PURPLE, GOLD };

        for (int i = 0; i < 3; i++) {
            float btnY = contentY + i * 42.0f;

            UILayout::EntityButtonLayout btnLayout;
            btnLayout.rect = { sidebarContentX, btnY, sidebarContentWidth, 36.0f };

            auto state = UIWidgets::GetInteractionState(
                { btnLayout.rect.x, btnLayout.rect.y, btnLayout.rect.width, btnLayout.rect.height },
                activePos, inputActive, inputJustPressed);

            if (UIWidgets::DrawEntityButton(btnLayout, entityLabels[i], entityColors[i],
                                            settings.activeObjectType == i, state, isPinching)) {
                settings.activeObjectType = i;
            }
        }
    } else if (settings.activeSidebarTab == SIDEBAR_ENVIRONMENT) {
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Grid: 60x2.0 spacing", 12, Fade(RAYWHITE, 0.5f));
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Background: Dark", 12, Fade(RAYWHITE, 0.5f));
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Camera: Perspective", 12, Fade(RAYWHITE, 0.5f));
    } else if (settings.activeSidebarTab == SIDEBAR_PERFORMANCE) {
        UIWidgets::DrawLabel(sidebarContentX, contentY, TextFormat("FPS: %d", fps), 12, RAYWHITE);
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, TextFormat("Alive: %d", gpuParticleCount), 12, RAYWHITE);
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, TextFormat("Time Scale: %.2f", settings.timeScale), 12, RAYWHITE);
    } else if (settings.activeSidebarTab == SIDEBAR_AI) {
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Pipeline Status", 11, GetColor(0x00D2FF88));
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Active", 12, GetColor(0x00FF88FF));
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, "Model: Gemini", 12, Fade(RAYWHITE, 0.5f));
    } else if (settings.activeSidebarTab == SIDEBAR_SCENE) {
        UIWidgets::DrawLabel(sidebarContentX, contentY, "World State", 11, GetColor(0x00D2FF88));
        contentY += 20.0f;
        UIWidgets::DrawLabel(sidebarContentX, contentY, TextFormat("Particles: %d", gpuParticleCount), 12, Fade(RAYWHITE, 0.5f));
    }

    float inspectorContentX = regions.rightInspector.x + 14.0f;
    float inspectorContentWidth = regions.rightInspector.width - 28.0f;
    float inspectorContentY = regions.rightInspector.y + 14.0f;

    UIWidgets::DrawSectionHeader(inspectorContentX, inspectorContentY, inspectorContentWidth, "STATE");
    inspectorContentY += 28.0f;

    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("Alive: %d", gpuParticleCount), 12, RAYWHITE);
    inspectorContentY += 20.0f;
    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("FPS: %d", fps), 12, RAYWHITE);
    inspectorContentY += 20.0f;
    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("Gravity: %.2f", settings.gravityStrength), 12, Fade(RAYWHITE, 0.7f));
    inspectorContentY += 20.0f;
    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("Friction: %.2f", settings.frictionCoefficient), 12, Fade(RAYWHITE, 0.7f));
    inspectorContentY += 20.0f;
    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("Elasticity: %.2f", settings.elasticityValue), 12, Fade(RAYWHITE, 0.7f));
    inspectorContentY += 20.0f;
    UIWidgets::DrawLabel(inspectorContentX, inspectorContentY, TextFormat("Time Scale: %.2f", settings.timeScale), 12, Fade(RAYWHITE, 0.7f));
    inspectorContentY += 28.0f;

    UIWidgets::DrawSectionHeader(inspectorContentX, inspectorContentY, inspectorContentWidth, "INPUT");
    inspectorContentY += 28.0f;

    UIWidgets::DrawStatusIndicator(inspectorContentX, inspectorContentY, "Hand", isHandVisible);
    inspectorContentY += 20.0f;
    UIWidgets::DrawStatusIndicator(inspectorContentX, inspectorContentY, "Grip", isPinching);
    inspectorContentY += 20.0f;
    UIWidgets::DrawStatusIndicator(inspectorContentX, inspectorContentY, "Link", isNetworkOnline);

    float dockTabX = regions.bottomDock.x + 10.0f;
    float dockTabWidth = 80.0f;
    float dockTabHeight = 26.0f;
    float dockTabSpacing = 84.0f;
    float dockTabY = regions.bottomDock.y + 8.0f;

    for (int i = 0; i < DOCK_COUNT; i++) {
        float tabXPos = dockTabX + i * dockTabSpacing;
        auto state = UIWidgets::GetInteractionState(
            { tabXPos, dockTabY, dockTabWidth, dockTabHeight },
            activePos, inputActive, inputJustPressed);

        Rectangle tabRect = { tabXPos, dockTabY, dockTabWidth, dockTabHeight };
        Color tabBg = (settings.activeBottomDockTab == i) ? Fade(GetColor(0x00D2FFFF), 0.15f) : (state.hovered ? Fade(RAYWHITE, 0.05f) : BLANK);
        Color tabText = (settings.activeBottomDockTab == i) ? GetColor(0x00D2FFFF) : (state.hovered ? RAYWHITE : Fade(RAYWHITE, 0.5f));

        DrawRectangleRounded(tabRect, 0.3f, 6, tabBg);
        DrawText(dockTabNames[i], (int)tabXPos + 8, (int)dockTabY + 6, 11, tabText);

        if (state.justClicked) {
            settings.activeBottomDockTab = (BottomDockTab)i;
        }
    }

    float dockContentX = regions.bottomDock.x + 14.0f;
    float dockContentY = regions.bottomDock.y + 42.0f;
    float dockContentWidth = regions.bottomDock.width - 28.0f;

    if (settings.activeBottomDockTab == DOCK_CONSOLE) {
        UIWidgets::DrawLabel(dockContentX, dockContentY, "> aetheris engine v0.1", 11, GetColor(0x00FF88FF));
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, "> gpu pipeline active", 11, GetColor(0x00D2FFFF));
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, "> particle system ready", 11, Fade(RAYWHITE, 0.5f));
    } else if (settings.activeBottomDockTab == DOCK_LOGS) {
        UIWidgets::DrawLabel(dockContentX, dockContentY, "[info] engine initialized", 11, GetColor(0x00FF88FF));
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, "[info] compute shader loaded", 11, GetColor(0x00D2FFFF));
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, "[info] network listening on :8080", 11, Fade(RAYWHITE, 0.5f));
    } else if (settings.activeBottomDockTab == DOCK_PERFORMANCE) {
        UIWidgets::DrawLabel(dockContentX, dockContentY, TextFormat("Frame Time: %.2f ms", 1000.0f / fmaxf((float)fps, 1.0f)), 11, RAYWHITE);
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, TextFormat("Alive Particles: %d", gpuParticleCount), 11, RAYWHITE);
        dockContentY += 18.0f;
        UIWidgets::DrawLabel(dockContentX, dockContentY, TextFormat("Frame Rate: %d fps", fps), 11, RAYWHITE);
    }

    if (activePos.x > 0) {
        bool inViewport = (activePos.x >= regions.viewport.x &&
                          activePos.x <= regions.viewport.x + regions.viewport.width &&
                          activePos.y >= regions.viewport.y &&
                          activePos.y <= regions.viewport.y + regions.viewport.height);

        if (inViewport) {
            Color cursorCol = isPinching ? GetColor(0x00FF88FF) : GetColor(0x00D2FFFF);
            float innerRad = isPinching ? 6.0f : 4.0f;
            float outerRad = isPinching ? 10.0f : 16.0f;

            DrawCircleV(activePos, innerRad, cursorCol);
            DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad, Fade(cursorCol, 0.8f));
            if (isPinching) DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad + 4.0f, Fade(cursorCol, 0.3f));
        }
    }
}

void EngineUI::HandleSpatialInteractivity(EngineSettings& settings, Vector2 handPos, bool isPinching, bool isHandVisible) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();
    bool activeTriggered = isHandVisible ? isPinching : IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    static bool wasTriggered = false;
    bool justClicked = (activeTriggered && !wasTriggered);
    wasTriggered = activeTriggered;

    auto regions = UILayout::ComputeEditorRegions(GetScreenWidth(), GetScreenHeight());

    bool inputActive = isHandVisible ? isPinching : IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    bool inputJustPressed = isHandVisible ? isPinching : IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    float tabX = regions.leftSidebar.x + 4.0f;
    float tabWidth = 36.0f;
    float tabHeight = 30.0f;
    float tabSpacing = 34.0f;
    float sidebarContentY = regions.leftSidebar.y + 8.0f;

    if (justClicked) {
        for (int i = 0; i < SIDEBAR_COUNT; i++) {
            float tabY = sidebarContentY + i * tabSpacing;
            if (CheckCollisionPointRec(activePos, { tabX, tabY, tabWidth, tabHeight })) {
                settings.activeSidebarTab = (SidebarTab)i;
            }
        }

        float dockTabX = regions.bottomDock.x + 8.0f;
        float dockTabWidth = 80.0f;
        float dockTabHeight = 26.0f;
        float dockTabSpacing = 84.0f;
        float dockTabY = regions.bottomDock.y + 6.0f;

        for (int i = 0; i < DOCK_COUNT; i++) {
            float tabXPos = dockTabX + i * dockTabSpacing;
            if (CheckCollisionPointRec(activePos, { tabXPos, dockTabY, dockTabWidth, dockTabHeight })) {
                settings.activeBottomDockTab = (BottomDockTab)i;
            }
        }
    }

    bool inSidebar = (activePos.x >= regions.leftSidebar.x &&
                     activePos.x <= regions.leftSidebar.x + regions.leftSidebar.width &&
                     activePos.y >= regions.leftSidebar.y &&
                     activePos.y <= regions.leftSidebar.y + regions.leftSidebar.height);

    if (inSidebar && settings.activeSidebarTab == SIDEBAR_PHYSICS) {
        float sidebarContentX = regions.leftSidebar.x + 42.0f;
        float sidebarContentWidth = regions.leftSidebar.width - 54.0f;
        float contentY = regions.leftSidebar.y + 32.0f;

        bool activeGrip = isHandVisible ? isPinching : IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        for (int i = 0; i < 3; i++) {
            float sliderY = contentY + i * 62.0f;

            UILayout::SliderLayout sliderLayout;
            sliderLayout.labelRect = { sidebarContentX, sliderY, sidebarContentWidth, 18.0f };
            sliderLayout.trackRect = { sidebarContentX, sliderY + 22.0f, sidebarContentWidth, 5.0f };
            sliderLayout.hitRect = { sidebarContentX, sliderY - 4.0f, sidebarContentWidth, 36.0f };

            Rectangle hitRect = { sliderLayout.hitRect.x, sliderLayout.hitRect.y, sliderLayout.hitRect.width, sliderLayout.hitRect.height };

            if (activeGrip && CheckCollisionPointRec(activePos, hitRect)) {
                float relativeX = activePos.x - sliderLayout.trackRect.x;
                float sliderWidth = sliderLayout.trackRect.width;
                float pct = Clamp(relativeX / sliderWidth, 0.0f, 1.0f);

                if (i == 0) {
                    settings.gravityStrength = Lerp(-20.0f, 20.0f, pct);
                    settings.targetGravity = settings.gravityStrength;
                }
                else if (i == 1) {
                    settings.frictionCoefficient = pct;
                    settings.targetFriction = settings.frictionCoefficient;
                }
                else if (i == 2) {
                    settings.elasticityValue = pct;
                    settings.targetElasticity = settings.elasticityValue;
                }
            }
        }
    } else if (inSidebar && settings.activeSidebarTab == SIDEBAR_PARTICLES && justClicked) {
        float sidebarContentX = regions.leftSidebar.x + 42.0f;
        float sidebarContentWidth = regions.leftSidebar.width - 54.0f;
        float contentY = regions.leftSidebar.y + 56.0f;

        for (int i = 0; i < 3; i++) {
            float btnY = contentY + i * 42.0f;

            UILayout::EntityButtonLayout btnLayout;
            btnLayout.rect = { sidebarContentX, btnY, sidebarContentWidth, 36.0f };

            Rectangle hitRect = { btnLayout.rect.x, btnLayout.rect.y, btnLayout.rect.width, btnLayout.rect.height };

            if (CheckCollisionPointRec(activePos, hitRect)) {
                settings.activeObjectType = i;
            }
        }
    }
}
