#include "renderer.h"
#include "settings.h"
#include "gl_helpers.h"
#include "rlgl.h"

Renderer::Renderer() = default;
Renderer::~Renderer() { Shutdown(); }

bool Renderer::Init(const std::string& computeShaderPath) {
    if (m_initialized) Shutdown();

    if (!m_allocator.Init(DEFAULT_ACTIVE_PARTICLES)) {
        return false;
    }

    if (!m_compute.Init(computeShaderPath, m_allocator)) {
        return false;
    }

    if (!m_particleRenderer.Init(m_allocator)) {
        return false;
    }

    m_initialized = true;
    return true;
}

void Renderer::Shutdown() {
    m_particleRenderer.Shutdown();
    m_compute.Shutdown();
    m_allocator.Shutdown();
    m_initialized = false;
}

void Renderer::DispatchCompute(uint32_t activeCount, float dt, const EngineSettings& settings,
                               float handWorldX, float handWorldY, float handWorldZ,
                               bool handDetected, bool isPinching) {
    if (!m_initialized) return;

    rlBindShaderBuffer(m_allocator.GetSSBO(), 0);

    m_compute.Dispatch(activeCount, dt, settings,
                       handWorldX, handWorldY, handWorldZ,
                       handDetected, isPinching);
}

void Renderer::RenderParticles(uint32_t activeCount) {
    if (!m_initialized) return;

    ext_glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_allocator.GetSSBO());
    m_particleRenderer.Render(activeCount);
}

void Renderer::EnqueueSpawn(const GPUParticle& particle) {
    m_allocator.EnqueueSpawn(particle);
}

void Renderer::FlushSpawnQueue() {
    m_allocator.FlushSpawnQueue();
}
