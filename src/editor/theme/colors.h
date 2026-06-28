#pragma once

#include <imgui.h>

namespace Theme {
namespace Colors {

    // Very dark blue/black background
    constexpr ImVec4 Background       = ImVec4(0.05f, 0.05f, 0.07f, 1.0f);
    constexpr ImVec4 BackgroundDark   = ImVec4(0.03f, 0.03f, 0.05f, 1.0f);
    constexpr ImVec4 BackgroundLight  = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);
    
    // Panel/Popup Backgrounds
    constexpr ImVec4 PanelBackground  = ImVec4(0.07f, 0.07f, 0.09f, 1.0f);
    constexpr ImVec4 Border           = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    
    // Accents
    constexpr ImVec4 AccentCyan       = ImVec4(0.00f, 0.80f, 0.85f, 1.0f);
    constexpr ImVec4 AccentCyanDim    = ImVec4(0.00f, 0.60f, 0.65f, 1.0f);
    constexpr ImVec4 AccentPurple     = ImVec4(0.60f, 0.20f, 0.80f, 1.0f);
    
    // Status Colors
    constexpr ImVec4 Success          = ImVec4(0.10f, 0.80f, 0.30f, 1.0f);
    constexpr ImVec4 Warning          = ImVec4(0.90f, 0.60f, 0.10f, 1.0f);
    constexpr ImVec4 Error            = ImVec4(0.90f, 0.20f, 0.20f, 1.0f);
    
    // Text
    constexpr ImVec4 Text             = ImVec4(0.90f, 0.90f, 0.92f, 1.0f);
    constexpr ImVec4 TextDim          = ImVec4(0.50f, 0.50f, 0.55f, 1.0f);

}
}
