#pragma once

#include <cstdint>

enum class CollisionMode
{
    None,
    Bounce,
    Wrap,
    Destroy
};

enum class SimulationState
{
    Edit,
    Play,
    Pause
};

struct SimulationSettings
{
    static constexpr float MAX_SPAWN_RATE = 100000.0f;
    static constexpr uint32_t MIN_PARTICLES_LIMIT = 100;
    static constexpr uint32_t MAX_PARTICLES_LIMIT = 1000001;

    float gravity[2] = {0.0f, -50.0f};
    float restitution = 0.8f;
    float spawnRate = 60.0f;
    float spawnHoldDelay = 0.2f;
    float defaultLifetime = 30.0f;
    float defaultRadius = 3.0f;
    float defaultVelocity[2] = {0.0f, 0.0f};
    uint32_t maxParticles = 25000;
    CollisionMode collisionMode = CollisionMode::Bounce;
    bool useComputeShader = true;
    
    SimulationState state = SimulationState::Edit;
    int substeps = 4;
    int solverIterations = 8;
};
