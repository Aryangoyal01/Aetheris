#pragma once

#include <cstdint>

enum class CollisionMode
{
    None,
    Bounce,
    Wrap,
    Destroy
};

struct SimulationSettings
{
    float gravity = -50.0f;
    float restitution = 0.8f;
    float spawnRate = 60.0f;
    float spawnHoldDelay = 0.2f;
    float defaultLifetime = 30.0f;
    float defaultRadius = 3.0f;
    uint32_t maxParticles = 25000;
    CollisionMode collisionMode = CollisionMode::Bounce;
};
