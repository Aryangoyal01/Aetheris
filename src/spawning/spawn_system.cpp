#include "spawn_system.h"

void SpawnSystem::SpawnParticle(World& world, const SpawnRequest& request, const SimulationSettings& settings)
{
    auto& ps = world.GetParticleSystem();
    if (ps.ParticleCount() >= settings.maxParticles)
    {
        return;
    }

    Particle p;
    p.position = request.position;
    p.velocity = request.velocity;
    p.lifetime = request.lifetime;
    p.maxLifetime = request.lifetime;
    p.size = request.radius;
    p.r = request.color.r;
    p.g = request.color.g;
    p.b = request.color.b;
    p.a = request.color.a;
    p.active = true;

    ps.AddParticle(p);
}

void SpawnSystem::Update(World& world, float deltaTime, bool isPressed, bool isDown, const SpawnRequest& request, const SimulationSettings& settings)
{
    float timePerSpawn = 1.0f / settings.spawnRate;

    if (isPressed)
    {
        SpawnParticle(world, request, settings);
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
    }
    else if (isDown)
    {
        m_holdTimer += deltaTime;
        if (m_holdTimer >= settings.spawnHoldDelay)
        {
            m_spawnAccumulator += deltaTime;
            while (m_spawnAccumulator >= timePerSpawn)
            {
                SpawnParticle(world, request, settings);
                m_spawnAccumulator -= timePerSpawn;
            }
        }
    }
    else
    {
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
    }
}
