# AETHERIS — Development Log

> Engineering log documenting the evolution of Project Aetheris.
>
> Each session records major technical decisions, root causes, implementation details, and verification results.

---

# Session 01 — GPU Compute Pipeline Completed

**Date:** June 2026

## Objective

Replace the original CPU-based particle simulation with a scalable GPU compute pipeline using OpenGL 4.3 while establishing the renderer architecture that will support future AI, networking, and gesture interaction.

---

# Initial State

The project initially consisted of:

* CPU particle simulation
* Raylib rendering
* Basic particle spawning
* UDP tracking infrastructure
* Single-threaded update loop

Although functional, the architecture could not scale to large particle counts and was unsuitable for future AI-driven simulation.

---

# Major Engineering Work

## 1. Renderer Architecture

A modular rendering system was introduced.

New renderer components:

```text
Renderer

├── GPUAllocator
├── ComputePipeline
├── ParticleRenderer
└── Renderer
```

Responsibilities were clearly separated to simplify future maintenance.

---

## 2. OpenGL 4.3 Migration

The renderer was upgraded from the default OpenGL 3.3 pipeline to an OpenGL 4.3 Compute Shader pipeline.

Implemented:

* Modern OpenGL function loading
* Compute shader support
* Shader Storage Buffer Objects (SSBOs)
* GPU compute dispatch

This became the foundation for GPU particle simulation.

---

## 3. Hybrid GPU Selection

Problem

Windows consistently selected the integrated AMD GPU instead of the RTX 4050.

Result

* Compute shaders unavailable
* OpenGL 3.3 context created
* SSBO initialization failed

Solution

Exported

```cpp
NvOptimusEnablement

AmdPowerXpressRequestHighPerformance
```

forcing Windows to launch the application on the dedicated GPU.

Verification

Renderer now reports:

* NVIDIA RTX 4050
* OpenGL 4.3
* GLSL 4.30

---

## 4. SSBO Pipeline

Persistent Shader Storage Buffer Objects were implemented.

Design decisions:

* Maximum capacity: 500,000 particles
* Persistent allocation
* No per-frame recreation
* Spawn queue uploads only modified particles

CPU no longer owns particle state.

---

## 5. Compute Shader

GPU simulation implemented using compute shaders.

Responsibilities include:

* Gravity
* Velocity integration
* Collision response
* Particle lifetime
* Physics updates

The CPU no longer performs particle simulation.

---

## 6. Instanced Rendering

Particle rendering transitioned from CPU draw loops to GPU instanced rendering.

Custom rendering shaders were introduced.

Each instance retrieves its particle data directly from the SSBO using `gl_InstanceID`.

---

# Major Bugs Encountered

## Wrong OpenGL Context

Symptoms

* OpenGL 3.3
* Compute unavailable

Root Cause

Integrated GPU selected.

Resolution

Forced dedicated GPU selection.

---

## SSBO Initialization Failure

Symptoms

Renderer initialization failed.

Root Cause

OpenGL 3.3 context.

Resolution

Resolved automatically after OpenGL 4.3 context was established.

---

## Access Violation (0xC0000005)

Symptoms

Application crashed immediately after startup.

Root Cause

Incorrect OpenGL function loading through `GetProcAddress`.

Resolution

Correctly dereferenced GLAD function pointers loaded from the raylib DLL.

---

## Massive Black Particle Blob

Symptoms

Huge black sphere at world origin.

Actual FPS approximately 1.

Root Cause

Renderer always drew `activeParticleCount` (50,000 instances) instead of the number of alive particles.

Inactive particles shared identical transform data.

Resolution

Rendering now uses the allocator's alive particle count.

---

## Spawn Pipeline Failure

Symptoms

Mouse clicks produced no particles.

Root Cause

When tracking was offline, the smoothed cursor position never updated from the mouse.

Spawn condition therefore never became true.

Resolution

Added automatic fallback to `GetMousePosition()` whenever no tracking data is available.

---

## Incorrect Camera Transform

Symptoms

Particles rendered incorrectly in world space.

Root Cause

Projection and view matrices multiplied in the wrong order.

Resolution

Corrected matrix multiplication order inside the particle renderer.

---

# Runtime Verification

Verified successfully:

* Application launches
* OpenGL 4.3 context
* RTX 4050 selected
* Compute shader compilation
* SSBO allocation
* Compute dispatch
* Particle spawning
* Gravity simulation
* Collision response
* Stable 60 FPS
* Responsive UI
* Clean shutdown

Milestone 1 acceptance criteria satisfied.

---

# Current Status

Milestone 1

**Status:** ✅ Completed

Current engine capabilities:

* GPU particle simulation
* Instanced rendering
* Stable renderer architecture
* Compute shader pipeline
* Modular renderer
* Responsive runtime

---

# Remaining Polish

The following items remain before beginning Milestone 2.

Simulation

* Improve lifetime fade
* Tune bounce behaviour
* Tune friction
* Improve visual feel

Rendering

* Better lighting
* Particle glow
* Material improvements

Diagnostics

* GPU timing overlay
* Particle statistics
* Runtime profiler

These are polish tasks and do not block future development.

---

# Next Milestone

## Milestone 1.5 — Simulation Polish

Immediate objectives:

* Refine particle behaviour
* Improve rendering quality
* Expand diagnostics
* Prepare the engine for AI integration

Following successful polish, development will proceed to:

**Milestone 2 — AI Networking Layer**

---

# Lessons Learned

Several important architectural lessons emerged during this milestone:

* GPU infrastructure should be validated incrementally before optimization.
* Always verify actual runtime behaviour rather than relying solely on engine diagnostics.
* Persistent GPU resources significantly simplify large-scale simulation.
* Correct subsystem isolation is often more effective than premature optimization.
* Clear documentation of architectural decisions greatly improves continuity across development sessions.

---

**End of Session 01**
