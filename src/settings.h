#pragma once
#include "raylib.h"
#include <cstdint>

constexpr uint32_t WORKGROUP_SIZE = 256;
constexpr uint32_t MAX_PARTICLES = 500000;
constexpr uint32_t DEFAULT_ACTIVE_PARTICLES = 50000;

enum SidebarTab : int {
    SIDEBAR_PHYSICS = 0,
    SIDEBAR_PARTICLES,
    SIDEBAR_ENVIRONMENT,
    SIDEBAR_PERFORMANCE,
    SIDEBAR_AI,
    SIDEBAR_SCENE,
    SIDEBAR_COUNT
};

enum BottomDockTab : int {
    DOCK_CONSOLE = 0,
    DOCK_LOGS,
    DOCK_PERFORMANCE,
    DOCK_COUNT
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
    SidebarTab activeSidebarTab = SIDEBAR_PHYSICS;
    BottomDockTab activeBottomDockTab = DOCK_CONSOLE;
    bool sidebarVisible = true;
    bool dockVisible = true;
    int activeObjectType = 0;

    float gravityStrength = 9.81f;
    float frictionCoefficient = 0.15f;
    float elasticityValue = 0.75f;
    float smoothingFactor = 12.0f;
    float interactionRadius = 15.0f;
    float timeScale = 1.0f;

    float targetGravity = 9.81f;
    float targetFriction = 0.15f;
    float targetElasticity = 0.75f;
    float targetTimeScale = 1.0f;

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