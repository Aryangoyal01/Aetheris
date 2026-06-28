#include "particle_system.h"

void ParticleSystem::AddParticle(const Particle& particle)
{
    m_particles.push_back(particle);
}

void ParticleSystem::RemoveParticle(size_t index)
{
    if (index < m_particles.size())
    {
        m_particles[index] = m_particles.back();
        m_particles.pop_back();
    }
}

void ParticleSystem::Clear()
{
    m_particles.clear();
}

size_t ParticleSystem::ParticleCount() const
{
    return m_particles.size();
}

const std::vector<Particle>& ParticleSystem::GetParticles() const
{
    return m_particles;
}

std::vector<Particle>& ParticleSystem::GetParticles()
{
    return m_particles;
}
