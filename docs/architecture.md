# AETHERIS — Architecture

> This document defines the high-level architecture of Aetheris.
>
> It describes how the engine is organized, how subsystems interact, and the architectural principles that guide future development.
>
> Detailed implementation of individual systems is documented separately in `systems.md`.

---

# Overview

Aetheris is a modular, GPU-native scientific simulation platform with an integrated professional editor.

The engine is organized into independent subsystems that communicate through well-defined interfaces. Every subsystem owns a single responsibility and is designed to evolve independently without introducing unnecessary coupling.

The architecture prioritizes long-term scalability, deterministic behavior, reusable editor components, and GPU-first execution.

---

# High-Level Architecture

```text
                     Application
                          │
        ┌─────────────────┼─────────────────┐
        │                 │                 │
     Editor            Engine          Platform
        │                 │                 │
        │        ┌────────┴────────┐        │
        │        │                 │        │
      Theme   Simulation      Renderer   GPU Preference
        │        │                 │
        │        │                 │
        └────────┴────────┬────────┘
                          │
                        World
```

The **Application** owns subsystem lifetime but contains no rendering or simulation logic.

The **World** remains the single source of truth shared by all runtime systems.

Subsystems communicate through public interfaces or the World rather than depending directly on one another.

---

# Core Subsystems

## Application

Coordinates the engine lifecycle.

Responsibilities:

- Initialization
- Main loop
- Shutdown
- Configuration
- Subsystem ownership

---

## Editor

Provides the complete desktop editing environment.

Responsibilities:

- Dockspace
- Viewport
- Inspector
- Console
- Toolbar
- Status telemetry
- Quick Actions
- User interaction

The Editor never performs simulation or rendering itself.

---

## Theme & Widgets

Provides reusable editor UI components.

Responsibilities:

- Color palette
- Typography
- Layout metrics
- Shared widgets
- Property controls
- Section headers
- Vector2Control
- Segmented controls

Panels consume these widgets instead of implementing custom UI logic.

This subsystem establishes a single visual language across the editor.

---

## Input

Collects user input.

Current:

- Keyboard
- Mouse

Planned:

- Game controllers
- Hand tracking
- Voice input

Input only reports state and never modifies simulation directly.

---

## World

The World is the central runtime data model.

It stores the complete simulation state and acts as the single source of truth shared by every runtime subsystem.

Future systems such as entities, assets, materials, and scene data will also be managed here.

---

## Simulation

Responsible for updating the World.

Current architecture includes two implementations:

### CPU Reference Simulation

Used for correctness validation.

### GPU Compute Simulation

Primary runtime implementation using compute shaders and SSBOs.

Simulation computes state only.

It never performs rendering or UI operations.

---

## Renderer

Visualizes the current World state.

Responsibilities:

- GPU particle rendering
- Instanced rendering
- Infinite grid
- Debug visualization
- Editor overlays

Renderer never modifies simulation data.

---

## Platform

Provides platform-specific integrations.

Current responsibilities:

- Window creation
- OpenGL context
- File system
- Native platform services
- High-performance GPU selection (Optimus / PowerXpress)

Platform services remain isolated from engine logic.

---

# Ownership Rules

Each subsystem owns exactly one area of responsibility.

| Subsystem | Owns |
|------------|--------------------------------|
| Application | Lifetime & initialization |
| Editor | User interface |
| Theme | UI appearance & reusable widgets |
| Input | Input state |
| World | Runtime simulation data |
| Simulation | State updates |
| Renderer | Visualization |
| Platform | Platform integration |

Ownership must remain unique.

Responsibilities should never overlap.

---

# Dependency Rules

Dependencies always flow downward.

Allowed

- Application → All subsystems
- Editor → Theme
- Editor → Engine interfaces
- Simulation → World
- Renderer → World
- Platform → OS APIs

Forbidden

- Renderer → Simulation
- Simulation → Renderer
- World → Renderer
- World → Editor
- Theme → Panels
- Circular dependencies

Communication should occur through public interfaces whenever practical.

---

# Architectural Principles

The architecture follows several non-negotiable principles.

- Single Responsibility
- Explicit Ownership
- Clear Dependency Direction
- Modular Design
- Separation of Engine and Editor
- Reusable UI Components
- Theme-Driven Editor
- GPU-First Simulation
- Deterministic Simulation
- Data-Oriented Growth

These principles take precedence over implementation convenience.

---

# Extensibility

The architecture is intentionally designed to grow without structural redesign.

Planned additions include:

- Advanced GPU particle systems
- Scene hierarchy
- Asset management
- Material system
- Serialization
- AI assistant
- Timeline
- Performance profiler
- Plugin framework
- Python scripting
- Hand tracking
- Voice interaction

New systems should integrate into existing architecture rather than introducing new dependency patterns.

---

# Architecture Stability

The overall architecture is now considered stable following the completion of the Professional Editor and GPU Compute Pipeline milestones.

Future work should extend existing systems rather than restructuring them.

Large architectural changes should only occur when they provide substantial long-term benefits that cannot be achieved within the existing modular design.