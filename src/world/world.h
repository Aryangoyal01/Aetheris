#pragma once

#include "particle_system.h"

class World
{
public:
    static constexpr float BOUNDS_LEFT = -500.0f;
    static constexpr float BOUNDS_RIGHT = 500.0f;
    static constexpr float BOUNDS_BOTTOM = -300.0f;
    static constexpr float BOUNDS_TOP = 300.0f;

    ParticleSystem& GetParticleSystem();
    const ParticleSystem& GetParticleSystem() const;

private:
    ParticleSystem m_particleSystem;
};
