#pragma once

#include "gpu_allocator.h"
#include "compute_pipeline.h"
#include "particle_renderer.h"
#include <cstdint>
#include <string>

struct EngineSettings;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Init(const std::string& computeShaderPath);
    void Shutdown();

    void DispatchCompute(uint32_t activeCount, float dt, const EngineSettings& settings,
                         float handWorldX, float handWorldY, float handWorldZ,
                         bool handDetected, bool isPinching);

    void RenderParticles(uint32_t activeCount);

    void EnqueueSpawn(const GPUParticle& particle);
    void FlushSpawnQueue();

    GPUAllocator& GetAllocator() { return m_allocator; }
    bool IsInitialized() const { return m_initialized; }

private:
    GPUAllocator m_allocator;
    ComputePipeline m_compute;
    ParticleRenderer m_particleRenderer;
    bool m_initialized = false;
};
