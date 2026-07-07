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

Aetheris is a modern **GPU-native scientific simulation platform** written in C++ using OpenGL, Raylib, and Dear ImGui.

The project combines a high-performance simulation engine with a professional desktop editor designed for interactive scientific visualization, experimentation, and future AI-assisted workflows.

It is **not** a game engine.

Its primary goal is to become a modular engineering platform for real-time simulation, visualization, and scientific computing.

---

# Current Status

## Overall Progress

**Current Milestone**

**Checkpoint 3 — GPU Particle Simulation & Emission**

**Completed Milestones**

- ✅ Engine Foundation
- ✅ Interactive Workspace
- ✅ CPU Reference Simulation
- ✅ GPU Compute Pipeline
- ✅ Professional Editor UI (Checkpoint 2)

The project has completed its editor foundation and GPU compute integration. Development is now focused on expanding simulation capabilities and particle behavior while preserving the established editor architecture.

---

# Current Architecture

The core architecture is considered stable.

Major subsystems include:

- Application
- Editor
- Theme & Widgets
- Input
- World
- Spawn System
- GPU Simulation
- CPU Reference Simulation
- Renderer
- Viewport

Subsystem responsibilities and dependencies are documented in `architecture.md`.

---

# Current Engine Capabilities

Implemented:

### Core Engine

- Modular application framework
- Deterministic update loop
- CPU reference simulation
- GPU compute simulation
- SSBO-based particle storage
- GPU instanced rendering
- Automatic discrete GPU selection (Optimus / PowerXpress)

### Editor

- Professional docked workspace
- Fixed engineering layout
- Modern theme system
- Reusable UI widget library
- Standardized property inspectors
- Unified Vector2Control (Cartesian / Polar / Dial)
- Physics inspector
- Particle inspector
- Console
- Status telemetry
- Camera controls
- Quick Actions panel

The CPU simulation remains the reference implementation for validation, while the GPU simulation is now the primary execution path.

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

1. Continuous GPU particle spawning
2. High-rate emission smoothing
3. Improved particle lifecycle management
4. Additional emitter shapes
5. Velocity distributions and randomization
6. GPU/CPU simulation parity
7. Performance optimization

After simulation improvements:

- Scene hierarchy
- Asset browser
- Material system
- Serialization
- Performance profiler
- AI-assisted workflows

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