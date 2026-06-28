#include "theme.h"
#include "colors.h"
#include <imgui.h>

namespace Theme {

void ApplyAetherisTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Window rounding and borders
    style.WindowRounding = 6.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.WindowBorderSize = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.ItemSpacing = ImVec2(10.0f, 10.0f);
    style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);

    // Apply color palette
    colors[ImGuiCol_Text]                   = Colors::Text;
    colors[ImGuiCol_TextDisabled]           = Colors::TextDim;
    
    colors[ImGuiCol_WindowBg]               = Colors::Background;
    colors[ImGuiCol_ChildBg]                = Colors::PanelBackground;
    colors[ImGuiCol_PopupBg]                = Colors::BackgroundDark;
    
    colors[ImGuiCol_Border]                 = Colors::Border;
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    
    colors[ImGuiCol_FrameBg]                = Colors::BackgroundDark;
    colors[ImGuiCol_FrameBgHovered]         = Colors::AccentCyanDim;
    colors[ImGuiCol_FrameBgActive]          = Colors::AccentCyan;
    
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
    
    colors[ImGuiCol_Button]                 = Colors::BackgroundLight;
    colors[ImGuiCol_ButtonHovered]          = Colors::AccentCyanDim;
    colors[ImGuiCol_ButtonActive]           = Colors::AccentCyan;
    
    colors[ImGuiCol_Header]                 = Colors::BackgroundLight;
    colors[ImGuiCol_HeaderHovered]          = Colors::AccentCyanDim;
    colors[ImGuiCol_HeaderActive]           = Colors::AccentCyan;
    
    colors[ImGuiCol_Separator]              = Colors::Border;
    colors[ImGuiCol_SeparatorHovered]       = Colors::AccentCyanDim;
    colors[ImGuiCol_SeparatorActive]        = Colors::AccentCyan;
    
    colors[ImGuiCol_ResizeGrip]             = Colors::BackgroundLight;
    colors[ImGuiCol_ResizeGripHovered]      = Colors::AccentCyanDim;
    colors[ImGuiCol_ResizeGripActive]       = Colors::AccentCyan;
    
    colors[ImGuiCol_Tab]                    = Colors::BackgroundDark;
    colors[ImGuiCol_TabHovered]             = Colors::AccentCyanDim;
    colors[ImGuiCol_TabActive]              = Colors::BackgroundLight;
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
