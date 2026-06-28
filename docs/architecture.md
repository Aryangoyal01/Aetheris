# AETHERIS — Architecture

> This document defines the high-level architecture of Aetheris.
>
> It describes how the engine is organized, how subsystems interact, and the architectural principles that guide future development.
>
> Detailed implementation of individual systems is documented separately in `systems.md`.

---

# Overview

Aetheris is a modular, GPU-oriented scientific simulation engine with an integrated editor.

The project is organized into independent subsystems that communicate through well-defined interfaces. Each subsystem owns a single responsibility and can evolve without tightly coupling to the rest of the engine.

The architecture is designed for long-term scalability, allowing new features to be integrated without requiring major redesigns.

---

# Core Architecture

```text
                  Application
                       │
        ┌──────────────┼──────────────┐
        │              │              │
      Editor        Engine         Platform
                        │
      ┌─────────────────┼─────────────────┐
      │                 │                 │
   Input            Simulation       Renderer
      │                 │                 │
      └──────────────┬──┴─────────────────┘
                     │
                  World
```

The **Application** coordinates subsystem lifetime but contains no simulation or rendering logic.

The **World** represents the shared state of the engine.

Subsystems interact through the World or explicit interfaces rather than directly depending on one another.

---

# Subsystems

## Application

Coordinates the engine lifecycle.

Responsibilities:

* Initialization
* Main loop
* Shutdown
* Configuration
* Subsystem ownership

---

## Editor

Provides the user interface and editing environment.

Responsibilities:

* Dockspace
* Viewport
* Inspector
* Console
* Editor tools
* User interaction

The Editor never performs simulation or rendering itself.

---

## Input

Collects user input from supported devices.

Current:

* Keyboard
* Mouse

Planned:

* Game controllers
* Hand tracking
* Voice input

Input only reports state and never modifies the simulation directly.

---

## World

The World is the central data model.

It stores the current simulation state and acts as the single source of truth shared across the engine.

Future systems such as entities, assets, and scene data will also be managed here.

---

## Simulation

Updates the World according to the configured physics model.

Its responsibility is to compute state changes only.

It never performs rendering or UI operations.

---

## Renderer

Visualizes the current World state.

Responsibilities include:

* Particle rendering
* Grid rendering
* Debug visualization
* Editor overlays

The Renderer never modifies simulation data.

---

# Ownership Rules

Each subsystem has explicit ownership.

| Subsystem   | Owns                   |
| ----------- | ---------------------- |
| Application | Subsystem lifetime     |
| Editor      | User interface         |
| Input       | Input state            |
| World       | Shared simulation data |
| Simulation  | World updates          |
| Renderer    | Visualization          |

Ownership must remain unique.

No subsystem should duplicate another subsystem's responsibility.

---

# Dependency Principles

Dependencies always flow in one direction.

Allowed:

* Application → All subsystems
* Editor → Engine interfaces
* Simulation → World
* Renderer → World
* Input → Events

Forbidden:

* Renderer → Simulation
* Simulation → Renderer
* Renderer → Input
* World → Renderer
* Circular dependencies

Subsystems communicate through interfaces rather than direct coupling whenever practical.

---

# Design Principles

The architecture follows several core principles:

* Single Responsibility
* Explicit Ownership
* Clear Dependency Direction
* Modular Design
* Separation of Engine and Editor
* Data-Oriented Growth
* GPU-Oriented Future Design

These principles take precedence over short-term implementation convenience.

---

# Extensibility

The architecture is intentionally designed to accommodate future systems without structural redesign.

Planned additions include:

* GPU Compute Pipeline
* Scene Hierarchy
* Asset Management
* Material System
* AI Assistant
* Timeline
* Performance Profiler
* Plugin System
* Hand Tracking
* Voice Interaction

New systems should integrate into the existing architecture rather than introducing new dependency patterns.

---

# Architecture Stability

This document describes the intended long-term structure of Aetheris.

Subsystem implementations will evolve over time, but the architectural philosophy and module boundaries should remain stable throughout the project's lifetime.
