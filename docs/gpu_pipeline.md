# Aetheris GPU Pipeline Architecture

The Aetheris engine utilizes a GPU-authoritative pipeline for physics simulation and rendering. The CPU orchestrates the application loop, handles viewport/input, and spawns new particles, but all particle physics integration and visualization are executed on the GPU.

## Ownership Model

- **CPU (Application / World)**: Owns user input, spawn requests, camera viewport, and application orchestration.
- **GPU (SSBO)**: Owns the definitive state of every particle (position, velocity, lifetime, color, etc.).
- **Simulation (Compute Shader)**: Owns the physics integration logic. Mutates the SSBO.
- **Renderer (Graphics Shader)**: Owns the visualization pipeline. Reads from the SSBO via instanced rendering to draw quads.

## Data Flow Diagram

```text
CPU Input
│
▼
Spawn Requests
│
▼
SSBO Upload (new particles only)
│
▼
Compute Shader (Physics Simulation)
│
▼
Instanced Renderer (Visualization)
│
▼
Screen
```

## Frame Execution Order

1. **CPU Updates**: 
   - Time increments (`Time::Update`).
   - Input is polled (`Input::Update`).
   - Viewport is recalculated (zoom/pan).
2. **Spawning**:
   - `SpawnSystem::Update` adds newly spawned static particles to the CPU's `ParticleSystem`.
3. **Upload**:
   - `GPUParticleBuffer::Upload` detects newly added particles.
   - Using `glBufferSubData` (or `glNamedBufferSubData`), only the newly spawned particles are appended into the Shader Storage Buffer Object (SSBO). Existing simulated particles are not overwritten.
4. **Compute Phase**:
   - The `simulate.comp` compute shader is bound.
   - The SSBO is bound to binding point 0.
   - Uniforms (deltaTime, gravity, bounds) are injected.
   - `glDispatchCompute` is called, spawning threads equal to `maxParticles`.
   - The compute shader integrates velocity and position, and processes collision logic.
   - `glMemoryBarrier` ensures all writes to the SSBO are completed.
5. **Render Phase**:
   - The screen clears.
   - Grid renders.
   - `particle.vert` and `particle.frag` are bound.
   - `glDrawElementsInstanced` issues a single draw call for all particles.
   - The vertex shader uses `gl_InstanceID` to fetch position and properties directly from the updated SSBO, bypassing traditional Vertex Attributes.
   - The fragment shader rasterizes a perfect circle via distance-based discard.