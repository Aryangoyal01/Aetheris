#pragma once

#include "particle.h"
#include <vector>

class ParticleSystem
{
public:
    void AddParticle(const Particle& particle);
    void RemoveParticle(size_t index);
    void Clear();

    size_t ParticleCount() const;
    const std::vector<Particle>& GetParticles() const;
    std::vector<Particle>& GetParticles();

private:
    std::vector<Particle> m_particles;
};
