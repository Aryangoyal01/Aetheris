# AETHERIS — ROADMAP

> This document defines the long-term development roadmap of Aetheris.
>
> Milestones represent major architectural phases.
> Checkpoints represent small, independently verifiable implementation steps.
>
> Every milestone builds upon the previous one.
> Earlier milestones should never require fundamental redesign.

---

# ✅ Milestone 0 — Foundation (Completed)

Establish the core engine infrastructure.

## Objectives

* Build system
* Window management
* Application lifecycle
* Logging
* Time subsystem
* Input subsystem
* Mathematical foundation
* Coordinate projection
* Documentation

**Status:** Complete

---

# ✅ Milestone 1 — 2D Workspace Foundation (Completed)

Build the interactive Cartesian workspace.

## Objectives

* Cartesian world
* Orthographic viewport
* World ↔ Screen projection
* Grid rendering
* Sandbox boundaries
* Pan
* Zoom
* Particle rendering
* Viewport interaction

**Status:** Complete

---

# ✅ Milestone 2 — CPU Reference Simulation (Completed)

Create a deterministic CPU simulation that serves as the reference implementation.

## Objectives

* World
* Particle system
* Spawn system
* Gravity
* Lifetime
* Collision modes
* Simulation settings
* Spawn requests
* Backend abstraction
* Manual verification

The CPU simulation exists primarily as a correctness reference for future GPU development.

**Status:** Complete

---

# 🚧 Milestone 3 — GPU Compute Infrastructure (Current)

Transition from CPU-driven simulation to GPU-native computation.

## Objectives

### Phase 3.1

* OpenGL 4.3 compute support
* Compute shader management
* Shader compilation
* Empty compute dispatch

### Phase 3.2

* SSBO infrastructure
* Persistent GPU buffers
* Particle upload
* Buffer validation

### Phase 3.3

* GPU gravity
* GPU integration
* CPU/GPU validation

### Phase 3.4

* GPU collision handling
* Bounce
* Wrap
* Destroy

### Phase 3.5

* GPU particle rendering
* Instanced rendering
* Remove CPU rendering path

### Phase 3.6

* Final GPU-native simulation
* CPU retained only as a reference implementation

**Status:** In Progress

---

# Milestone 4 — Professional Editor

Transform the simulation into a production workspace.

## Objectives

* Dockable panels
* Toolbar
* Inspector
* Hierarchy
* Properties
* Timeline (future)
* Debug overlays
* Selection
* Gizmos
* Runtime controls

The viewport remains the primary focus.

---

# Milestone 5 — Artificial Intelligence

Introduce AI as a first-class engine subsystem.

## Objectives

* Natural language interaction
* Scene generation
* Parameter editing
* Simulation assistance
* Autonomous workflows
* Agentic AI
* Engine reasoning

AI communicates through structured engine commands rather than directly manipulating simulation or rendering.

---

# Milestone 6 — Natural Interaction

Extend the engine beyond mouse and keyboard.

## Objectives

* Hand tracking
* Gesture recognition
* Voice commands
* Multi-modal interaction
* Future XR interfaces

All interaction methods ultimately produce structured world commands.

---

# Milestone 7 — Networking

Enable external communication and collaborative workflows.

## Objectives

* Remote control
* Python integration
* AI clients
* Telemetry
* Collaborative editing
* Automation APIs

Networking must remain independent of rendering.

---

# Milestone 8 — Production

Prepare Aetheris for long-term use and distribution.

## Objectives

* Performance optimization
* Serialization
* Project saving/loading
* Plugin system
* Advanced rendering
* Packaging
* Documentation
* Automated testing
* Cross-platform validation

---

# Long-Term Vision

The completed engine will combine:

* GPU-native simulation
* Professional editor tooling
* Artificial intelligence
* Natural interaction
* Networking
* High-performance rendering

within a single coherent architecture centered around one mathematical 2D world.

Every milestone should move the project closer to that vision while preserving architectural stability.
