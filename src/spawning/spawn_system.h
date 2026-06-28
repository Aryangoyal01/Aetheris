#pragma once

#include "world/world.h"
#include "core_math/bounds.h"
#include "spawning/spawn_request.h"
#include "simulation/simulation_settings.h"

class SpawnSystem
{
public:
    void Update(World& world, float deltaTime, bool isPressed, bool isDown, const SpawnRequest& request, const SimulationSettings& settings);

private:
    void SpawnParticle(World& world, const SpawnRequest& request, const SimulationSettings& settings);

    float m_holdTimer = 0.0f;
    float m_spawnAccumulator = 0.0f;
};
