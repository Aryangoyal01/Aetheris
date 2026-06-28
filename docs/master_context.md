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

The project combines a high-performance simulation engine with a professional editor designed for interactive scientific visualization and future AI-assisted workflows.

It is **not** a game engine.

Its primary goal is to become a modular platform for real-time simulation, visualization, and experimentation.

---

# Current Status

## Overall Progress

Current milestone:

**GPU Compute Pipeline**

Previous milestones completed:

* ✅ Engine Foundation
* ✅ Interactive Workspace
* ✅ CPU Reference Simulation
* ✅ Professional Editor (UI Foundation v1.0)

The project has transitioned from building infrastructure to implementing core engine functionality.

---

# Current Architecture

The architecture is considered stable.

Major subsystems include:

* Application
* Editor
* Input
* World
* Spawn System
* Simulation
* Renderer
* Viewport

Subsystem responsibilities and dependencies are documented in `architecture.md`.

---

# Current Engine Capabilities

Implemented:

* Modular application framework
* Dockable editor
* Modern UI design system
* Cartesian world
* Camera controls
* Particle rendering
* CPU particle simulation
* Physics inspector
* Console
* Status telemetry
* Deterministic update loop

Current simulation is CPU-based and serves as the reference implementation.

---

# Current Design Decisions

The following decisions are considered foundational:

* One shared World model.
* Deterministic simulation.
* Separation between Editor and Engine.
* Modular subsystem architecture.
* Explicit ownership.
* GPU-first long-term direction.
* Professional desktop application workflow.

These should not be changed without strong architectural justification.

---

# Immediate Development Priorities

Current focus:

1. GPU Compute Shader framework
2. SSBO infrastructure
3. GPU particle simulation
4. GPU rendering
5. CPU/GPU validation

After GPU migration:

* Scene hierarchy
* Asset browser
* Material system
* Serialization
* AI assistant
* Performance profiler

---

# Future Direction

The long-term objective is to evolve Aetheris into a complete engineering platform supporting:

* GPU-native simulation
* AI-assisted workflows
* Professional editor tooling
* Natural interaction
* Plugin architecture
* Advanced visualization

Future milestones are described in `roadmap.md`.

---

# Working Principles

Every implementation should:

* Preserve architectural consistency.
* Follow the coding standards.
* Respect subsystem boundaries.
* Remain independently testable.
* Leave the project in a stable state.

Development always proceeds through small, reviewable checkpoints.

---

# Before Starting Any New Work

Before implementing a feature:

1. Read this document.
2. Review `architecture.md`.
3. Review `systems.md`.
4. Check `roadmap.md` for the current milestone.
5. Follow `coding_standards.md`.
6. Implement the feature using the defined development workflow.

---

# Purpose of this Document

This document is intentionally concise.

It does **not** duplicate detailed architectural, system, or roadmap information.

Instead, it provides the current project snapshot and directs contributors to the appropriate documentation for deeper understanding.
