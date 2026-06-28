#include "editor.h"
#include "theme/theme.h"
#include "theme/colors.h"
#include "panels/viewport_panel.h"
#include "panels/toolbar_panel.h"
#include "panels/left_sidebar.h"
#include "panels/right_sidebar.h"
#include "panels/bottom_panel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <rlImGui.h>
#include <implot.h>

#include <extras/IconsFontAwesome6.h>
#include <extras/FA6FreeSolidFontData.h>
#include "theme/widgets.h"

Editor::Editor() = default;

Editor::~Editor() = default;

void Editor::Initialize(Renderer* renderer, Viewport* viewport)
{
    rlImGuiSetup(true); // Sets up imgui, default font, and FA5
    
    ImPlot::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // We must clear the fonts so the default imgui font is removed.
    // This ensures Inter becomes Font 0 (the default font).
    io.Fonts->Clear();

    // Load Fonts
    ImFontConfig fontConfig;
    fontConfig.OversampleH = 4;
    fontConfig.OversampleV = 4;
    fontConfig.PixelSnapH = true;

    // Shared icon config for merging FA into every font
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

    // Default font (Index 0) - Regular body text
    m_state.fontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Regular.ttf", Theme::Metrics::FontSizeDefault, &fontConfig);
    {
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = Theme::Metrics::FontSizeDefault;
        io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, Theme::Metrics::FontSizeDefault, &iconsConfig, icon_ranges);
    }

    // Title font - SemiBold (branding, large titles)
    m_state.fontTitle = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-SemiBold.ttf", Theme::Metrics::FontSizeTitle, &fontConfig);
    {
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = Theme::Metrics::FontSizeTitle;
        io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, Theme::Metrics::FontSizeTitle, &iconsConfig, icon_ranges);
    }

    // Header font - SemiBold (section headers)
    m_state.fontHeader = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-SemiBold.ttf", Theme::Metrics::FontSizeHeader, &fontConfig);
    {
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = Theme::Metrics::FontSizeHeader;
        io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, Theme::Metrics::FontSizeHeader, &iconsConfig, icon_ranges);
    }

    // Caption font - Regular (small text, labels, sidebar items)
    m_state.fontCaption = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Regular.ttf", Theme::Metrics::FontSizeCaption, &fontConfig);
    {
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphMinAdvanceX = Theme::Metrics::FontSizeCaption;
        io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, Theme::Metrics::FontSizeCaption, &iconsConfig, icon_ranges);
    }

    // Force build the font atlas so the texture is marked for creation in rlImGui
    io.Fonts->Build();

    printf("[DEBUG] Fonts Loaded:\n");
    printf("  Default: %p\n", (void*)m_state.fontDefault);
    printf("  Title:   %p\n", (void*)m_state.fontTitle);
    printf("  Header:  %p\n", (void*)m_state.fontHeader);
    printf("  Caption: %p\n", (void*)m_state.fontCaption);
    printf("  Total Fonts in Atlas: %d\n", io.Fonts->Fonts.Size);

    Theme::ApplyAetherisTheme();

    m_panels.push_back(std::make_unique<ToolbarPanel>(&m_state));
    m_panels.push_back(std::make_unique<LeftSidebar>(&m_state));
    m_panels.push_back(std::make_unique<RightSidebar>(&m_state));
    m_panels.push_back(std::make_unique<BottomPanel>(&m_state));
    m_panels.push_back(std::make_unique<ViewportPanel>(renderer, viewport));
}

void Editor::BeginFrame()
{
    rlImGuiBegin();
    SetupDockspace();
}

void Editor::Update()
{
    for (auto& panel : m_panels)
    {
        panel->Render();
    }
    
    // Draw Footer
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    float footerHeight = Theme::Metrics::FooterHeight;
    ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + viewport->WorkSize.y - footerHeight));
    ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, footerHeight));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Theme::Metrics::Spacing16, Theme::Metrics::Spacing4));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, Theme::Colors::BackgroundDark);

    ImGuiWindowFlags footerFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDocking;
    ImGui::Begin("##Footer", nullptr, footerFlags);

    if (m_state.fontCaption) ImGui::PushFont(m_state.fontCaption);

    // Top border — thick line for clean separation from viewport
    ImVec2 footerPos = ImGui::GetWindowPos();
    ImVec2 footerSize = ImGui::GetWindowSize();
    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(footerPos.x, footerPos.y),
        ImVec2(footerPos.x + footerSize.x, footerPos.y + 3.0f),
        ImGui::GetColorU32(Theme::Colors::BorderStrong));

    // Left side chips
    Theme::Widgets::StatusChip("SIMULATION STATE", "PAUSED", Theme::Colors::AccentCyan);
    ImGui::SameLine(0, 12);
    Theme::Widgets::StatusChip("CAMERA", "PERSPECTIVE", Theme::Colors::TextDim);
    ImGui::SameLine(0, 12);
    Theme::Widgets::StatusChip("PARTICLES", "1,024,000", Theme::Colors::Success);

    // Right side chips — pushed far right with clear spacing
    float rightPad = Theme::Metrics::Spacing20;

    // Grid toggle at far right
    ImGui::SameLine(viewport->WorkSize.x - 100.0f - rightPad);
    static bool gridEnabled = true;
    Theme::Widgets::ToggleSwitch("grid_toggle", gridEnabled, ImVec2(32, 18));
    ImGui::SameLine(0, 8);
    ImGui::TextColored(Theme::Colors::TextDim, "GRID");

    // Frame Time chip
    ImGui::SameLine(viewport->WorkSize.x - 310.0f - rightPad);
    Theme::Widgets::StatusChip("FRAME TIME", "16.6 ms", Theme::Colors::Success);

    // GPU Compute chip
    ImGui::SameLine(viewport->WorkSize.x - 520.0f - rightPad);
    Theme::Widgets::StatusChip("GPU COMPUTE", "5.21 ms", Theme::Colors::AccentPurple);

    if (m_state.fontCaption) ImGui::PopFont();

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}

void Editor::EndFrame()
{
    ImGui::End(); // End Dockspace
    rlImGuiEnd();
}

void Editor::Destroy()
{
    m_panels.clear();
    ImPlot::DestroyContext();
    rlImGuiShutdown();
}

bool Editor::IsViewportHovered() const
{
    for (const auto& panel : m_panels)
    {
        if (panel->GetName() == "Viewport")
        {
            auto* vp = static_cast<ViewportPanel*>(panel.get());
            return vp->IsHovered();
        }
    }
    return false;
}

Point2 Editor::GetViewportLocalMouse() const
{
    for (const auto& panel : m_panels)
    {
        if (panel->GetName() == "Viewport")
        {
            auto* vp = static_cast<ViewportPanel*>(panel.get());
            return vp->GetLocalMousePos();
        }
    }
    return {0.0f, 0.0f};
}

void Editor::SetupDockspace()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    float footerHeight = Theme::Metrics::FooterHeight;
    ImVec2 workSize = viewport->WorkSize;
    workSize.y -= footerHeight;
    
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(workSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("AetherisDockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);

    ImGuiID dockspace_id = ImGui::GetID("AetherisDockSpace");
    
    // Set up default layout on first run or if imgui.ini is missing/reset
    static bool first_time = true;
    if (first_time)
    {
        first_time = false;
        
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, workSize);

        ImGuiID dock_main_id = dockspace_id;
        
        // Use ImGuiDockNodeFlags_HiddenTabBar and ImGuiDockNodeFlags_NoUndocking to lock layout
        // wait, ImGui::DockBuilderAPI doesn't have a specific way to set flags for individual nodes easily except modifying the node after creation,
        // but we can set flags on the root dockspace? No, it's better to set it using DockNodeFlags on the DockSpace.
        // Or wait, DockBuilderDockWindow sets the window to the node. The window flags will prevent undocking.
        // Actually, we can get the node and set flags:
        
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.10f, nullptr, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.22f, nullptr, &dock_main_id);
        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.22f, nullptr, &dock_main_id);
        
        // Hide tabs to prevent dragging
        ImGuiDockNode* node_left = ImGui::DockBuilderGetNode(dock_id_left);
        ImGuiDockNode* node_right = ImGui::DockBuilderGetNode(dock_id_right);
        ImGuiDockNode* node_bottom = ImGui::DockBuilderGetNode(dock_id_bottom);
        ImGuiDockNode* node_center = ImGui::DockBuilderGetNode(dock_main_id);
        
        if (node_left) { node_left->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoResize; }
        if (node_right) { node_right->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton; }
        if (node_bottom) { node_bottom->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton; }
        if (node_center) { node_center->LocalFlags |= ImGuiDockNodeFlags_HiddenTabBar | ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton; }
        
        ImGui::DockBuilderDockWindow("Navigation", dock_id_left);
        ImGui::DockBuilderDockWindow("Viewport", dock_main_id);
        ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
        ImGui::DockBuilderDockWindow("Console", dock_id_bottom);
        
        ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoUndocking);
}