#include "ui.h"
#include "ui_layout.h"
#include "ui_widgets.h"
#include "raylib.h"
#include "raymath.h"

void EngineUI::DrawDashboard(EngineSettings& settings, Vector2 handPos, bool isHandVisible, bool isNetworkOnline, bool isPinching) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();

    auto panelRect = UILayout::GetPanelRect();
    UIWidgets::DrawPanelBackground(panelRect);
    UIWidgets::DrawPanelBorder(panelRect);

    UIWidgets::DrawLabel((float)UILayout::MARGIN, (float)UILayout::TITLE_Y,
                         "AETHERIS // MATRIX", 24, GetColor(0xE0F0FFFF));
    UIWidgets::DrawLabel((float)UILayout::MARGIN, (float)UILayout::SUBTITLE_Y,
                         "SPATIAL TELEMETRY LINK", 10, GetColor(0x00D2FF88));

    auto tab0 = UILayout::GetTabRect(0);
    auto tab1 = UILayout::GetTabRect(1);
    UILayout::TabLayout tabLayout0 = { tab0 };
    UILayout::TabLayout tabLayout1 = { tab1 };

    auto tabState0 = UIWidgets::GetInteractionState({tab0.x, tab0.y, tab0.width, tab0.height}, activePos, false, false);
    auto tabState1 = UIWidgets::GetInteractionState({tab1.x, tab1.y, tab1.width, tab1.height}, activePos, false, false);

    UIWidgets::DrawTab(tabLayout0, "PHYSICS", settings.activeTab == TAB_PARAMETERS, tabState0, isPinching);
    UIWidgets::DrawTab(tabLayout1, "ENTITIES", settings.activeTab == TAB_ENTITIES, tabState1, isPinching);

    auto divider = UILayout::GetDividerRect();
    DrawLine((int)divider.x, (int)divider.y, (int)(divider.x + divider.width), (int)divider.y, Fade(RAYWHITE, 0.1f));

    if (settings.activeTab == TAB_PARAMETERS) {
        UIWidgets::DrawLabel((float)UILayout::MARGIN, (float)UILayout::SECTION_HEADER_Y,
                             "SYSTEM DYNAMICS", UILayout::SECTION_HEADER_SIZE, GetColor(0x00D2FF88));

        const char* sliderLabels[] = { "GRAVITY", "FRICTION", "ELASTICITY" };
        float sliderMins[] = { -20.0f, 0.0f, 0.0f };
        float sliderMaxs[] = { 20.0f, 1.0f, 1.0f };

        for (int i = 0; i < 3; i++) {
            auto sliderLayout = UILayout::GetSliderLayout(i);
            auto state = UIWidgets::GetInteractionState(
                {sliderLayout.hitRect.x, sliderLayout.hitRect.y, sliderLayout.hitRect.width, sliderLayout.hitRect.height},
                activePos, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

            float currentVal = (i == 0) ? settings.gravityStrength :
                               (i == 1) ? settings.frictionCoefficient :
                                          settings.elasticityValue;
            UIWidgets::DrawSlider(sliderLayout, sliderLabels[i], currentVal,
                                  sliderMins[i], sliderMaxs[i], state);
        }

    } else if (settings.activeTab == TAB_ENTITIES) {
        UIWidgets::DrawLabel((float)UILayout::MARGIN, (float)UILayout::SECTION_HEADER_Y,
                             "MATTER SPECIFICATION", UILayout::SECTION_HEADER_SIZE, GetColor(0x00D2FF88));

        const char* entityLabels[] = { "SINGULARITY WELL", "RECURSIVE REPELLER", "ORBITAL PULSAR" };
        Color entityColors[] = { SKYBLUE, PURPLE, GOLD };

        for (int i = 0; i < 3; i++) {
            auto btnLayout = UILayout::GetEntityButtonLayout(i);
            auto state = UIWidgets::GetInteractionState(
                {btnLayout.rect.x, btnLayout.rect.y, btnLayout.rect.width, btnLayout.rect.height},
                activePos, IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

            if (UIWidgets::DrawEntityButton(btnLayout, entityLabels[i], entityColors[i],
                                            settings.activeObjectType == i, state, isPinching)) {
                settings.activeObjectType = i;
            }
        }
    }

    if (activePos.x > 0) {
        Color cursorCol = isPinching ? GetColor(0x00FF88FF) : GetColor(0x00D2FFFF);
        float innerRad = isPinching ? 6.0f : 4.0f;
        float outerRad = isPinching ? 10.0f : 16.0f;

        DrawCircleV(activePos, innerRad, cursorCol);
        DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad, Fade(cursorCol, 0.8f));
        if (isPinching) DrawCircleLines((int)activePos.x, (int)activePos.y, outerRad + 4.0f, Fade(cursorCol, 0.3f));
    }
}

void EngineUI::HandleSpatialInteractivity(EngineSettings& settings, Vector2 handPos, bool isPinching, bool isHandVisible) {
    Vector2 activePos = isHandVisible ? handPos : GetMousePosition();
    bool activeTriggered = isHandVisible ? isPinching : IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    static bool wasTriggered = false;
    bool justClicked = (activeTriggered && !wasTriggered);
    wasTriggered = activeTriggered;

    auto tab0 = UILayout::GetTabRect(0);
    auto tab1 = UILayout::GetTabRect(1);

    if (justClicked) {
        if (CheckCollisionPointRec(activePos, {tab0.x, tab0.y, tab0.width, tab0.height})) {
            settings.activeTab = TAB_PARAMETERS;
        }
        if (CheckCollisionPointRec(activePos, {tab1.x, tab1.y, tab1.width, tab1.height})) {
            settings.activeTab = TAB_ENTITIES;
        }
    }

    if (activePos.x > 0 && activePos.x < EngineUI::PANEL_WIDTH) {
        if (settings.activeTab == TAB_PARAMETERS) {
            bool activeGrip = isHandVisible ? isPinching : IsMouseButtonDown(MOUSE_BUTTON_LEFT);

            for (int i = 0; i < 3; i++) {
                auto sliderLayout = UILayout::GetSliderLayout(i);
                Rectangle hitRect = {sliderLayout.hitRect.x, sliderLayout.hitRect.y, sliderLayout.hitRect.width, sliderLayout.hitRect.height};

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
        } else if (settings.activeTab == TAB_ENTITIES && justClicked) {
            for (int i = 0; i < 3; i++) {
                auto btnLayout = UILayout::GetEntityButtonLayout(i);
                Rectangle hitRect = {btnLayout.rect.x, btnLayout.rect.y, btnLayout.rect.width, btnLayout.rect.height};

                if (CheckCollisionPointRec(activePos, hitRect)) {
                    settings.activeObjectType = i;
                }
            }
        }
    }
}
