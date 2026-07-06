#pragma once
#include "world/world.h"
#include "simulation_settings.h"

class CPUSimulation
{
public:
    void Update(World& world, float deltaTime, const SimulationSettings& settings);
};
