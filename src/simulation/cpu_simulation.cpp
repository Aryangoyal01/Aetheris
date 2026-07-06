#include "cpu_simulation.h"

void CPUSimulation::Update(World& world, float deltaTime, const SimulationSettings& settings)
{
    auto& particles = world.GetParticleSystem().GetParticles();
    
    float boundsLeft = World::BOUNDS_LEFT;
    float boundsRight = World::BOUNDS_RIGHT;
    float boundsBottom = World::BOUNDS_BOTTOM;
    float boundsTop = World::BOUNDS_TOP;
    
    float subDt = deltaTime / settings.substeps;

    for (int step = 0; step < settings.substeps; ++step)
    {
        for (auto& p : particles)
        {
            if (!p.active) continue;
            
            p.lifetime -= subDt;
            if (p.lifetime <= 0.0f)
            {
                p.active = false;
                continue;
            }
            
            p.velocity.y += settings.gravity * subDt;
            
            p.position.x += p.velocity.x * subDt;
            p.position.y += p.velocity.y * subDt;
            
            if (settings.collisionMode != CollisionMode::None)
            {
                bool collision = false;
                if (p.position.x - p.size < boundsLeft) {
                    p.position.x = boundsLeft + p.size;
                    p.velocity.x *= -settings.restitution;
                    collision = true;
                }
                else if (p.position.x + p.size > boundsRight) {
                    p.position.x = boundsRight - p.size;
                    p.velocity.x *= -settings.restitution;
                    collision = true;
                }
                
                if (p.position.y - p.size < boundsBottom) {
                    p.position.y = boundsBottom + p.size;
                    p.velocity.y *= -settings.restitution;
                    collision = true;
                }
                else if (p.position.y + p.size > boundsTop) {
                    p.position.y = boundsTop - p.size;
                    p.velocity.y *= -settings.restitution;
                    collision = true;
                }
                
                if (collision && settings.collisionMode == CollisionMode::Destroy)
                {
                    p.active = false;
                }
                else if (collision && settings.collisionMode == CollisionMode::Wrap)
                {
                    if (p.position.x - p.size <= boundsLeft) p.position.x = boundsRight - p.size;
                    else if (p.position.x + p.size >= boundsRight) p.position.x = boundsLeft + p.size;
                    if (p.position.y - p.size <= boundsBottom) p.position.y = boundsTop - p.size;
                    else if (p.position.y + p.size >= boundsTop) p.position.y = boundsBottom + p.size;
                }
            }
        }
    }
}
