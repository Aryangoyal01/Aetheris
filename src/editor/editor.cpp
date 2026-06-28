#include "editor.h"
#include "theme/theme.h"
#include "panels/viewport_panel.h"
#include "panels/toolbar_panel.h"
#include "panels/left_sidebar.h"
#include "panels/right_sidebar.h"
#include "panels/bottom_panel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <rlImGui.h>
#include <implot.h>

Editor::Editor() = default;

Editor::~Editor() = default;

void Editor::Initialize(Renderer* renderer, Viewport* viewport)
{
    rlImGuiSetup(true); // true = enable docking
    ImPlot::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    Theme::ApplyAetherisTheme();

    m_panels.push_back(std::make_unique<ToolbarPanel>());
    m_panels.push_back(std::make_unique<LeftSidebar>(&m_state));
    m_panels.push_back(std::make_unique<RightSidebar>(&m_state));
    m_panels.push_back(std::make_unique<BottomPanel>());
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
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
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
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

        ImGuiID dock_main_id = dockspace_id;
        
        // Use ImGuiDockNodeFlags_HiddenTabBar and ImGuiDockNodeFlags_NoUndocking to lock layout
        // wait, ImGui::DockBuilderAPI doesn't have a specific way to set flags for individual nodes easily except modifying the node after creation,
        // but we can set flags on the root dockspace? No, it's better to set it using DockNodeFlags on the DockSpace.
        // Or wait, DockBuilderDockWindow sets the window to the node. The window flags will prevent undocking.
        // Actually, we can get the node and set flags:
        
        ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.05f, nullptr, &dock_main_id);
        ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, nullptr, &dock_main_id);
        ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.15f, nullptr, &dock_main_id);
        
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