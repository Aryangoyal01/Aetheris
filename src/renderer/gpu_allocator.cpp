#include "gpu_allocator.h"
#include "settings.h"
#include "rlgl.h"
#include "gl_helpers.h"
#include <cstring>
#include <vector>

GPUAllocator::GPUAllocator() = default;

GPUAllocator::~GPUAllocator() {
    Shutdown();
}

bool GPUAllocator::Init(uint32_t maxParticles) {
    if (m_initialized) {
        Shutdown();
    }

    m_maxCapacity = maxParticles;
    uint64_t bufferSize = static_cast<uint64_t>(maxParticles) * sizeof(GPUParticle);

    m_ssbo = rlLoadShaderBuffer(bufferSize, nullptr, RL_DYNAMIC_COPY);

    if (m_ssbo == 0) {
        return false;
    }

    m_initialized = true;
    ClearAll();

    return true;
}

void GPUAllocator::Shutdown() {
    if (m_ssbo != 0) {
        rlUnloadShaderBuffer(m_ssbo);
        m_ssbo = 0;
    }
    m_initialized = false;
    m_maxCapacity = 0;
}

void GPUAllocator::UploadParticles(const GPUParticle* particles, uint32_t count, uint32_t offset) {
    if (!m_initialized || count == 0) return;

    uint32_t clampedCount = (offset + count > m_maxCapacity)
        ? m_maxCapacity - offset
        : count;

    uint64_t byteOffset = static_cast<uint64_t>(offset) * sizeof(GPUParticle);
    uint64_t byteSize = static_cast<uint64_t>(clampedCount) * sizeof(GPUParticle);

    rlUpdateShaderBuffer(m_ssbo, particles, byteSize, byteOffset);
}

void GPUAllocator::ReadbackParticles(GPUParticle* dest, uint32_t count, uint32_t offset) const {
    if (!m_initialized || count == 0) return;

    uint32_t clampedCount = (offset + count > m_maxCapacity)
        ? m_maxCapacity - offset
        : count;

    uint64_t byteOffset = static_cast<uint64_t>(offset) * sizeof(GPUParticle);
    uint64_t byteSize = static_cast<uint64_t>(clampedCount) * sizeof(GPUParticle);

    rlReadShaderBuffer(m_ssbo, dest, byteSize, byteOffset);
}

void GPUAllocator::ClearAll() {
    if (!m_initialized) return;

    uint64_t bufferSize = static_cast<uint64_t>(m_maxCapacity) * sizeof(GPUParticle);
    GPUParticle zero{};
    std::vector<GPUParticle> zeros(m_maxCapacity, zero);
    rlUpdateShaderBuffer(m_ssbo, zeros.data(), bufferSize, 0);
    m_aliveCount = 0;
    m_nextSpawnIndex = 0;
    m_spawnQueue.clear();
}

void GPUAllocator::EnqueueSpawn(const GPUParticle& particle) {
    m_spawnQueue.push_back(particle);
}

void GPUAllocator::FlushSpawnQueue() {
    if (!m_initialized || m_spawnQueue.empty()) return;

    uint32_t count = static_cast<uint32_t>(m_spawnQueue.size());

    for (uint32_t i = 0; i < count; ++i) {
        uint32_t slot = (m_nextSpawnIndex + i) % m_maxCapacity;
        uint64_t byteOffset = static_cast<uint64_t>(slot) * sizeof(GPUParticle);
        rlUpdateShaderBuffer(m_ssbo, &m_spawnQueue[i], sizeof(GPUParticle), byteOffset);
    }

    m_nextSpawnIndex = (m_nextSpawnIndex + count) % m_maxCapacity;
    m_aliveCount = (m_aliveCount + count > m_maxCapacity) ? m_maxCapacity : m_aliveCount + count;
    m_spawnQueue.clear();
}
