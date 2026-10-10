#include "spawn_system.h"
#include <algorithm>
#include <cmath>
#include <cstdio>

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
    // Sanitize deltaTime: ignore non-finite or negative values
    if (!std::isfinite(deltaTime) || deltaTime < 0.0f)
    {
        deltaTime = 0.0f;
    }

    // Clamp frame-time spike to avoid runaway accumulator accumulation (max 100ms per frame)
    constexpr float MAX_DELTA_TIME = 0.1f;
    if (deltaTime > MAX_DELTA_TIME)
    {
        deltaTime = MAX_DELTA_TIME;
    }

    // Verify spawnRate is finite and strictly positive
    if (!std::isfinite(settings.spawnRate) || settings.spawnRate <= 0.0f)
    {
        if (isPressed)
        {
            SpawnParticle(world, request, settings);
        }
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
        m_hasPrevPosition = false;
        return;
    }

    float timePerSpawn = 1.0f / settings.spawnRate;
    if (!std::isfinite(timePerSpawn) || timePerSpawn <= 0.0f)
    {
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
        m_hasPrevPosition = false;
        return;
    }

    if (isPressed)
    {
        SpawnParticle(world, request, settings);
        m_prevPosition = request.position;
        m_hasPrevPosition = true;
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
    }
    else if (isDown)
    {
        if (!m_hasPrevPosition)
        {
            m_prevPosition = request.position;
            m_hasPrevPosition = true;
        }

        float prevHoldTimer = m_holdTimer;
        m_holdTimer += deltaTime;

        // If we are below the hold delay threshold, track position and do not emit
        if (m_holdTimer < settings.spawnHoldDelay)
        {
            m_prevPosition = request.position;
            return;
        }

        // Determine active emission time within this frame
        float activeDt = deltaTime;
        Point2 segStart = m_prevPosition;

        // Check if this frame crossed the hold delay boundary
        if (prevHoldTimer < settings.spawnHoldDelay)
        {
            activeDt = m_holdTimer - settings.spawnHoldDelay;
            if (activeDt > deltaTime) activeDt = deltaTime;

            // Interpolate position to the exact threshold point where emission began
            if (deltaTime > 0.0f)
            {
                float thresholdFraction = (settings.spawnHoldDelay - prevHoldTimer) / deltaTime;
                thresholdFraction = std::clamp(thresholdFraction, 0.0f, 1.0f);
                segStart.x = m_prevPosition.x + thresholdFraction * (request.position.x - m_prevPosition.x);
                segStart.y = m_prevPosition.y + thresholdFraction * (request.position.y - m_prevPosition.y);
            }
        }

        m_spawnAccumulator += activeDt;

        // Bound accumulator to prevent excessive iterations from accumulated remainder
        if (m_spawnAccumulator > 0.25f)
        {
            m_spawnAccumulator = 0.25f;
        }

        // Emit particles
        while (m_spawnAccumulator >= timePerSpawn)
        {
            // Capacity guard: stop immediately if capacity reached to avoid unbounded work
            if (world.GetParticleSystem().ParticleCount() >= settings.maxParticles)
            {
                m_spawnAccumulator = std::fmod(m_spawnAccumulator, timePerSpawn);
                break;
            }

            float t = 1.0f;
            if (activeDt > 0.0f)
            {
                float timeElapsed = activeDt - (m_spawnAccumulator - timePerSpawn);
                t = std::clamp(timeElapsed / activeDt, 0.0f, 1.0f);
            }

            SpawnRequest interpReq = request;
            interpReq.position.x = segStart.x + t * (request.position.x - segStart.x);
            interpReq.position.y = segStart.y + t * (request.position.y - segStart.y);

            SpawnParticle(world, interpReq, settings);
            m_spawnAccumulator -= timePerSpawn;
        }

        m_prevPosition = request.position;
    }
    else
    {
        m_holdTimer = 0.0f;
        m_spawnAccumulator = 0.0f;
        m_hasPrevPosition = false;
    }
}
