#pragma once

#include <cstdint>
#include <string>

struct EngineSettings;
class GPUAllocator;

struct ParticleSpawnData {
    float px, py, pz;
    float vx, vy, vz;
    float r, g, b, a;
    int type;
    float life;
    float maxLife;
};

class ComputePipeline {
public:
    ComputePipeline();
    ~ComputePipeline();

    bool Init(const std::string& shaderPath, GPUAllocator& allocator);
    void Shutdown();

    void Dispatch(uint32_t activeCount, float dt, const EngineSettings& settings,
                  float handWorldX, float handWorldY, float handWorldZ,
                  bool handDetected, bool isPinching);

    bool IsInitialized() const { return m_initialized; }

private:
    uint32_t m_computeProgram = 0;
    bool m_initialized = false;

    int m_locDt = -1;
    int m_locGravity = -1;
    int m_locFriction = -1;
    int m_locElasticity = -1;
    int m_locHandPos = -1;
    int m_locHandDetected = -1;
    int m_locIsPinching = -1;
    int m_locActiveObjectType = -1;
    int m_locInteractionStrength = -1;
    int m_locInteractionRadius = -1;

    void CacheUniformLocations();
};
