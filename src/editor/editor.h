#pragma once

#include <vector>
#include <memory>
#include "editor_state.h"
#include "core_math/geometry.h"

class IPanel;
class Renderer;
class Viewport;

class Editor
{
public:
    Editor();
    ~Editor();

    void Initialize(Renderer* renderer, Viewport* viewport, SimulationSettings* settings);
    void BeginFrame();
    void Update();
    void EndFrame();
    void SetTelemetry(int fps, float frameTimeMs, float gpuTimeMs, int particleCount);
    bool ConsumeClearParticlesRequest();
    bool ConsumeSyncRequest();
    // Viewport State Routing
    bool IsViewportHovered() const;
    Point2 GetViewportLocalMouse() const;

    void Destroy();

private:
    void SetupDockspace();

    EditorState m_state;
    std::vector<std::unique_ptr<IPanel>> m_panels;
};