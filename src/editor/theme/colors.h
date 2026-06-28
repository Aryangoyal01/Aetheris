#pragma once

#include <imgui.h>

namespace Theme {
namespace Colors {

    // Backgrounds - layered depth system
    constexpr ImVec4 Background       = ImVec4(0.07f, 0.07f, 0.08f, 1.0f);  // Deepest (viewport)
    constexpr ImVec4 BackgroundDark   = ImVec4(0.04f, 0.04f, 0.05f, 1.0f);  // Panels, footer
    constexpr ImVec4 BackgroundLight  = ImVec4(0.11f, 0.11f, 0.12f, 1.0f);  // Active titles, buttons

    // Surfaces
    constexpr ImVec4 PanelBackground  = ImVec4(0.09f, 0.09f, 0.10f, 1.0f);  // Child windows
    constexpr ImVec4 Surface          = ImVec4(0.12f, 0.12f, 0.13f, 1.0f);  // Input fields, cards
    constexpr ImVec4 SurfaceHover     = ImVec4(0.15f, 0.15f, 0.16f, 1.0f);  // Input hover
    constexpr ImVec4 SurfaceActive    = ImVec4(0.18f, 0.18f, 0.19f, 1.0f);  // Input active

    // Borders
    constexpr ImVec4 Border           = ImVec4(0.16f, 0.16f, 0.18f, 1.0f);  // Default borders
    constexpr ImVec4 BorderSubtle     = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);  // Subtle separators
    constexpr ImVec4 BorderStrong     = ImVec4(0.30f, 0.30f, 0.33f, 1.0f);  // Strong separators

    // Accents
    constexpr ImVec4 AccentCyan       = ImVec4(0.15f, 0.85f, 0.90f, 1.0f);
    constexpr ImVec4 AccentCyanDim    = ImVec4(0.10f, 0.65f, 0.70f, 1.0f);
    constexpr ImVec4 AccentCyanFaded  = ImVec4(0.15f, 0.85f, 0.90f, 0.15f);
    constexpr ImVec4 AccentPurple     = ImVec4(0.65f, 0.35f, 0.90f, 1.0f);
    constexpr ImVec4 AccentPurpleDim  = ImVec4(0.50f, 0.25f, 0.70f, 1.0f);
    constexpr ImVec4 AccentPurpleFaded= ImVec4(0.65f, 0.35f, 0.90f, 0.12f);

    // Status Colors
    constexpr ImVec4 Success          = ImVec4(0.20f, 0.85f, 0.45f, 1.0f);
    constexpr ImVec4 SuccessDim       = ImVec4(0.15f, 0.65f, 0.35f, 1.0f);
    constexpr ImVec4 Warning          = ImVec4(0.95f, 0.75f, 0.20f, 1.0f);
    constexpr ImVec4 WarningDim       = ImVec4(0.75f, 0.58f, 0.15f, 1.0f);
    constexpr ImVec4 Error            = ImVec4(0.95f, 0.30f, 0.35f, 1.0f);
    constexpr ImVec4 ErrorDim         = ImVec4(0.75f, 0.22f, 0.27f, 1.0f);
    constexpr ImVec4 Info             = ImVec4(0.20f, 0.85f, 0.90f, 1.0f);
    constexpr ImVec4 System           = ImVec4(0.65f, 0.35f, 0.90f, 1.0f);
    constexpr ImVec4 AI               = ImVec4(0.65f, 0.35f, 0.90f, 1.0f);

    // Text
    constexpr ImVec4 Text             = ImVec4(0.93f, 0.93f, 0.95f, 1.0f);
    constexpr ImVec4 TextDim          = ImVec4(0.50f, 0.50f, 0.55f, 1.0f);
    constexpr ImVec4 TextMuted        = ImVec4(0.35f, 0.35f, 0.40f, 1.0f);

}
}
