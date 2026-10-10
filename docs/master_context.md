# AETHERIS — Master Context

> **Authoritative Project Context**
>
> This document provides the current state of the Aetheris project.
>
> It should be the **first document** read by any developer or AI agent before contributing to the codebase.
>
> Detailed information is available in:
>
> * `vision.md`
> * `architecture.md`
> * `systems.md`
> * `roadmap.md`
> * `coding_standards.md`
> * `development_workflow.md`

---

# Project Summary

Aetheris is a **GPU-native scientific simulation platform focused on visualising physics, interactive experimentation, and eventual physics-problem solving**.

The project combines a high-performance simulation engine with a professional desktop engineering editor designed for real-time visualization, scientific experimentation, and analytical/numerical problem solving.

It is **not** a game engine.

The current particle engine and workspace form the computational foundation for interactive scientific physics modeling.

---

# Current Status

## Overall Progress

**Current Milestone**

**Milestone 5 — GPU Particle Systems & Physics Visualization Foundation**

**Completed / Verified Milestones**

- ✅ Engine Foundation [Verified]
- ✅ Interactive Workspace [Verified]
- ✅ CPU Reference Simulation [Verified]
- ✅ GPU Compute Pipeline [Verified]
- ✅ Professional Editor UI (Checkpoint 2) [Verified]

---

# Current Engine Capabilities

### Core Engine

- Modular application framework [Verified]
- Deterministic update loop [Verified]
- Continuous particle emission with sub-frame spatial interpolation [Verified in automated headless tests; Manually Verified in GUI]
  *(Note: Spawning follows the CPU-side `SpawnSystem` → `World::ParticleSystem` → SSBO buffer upload path; it is not yet GPU-native compute emission. Known limitation: dead/inactive particles on the GPU remain represented in the CPU particle list and continue occupying capacity until an explicit CPU-GPU synchronization or compaction is performed. Delta time is clamped to 100 ms to intentionally limit catch-up emission bursts after large frame stalls)*
- Centralized `Log` subsystem with thread-safe ring buffer and severity routing [Verified]
- CPU reference simulation [Verified]
- GPU compute simulation via OpenGL 4.3 compute shaders [Verified]
- SSBO-based particle storage [Verified]
- GPU instanced quad rendering [Verified]
- Automatic discrete GPU preference exports [Verified on Windows runtime]

### Editor

- Professional docked workspace [Verified]
- Fixed engineering layout [Verified]
- Modern theme system and reusable UI widget library [Verified]
- Standardized property inspectors [Verified]
- Unified Vector2Control (Cartesian / Polar / Dial) [Verified]
- Physics and Particle inspectors [Verified]
- Developer Console [Verified in automated test harness; Manually Verified in GUI]:
  - Live engine log streaming with timestamps and severity badges (`INFO`, `WARN`, `ERROR`, `SYSTEM`) [Manually Verified in GUI]
  - Interactive commands: `help`, `clear`, `clear particles`, `gravity <x> <y>`, `pause`, `play`, `sync`, `stats` (with GPU time displayed as `N/A (unmetered)`), `reset` [Manually Verified in GUI]
  - Auto-scroll and manual scroll history [Manually Verified in GUI]
- Status telemetry (FPS, frame time, particle count) [Verified]
- Camera zoom/pan and Quick Actions panel [Verified]

The CPU simulation remains the reference implementation for validation, while the GPU simulation is the primary execution path.

---

# Current Design Decisions

The following decisions are considered foundational and should not be changed without strong architectural justification:

- One shared World model.
- Deterministic simulation.
- GPU-first architecture.
- Separation between Engine and Editor.
- Modular subsystem architecture.
- Explicit ownership.
- Theme-driven UI system.
- Reusable editor widgets.
- Professional desktop application workflow.

---

# Immediate Development Priorities

Current focus:

1. GPU-native compute emitter kernels (true GPU-side particle allocation/emission).
2. GPU dead-particle compaction / ring-buffer capacity reclamation (resolving CPU-side capacity retention of inactive particles).
3. GPU timer queries for real-time GPU frame-time profiling (replacing unmetered GPU telemetry).
4. Additional emitter shapes (disc, line, cone).
5. Velocity distributions and randomization.
6. Improved particle lifecycle management (color and size over lifetime).
7. Physics problem solving operators (vector fields, energy conservation telemetry).
8. GPU/CPU simulation parity.

After simulation improvements:

- Scene hierarchy
- Asset browser
- Material system
- Serialization
- Performance profiler
- AI-assisted physics modeling workflows

---

# Future Direction

The long-term objective is to evolve Aetheris into a complete engineering platform supporting:

- GPU-native scientific simulation
- Advanced particle systems
- Modular physics systems
- AI-assisted workflows
- Professional editor tooling
- Plugin architecture
- Advanced visualization
- Large-scale simulation datasets

Future milestones are described in `roadmap.md`.

---

# Working Principles

Every implementation should:

- Preserve architectural consistency.
- Follow the coding standards.
- Respect subsystem boundaries.
- Reuse existing systems before introducing new ones.
- Prefer Theme widgets over custom UI implementations.
- Remain independently testable.
- Leave the project in a stable state.

Development proceeds through small, reviewable checkpoints.

---

# Before Starting Any New Work

Before implementing a feature:

1. Read this document.
2. Review `architecture.md`.
3. Review `systems.md`.
4. Check `roadmap.md` for the active milestone.
5. Follow `coding_standards.md`.
6. Reuse existing Theme widgets and editor components whenever possible.
7. Implement the feature using the defined development workflow.

---

# Purpose of this Document

This document intentionally remains concise.

It provides the authoritative snapshot of the current project state while directing contributors to the detailed architectural, systems, and roadmap documentation.

Any AI agent or developer should treat this document as the primary source for understanding Aetheris before making implementation decisions.