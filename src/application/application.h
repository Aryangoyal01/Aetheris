#pragma once

#include <memory>
#include "rendering/renderer.h"
#include "rendering/shader/compute_shader.h"
#include "rendering/gpu/gpu_particle_buffer.h"
#include "simulation/simulation_settings.h"
#include "spawning/spawn_system.h"
#include "editor/editor.h"
#include "viewport/viewport.h"
#include "world/world.h"

class Application
{
public:
    Application() = default;
    ~Application() = default;

    void Initialize();
    void Run();
    void Shutdown();

private:
    SimulationSettings m_settings;
    World m_world;
    SpawnSystem m_spawnSystem;
    Viewport m_viewport;
    Renderer m_renderer;
    Editor m_editor;
    ComputeShader m_computeShader;
    GPUParticleBuffer m_particleBuffer;
};
