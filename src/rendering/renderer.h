#pragma once

#include "grid_renderer.h"
#include <raylib.h>
#include <cstdint>

class Viewport;
class World;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Initialize();
    void BeginFrame();
    void Clear();
    void RenderParticles(uint32_t particleCount);
    void EndFrame();

    void ResizeTarget(int width, int height);
    const RenderTexture2D& GetTarget() const { return m_renderTarget; }

    void SetViewport(const Viewport* viewport);
    GridRenderer& GetGridRenderer();

private:
    const Viewport* m_viewport = nullptr;
    GridRenderer m_gridRenderer;

    unsigned int m_shaderId = 0;
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    RenderTexture2D m_renderTarget = {0};
    int m_targetWidth = 0;
    int m_targetHeight = 0;
};
