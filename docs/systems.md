# AETHERIS — Systems

> This document provides a high-level overview of the major engine systems that power Aetheris.
>
> It explains the purpose, responsibilities, data flow, and interaction of each subsystem without describing low-level implementation details.

---

# Runtime Flow

Each frame follows a deterministic execution order:

```text
Input
    │
    ▼
Spawn System
    │
    ▼
Simulation (GPU)
    │
    ▼
Renderer
    │
    ▼
Editor & Presentation
```

Every system owns a single responsibility before passing control to the next stage.

The CPU simulation remains available as a reference implementation for validation.

---

# World System

The **World** is the central runtime data model of Aetheris.

It stores the complete state of the simulation and acts as the single source of truth shared by all engine systems.

Current responsibilities:

- Particle storage
- Simulation settings
- Global physics parameters
- Environment state

Future responsibilities:

- Scene hierarchy
- Entity management
- Asset references
- Selection state
- Material assignments

No subsystem owns duplicate simulation data.

---

# Particle System

The Particle System manages every simulated particle.

Responsibilities:

- Particle allocation
- Lifetime management
- Capacity limits
- Spawn initialization
- GPU buffer ownership

Current implementation:

- CPU reference simulation
- GPU-native particle storage (SSBO)
- GPU instanced rendering

Future additions:

- Multiple particle emitters
- Color over lifetime
- Size over lifetime
- Rotation
- Velocity distributions
- Particle attributes
- Custom particle modules

---

# Spawn System

The Spawn System converts editor input into particle creation.

Responsibilities:

- Spawn requests
- Spawn timing
- Continuous emission
- Burst emission
- Spawn shapes
- Initial particle properties

The Spawn System initializes particles only.

Simulation is handled exclusively by the Simulation System.

---

# Simulation System

The Simulation System updates the World every simulation step.

Current features:

- Gravity
- Velocity integration
- Boundary constraints
- GPU compute execution
- CPU validation path

Planned additions:

- Continuous high-rate emission
- Force fields
- Springs
- Attractors
- Turbulence
- Soft bodies
- Fluid simulation
- Modular simulation operators

Simulation modifies only World state.

---

# Rendering System

The Renderer visualizes the World.

Current responsibilities:

- GPU instanced particle rendering
- Infinite engineering grid
- Debug overlays
- Viewport rendering

Future additions:

- Advanced particle rendering
- Color gradients
- Lighting
- Post-processing
- Selection rendering
- Debug visualization layers

Rendering never changes simulation state.

---

# Camera & Viewport

The Viewport controls observation of the simulation.

Responsibilities:

- Pan
- Zoom
- Coordinate conversion
- Camera transforms
- World-to-screen projection

The viewport never modifies simulation data.

---

# Editor System

The Editor provides the professional engineering interface.

Current components:

- Navigation Rail
- Toolbar
- Viewport
- Inspector
- Console
- Status Bar
- Quick Actions

The editor communicates exclusively through engine interfaces and never manipulates simulation internals directly.

---

# Theme & Widget System

The editor uses a reusable UI framework built around shared widgets.

Current reusable components include:

- Section headers
- Property rows
- Segmented controls
- Vector2Control
- Polar direction dial
- Standard numeric controls

All inspector panels consume these shared widgets to ensure visual and behavioral consistency across the editor.

Future editor functionality should extend these widgets instead of introducing panel-specific implementations.

---

# GPU Compute Pipeline

The primary simulation path is fully GPU-native.

```text
Spawn Requests
       │
       ▼
Particle Upload
       │
       ▼
SSBO Storage
       │
       ▼
Compute Shader Simulation
       │
       ▼
Persistent GPU Buffers
       │
       ▼
GPU Instanced Rendering
```

The CPU implementation remains available as the authoritative correctness reference.

On hybrid laptops, Aetheris automatically requests the high-performance discrete GPU when available.

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

This deterministic flow minimizes coupling while keeping subsystem responsibilities well-defined.

---

# Future Systems

The architecture is designed to accommodate additional systems without structural redesign.

Planned additions include:

- Scene hierarchy
- Asset management
- Material system
- Serialization
- Undo / Redo
- Performance profiler
- AI assistant
- Timeline
- Plugin framework
- Python scripting
- Hand tracking
- Voice interaction
- Networking

Every new system should integrate through existing interfaces rather than introducing new dependency patterns.

---

# Guiding Principles

Every system in Aetheris should:

- Own a single responsibility.
- Operate independently of unrelated systems.
- Expose clear public interfaces.
- Reuse existing engine infrastructure whenever possible.
- Integrate cleanly into the overall architecture.

As the engine evolves, new functionality should extend existing systems rather than replacing or duplicating them.