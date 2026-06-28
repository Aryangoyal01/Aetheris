# AETHERIS — Systems

> This document provides a high-level overview of the major engine systems that power Aetheris.
>
> It explains the purpose, responsibilities, data flow, and future direction of each subsystem without describing implementation details.

---

# Engine Flow

Each frame follows a deterministic execution order:

```text
Input
    │
    ▼
Spawn System
    │
    ▼
Simulation
    │
    ▼
Renderer
    │
    ▼
Editor & Presentation
```

Each system performs a single responsibility before passing control to the next stage.

---

# World System

The **World** is the central data model of Aetheris.

It stores the complete state of the current simulation.

Current responsibilities include:

* Particle storage
* Simulation state
* Global environment

Future responsibilities:

* Scene graph
* Entity management
* Asset references
* Selection state

The World is the engine's single source of truth.

---

# Particle System

The Particle System manages all simulation particles.

Responsibilities:

* Creation
* Lifetime
* Storage
* Identification
* Removal

Current implementation is CPU based.

Future implementation will migrate particle storage entirely to GPU memory.

---

# Spawn System

The Spawn System converts user interaction into particle creation.

Responsibilities:

* Spawn requests
* Spawn timing
* Spawn shapes
* Spawn limits
* Initial particle properties

It prepares particles but never simulates them.

---

# Simulation System

The Simulation System updates the World every frame.

Current features:

* Gravity
* Velocity integration
* Collision handling
* Boundary constraints

Future additions:

* Force fields
* Springs
* Fluid simulation
* Soft bodies
* Rigid body interaction
* Custom simulation modules

Simulation modifies only World state.

---

# Rendering System

The Renderer visualizes the World.

Current responsibilities:

* Background grid
* Particle rendering
* Debug overlays
* Viewport rendering

Future additions:

* GPU instancing
* Post-processing
* Advanced visualization
* Selection rendering

Rendering never changes simulation data.

---

# Camera & Viewport

The Viewport defines how the user observes the World.

Responsibilities:

* Pan
* Zoom
* Coordinate conversion
* Camera transforms

It does not modify simulation state.

---

# Editor System

The Editor provides the user interface for interacting with the engine.

Current components:

* Navigation Rail
* Toolbar
* Viewport
* Inspector
* Console
* Status Bar

The Editor controls the engine through interfaces rather than directly manipulating internal systems.

---

# GPU Compute Pipeline

The engine is designed to transition from CPU simulation to GPU-native execution.

Target pipeline:

```text
Particle Spawn
        │
        ▼
GPU Upload
        │
        ▼
Compute Shader Simulation
        │
        ▼
Persistent GPU Buffers
        │
        ▼
GPU Rendering
```

The CPU implementation will remain available as a reference implementation for validation and debugging.

---

# Data Flow

Information moves through the engine in one direction.

```text
User Input
      │
      ▼
Spawn Requests
      │
      ▼
World State
      │
      ▼
Simulation
      │
      ▼
Renderer
      │
      ▼
Editor Presentation
```

This deterministic flow minimizes coupling and keeps subsystem responsibilities clear.

---

# Future Systems

The architecture is designed to accommodate additional systems without structural redesign.

Planned additions include:

* Scene Hierarchy
* Asset Management
* Material System
* Serialization
* Performance Profiler
* AI Assistant
* Timeline
* Plugin Framework
* Hand Tracking
* Voice Interaction
* Networking

Each new system should integrate through existing engine interfaces rather than introducing new dependency patterns.

---

# Guiding Principle

Every system in Aetheris should satisfy three goals:

* Own a single responsibility.
* Operate independently of unrelated systems.
* Integrate cleanly into the overall engine architecture.

As the engine evolves, new functionality should extend existing systems rather than replace or duplicate them.
