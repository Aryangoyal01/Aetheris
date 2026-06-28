#pragma once

#include <imgui.h>

namespace Theme {
    namespace Metrics {
        // Typography Sizing
        constexpr float FontSizeDefault = 26.0f;
        constexpr float FontSizeHeader  = 33.0f;
        constexpr float FontSizeTitle   = 46.0f;
        constexpr float FontSizeCaption = 20.0f;

        // Mathematical Spacing Scale
        constexpr float Spacing2  = 2.0f;
        constexpr float Spacing4  = 4.0f;
        constexpr float Spacing6  = 6.0f;
        constexpr float Spacing8  = 8.0f;
        constexpr float Spacing10 = 10.0f;
        constexpr float Spacing12 = 12.0f;
        constexpr float Spacing16 = 16.0f;
        constexpr float Spacing20 = 20.0f;
        constexpr float Spacing24 = 24.0f;
        constexpr float Spacing32 = 32.0f;
        constexpr float Spacing40 = 40.0f;

        // UI Component Heights
        constexpr float ToolbarHeight   = 72.0f;
        constexpr float FooterHeight    = 42.0f;
        constexpr float NavWidth        = 200.0f;
        constexpr float SidebarItemHeight = 64.0f;
    }

    namespace Radii {
        constexpr float Button   = 6.0f;
        constexpr float Panel    = 8.0f;
        constexpr float Card     = 8.0f;
        constexpr float Floating = 10.0f;
        constexpr float Input    = 4.0f;
        constexpr float Pill     = 999.0f;
    }

    void ApplyAetherisTheme();
}
