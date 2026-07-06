#pragma once

enum class InspectorMode
{
    Physics,
    Particles,
    Environment,
    Scene,
    Camera,
    Assets,
    Console,
    Settings
};

#include "../simulation/simulation_settings.h"

struct ImFont;

struct EditorState
{
    InspectorMode activeMode = InspectorMode::Physics;
    
    ImFont* fontTitle = nullptr;
    ImFont* fontHeader = nullptr;
    ImFont* fontDefault = nullptr;
    ImFont* fontCaption = nullptr;

    SimulationSettings* settings = nullptr;
    
    int fps = 0;
    float frameTimeMs = 0.0f;
    float gpuTimeMs = 0.0f;
    int particleCount = 0;
    
    bool clearParticlesRequested = false;
    bool syncRequested = false;
};
