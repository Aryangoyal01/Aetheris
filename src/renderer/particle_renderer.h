#pragma once

#include <cstdint>

class GPUAllocator;

class ParticleRenderer {
public:
    ParticleRenderer();
    ~ParticleRenderer();

    bool Init(GPUAllocator& allocator);
    void Shutdown();

    void Render(uint32_t activeCount);

    bool IsInitialized() const { return m_initialized; }

private:
    uint32_t m_vao = 0;
    uint32_t m_vbo = 0;
    uint32_t m_ebo = 0;
    uint32_t m_indexCount = 0;
    uint32_t m_renderProgram = 0;
    bool m_initialized = false;

    int m_locMVP = -1;
    int m_locModelView = -1;
    int m_locNormalMatrix = -1;
    int m_locViewPos = -1;

    void CreateUnitSphere(int segments = 16);
    bool LoadRenderShader();
};
