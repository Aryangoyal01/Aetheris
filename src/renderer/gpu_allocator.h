#pragma once

#include "settings.h"
#include <cstdint>
#include <vector>

class GPUAllocator {
public:
    GPUAllocator();
    ~GPUAllocator();

    bool Init(uint32_t maxParticles = MAX_PARTICLES);
    void Shutdown();

    uint32_t GetSSBO() const { return m_ssbo; }
    uint32_t GetMaxCapacity() const { return m_maxCapacity; }
    uint32_t GetAliveCount() const { return m_aliveCount; }

    void UploadParticles(const GPUParticle* particles, uint32_t count, uint32_t offset = 0);
    void ReadbackParticles(GPUParticle* dest, uint32_t count, uint32_t offset = 0) const;
    void ClearAll();

    void EnqueueSpawn(const GPUParticle& particle);
    void FlushSpawnQueue();

    bool IsInitialized() const { return m_initialized; }

private:
    uint32_t m_ssbo = 0;
    uint32_t m_maxCapacity = 0;
    uint32_t m_aliveCount = 0;
    bool m_initialized = false;

    std::vector<GPUParticle> m_spawnQueue;
    uint32_t m_nextSpawnIndex = 0;
};
