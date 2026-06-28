#include "world.h"

ParticleSystem& World::GetParticleSystem()
{
    return m_particleSystem;
}

const ParticleSystem& World::GetParticleSystem() const
{
    return m_particleSystem;
}
