#include "theme.h"
#include "colors.h"
#include <imgui.h>

namespace Theme {

void ApplyAetherisTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Rounding
    style.WindowRounding = Radii::Panel;
    style.ChildRounding = Radii::Panel;
    style.FrameRounding = Radii::Input;
    style.PopupRounding = Radii::Floating;
    style.ScrollbarRounding = Radii::Pill;
    style.GrabRounding = Radii::Button;
    style.TabRounding = Radii::Button;

    // Borders
    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;

    // Padding & Spacing
    style.WindowPadding = ImVec2(Metrics::Spacing20, Metrics::Spacing20);
    style.FramePadding = ImVec2(Metrics::Spacing12, Metrics::Spacing8);
    style.ItemSpacing = ImVec2(Metrics::Spacing12, Metrics::Spacing10);
    style.ItemInnerSpacing = ImVec2(Metrics::Spacing8, Metrics::Spacing8);
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;

    // Colors
    colors[ImGuiCol_Text]                   = Colors::Text;
    colors[ImGuiCol_TextDisabled]           = Colors::TextDim;

    colors[ImGuiCol_WindowBg]               = Colors::Background;
    colors[ImGuiCol_ChildBg]                = Colors::PanelBackground;
    colors[ImGuiCol_PopupBg]                = Colors::BackgroundDark;

    colors[ImGuiCol_Border]                 = Colors::Border;
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    colors[ImGuiCol_FrameBg]                = Colors::Surface;
    colors[ImGuiCol_FrameBgHovered]         = Colors::SurfaceHover;
    colors[ImGuiCol_FrameBgActive]          = Colors::SurfaceActive;

    colors[ImGuiCol_TitleBg]                = Colors::BackgroundDark;
    colors[ImGuiCol_TitleBgActive]          = Colors::BackgroundLight;
    colors[ImGuiCol_TitleBgCollapsed]       = Colors::BackgroundDark;

    colors[ImGuiCol_MenuBarBg]              = Colors::BackgroundDark;

    colors[ImGuiCol_ScrollbarBg]            = Colors::BackgroundDark;
    colors[ImGuiCol_ScrollbarGrab]          = Colors::Border;
    colors[ImGuiCol_ScrollbarGrabHovered]   = Colors::AccentCyanDim;
    colors[ImGuiCol_ScrollbarGrabActive]    = Colors::AccentCyan;

    colors[ImGuiCol_CheckMark]              = Colors::AccentCyan;

    colors[ImGuiCol_SliderGrab]             = Colors::AccentCyanDim;
    colors[ImGuiCol_SliderGrabActive]       = Colors::AccentCyan;

    colors[ImGuiCol_Button]                 = Colors::Surface;
    colors[ImGuiCol_ButtonHovered]          = Colors::SurfaceHover;
    colors[ImGuiCol_ButtonActive]           = Colors::AccentCyanDim;

    colors[ImGuiCol_Header]                 = Colors::Surface;
    colors[ImGuiCol_HeaderHovered]          = Colors::SurfaceHover;
    colors[ImGuiCol_HeaderActive]           = Colors::AccentCyanDim;

    colors[ImGuiCol_Separator]              = Colors::Border;
    colors[ImGuiCol_SeparatorHovered]       = Colors::AccentCyanDim;
    colors[ImGuiCol_SeparatorActive]        = Colors::AccentCyan;

    colors[ImGuiCol_ResizeGrip]             = Colors::Surface;
    colors[ImGuiCol_ResizeGripHovered]      = Colors::AccentCyanDim;
    colors[ImGuiCol_ResizeGripActive]       = Colors::AccentCyan;

    colors[ImGuiCol_Tab]                    = Colors::BackgroundDark;
    colors[ImGuiCol_TabHovered]             = Colors::SurfaceHover;
    colors[ImGuiCol_TabActive]              = Colors::Surface;
    colors[ImGuiCol_TabUnfocused]           = Colors::BackgroundDark;
    colors[ImGuiCol_TabUnfocusedActive]     = Colors::PanelBackground;

    colors[ImGuiCol_DockingPreview]         = Colors::AccentPurple;
    colors[ImGuiCol_DockingEmptyBg]         = Colors::Background;

    colors[ImGuiCol_PlotLines]              = Colors::AccentCyan;
    colors[ImGuiCol_PlotLinesHovered]       = Colors::AccentCyan;
    colors[ImGuiCol_PlotHistogram]          = Colors::AccentCyan;
    colors[ImGuiCol_PlotHistogramHovered]   = Colors::AccentCyan;

    colors[ImGuiCol_TextSelectedBg]         = Colors::AccentPurple;
    colors[ImGuiCol_DragDropTarget]         = Colors::Warning;
    colors[ImGuiCol_NavHighlight]           = Colors::AccentCyan;
    colors[ImGuiCol_NavWindowingHighlight]  = Colors::Text;
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
}

}
