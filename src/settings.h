#pragma once
#include "raylib.h"
#include <cstdint>

constexpr uint32_t WORKGROUP_SIZE = 256;
constexpr uint32_t MAX_PARTICLES = 500000;
constexpr uint32_t DEFAULT_ACTIVE_PARTICLES = 50000;

enum UITab {
    TAB_PARAMETERS,
    TAB_ENTITIES
};

enum ParticleType {
    PARTICLE_TYPE_A, // Singularity Well (Sphere)
    PARTICLE_TYPE_B, // Recursive Repeller (Cube)
    PARTICLE_TYPE_C  // Orbital Pulsar (Cylinder)
};

struct GPUParticle {
    Vector4 position;
    Vector4 velocity;
    Vector4 color;
    int type;
    int active;
    float padding[2];
};

struct EngineSettings {
    UITab activeTab = TAB_PARAMETERS;
    int activeObjectType = 0;

    float gravityStrength = 9.81f;
    float frictionCoefficient = 0.15f;
    float elasticityValue = 0.75f;
    float smoothingFactor = 12.0f;
    float interactionRadius = 15.0f;
    float timeScale = 1.0f;

    uint32_t activeParticleCount = 50000;
};

struct PendingSettings {
    bool hasUpdate = false;
    float gravityStrength = 9.81f;
    float frictionCoefficient = 0.15f;
    float elasticityValue = 0.75f;
    float timeScale = 1.0f;
    int spawnType = 0;
};