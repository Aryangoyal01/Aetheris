# AETHERIS — Roadmap

> This document outlines the long-term development roadmap of Aetheris.
>
> Each milestone represents a major stage in the evolution of the engine. Detailed implementation history is maintained separately in `checkpoints.md`.

---

# Project Progress

| Milestone | Status |
|-----------|--------|
| Engine Foundation | ✅ Complete |
| Interactive Workspace | ✅ Complete |
| Physics Foundation | ✅ Complete |
| Professional Editor (Checkpoint 2) | ✅ Complete |
| GPU Compute Pipeline | ✅ Complete |
| GPU Particle Systems | 🚧 Current |
| Scene & Asset System | ⏳ Planned |
| AI Integration | ⏳ Planned |
| Advanced Editor & Engineering Tools | ⏳ Planned |
| Production Release | ⏳ Future |

---

# ✅ Milestone 0 — Engine Foundation

Established the core infrastructure required for the engine.

Completed:

- Application framework
- Build system
- Logging
- Time management
- Input system
- Math library
- Documentation framework

---

# ✅ Milestone 1 — Interactive Workspace

Built the interactive scientific workspace.

Completed:

- Cartesian workspace
- Orthographic viewport
- Infinite engineering grid
- Camera controls
- Coordinate transformations
- Particle visualization

---

# ✅ Milestone 2 — Physics Foundation

Implemented the deterministic CPU simulation used as the reference implementation.

Completed:

- World model
- Particle system
- Spawn system
- Gravity
- Collision handling
- Simulation settings
- Deterministic fixed timestep

The CPU simulation remains the correctness reference for GPU validation.

---

# ✅ Milestone 3 — Professional Editor (Checkpoint 2)

Established a production-quality desktop editor.

Completed:

### Workspace

- Dockable layout
- Fixed engineering workspace
- Navigation rail
- Toolbar
- Status telemetry
- Developer Console (live log streaming, severity badges, interactive command prompt) [Verified in automated tests; Manually Verified in GUI]

### Inspector

- Standardized property panels
- Unified spacing and typography
- Capacity visualization
- Quick Actions
- Reusable section layout

### Widget System

- Theme-driven UI framework
- Reusable Vector2Control
- Cartesian / Polar editing
- Interactive direction dial
- Shared property controls

The editor architecture is now considered stable. Future UI work should primarily consist of extending existing reusable components rather than redesigning the interface.

---

# ✅ Milestone 4 — GPU Compute Pipeline

Migrated the simulation pipeline to the GPU.

Completed:

- Compute shader framework
- SSBO infrastructure
- GPU particle storage
- GPU simulation
- GPU instanced rendering
- CPU/GPU validation pipeline
- Automatic high-performance GPU selection (Optimus / PowerXpress)

The engine now executes its primary simulation entirely on the GPU while retaining the CPU implementation as a validation reference.

---

# 🚧 Milestone 5 — GPU Particle Systems & Physics Foundation

**Current Development Focus**

Objectives:

### Particle Emission
- ✅ Continuous high-rate spawning with sub-frame spatial interpolation (anti-clustering) [Verified in automated tests; Manually Verified in GUI; CPU SpawnSystem → SSBO upload]
- ✅ Frame-rate independent spawn accumulation [Verified in automated tests; Manually Verified in GUI]
- Burst emitters [Planned]
- Multi-shape emitters (disc, line, cone) [Planned]
- GPU-native compute emitter kernels [Planned]

### Particle Initialization
- Velocity distributions and randomization [Planned]
- Initial orientation and angular velocity [Planned]
- Initial size and mass distributions [Planned]
- Initial lifetime parameters [Planned]

### Simulation & Lifecycle
- In-place GPU dead particle compaction and capacity reclamation (resolving CPU-side capacity retention of inactive particles) [Planned]
- GPU timer query telemetry (replacing unmetered GPU time display) [Planned]
- Improved lifecycle management (color and size over lifetime) [Planned]
- Higher particle capacities and GPU memory optimization [Planned]
- GPU/CPU validation parity suite [Planned]

### Scientific Physics Visualization
- Trajectory path ribbons and streamlines [Planned]
- Dynamic force vector fields (gravity, drag, electric) [Planned]
- Velocity and kinetic energy heatmaps [Planned]
- Phase space and energy conservation diagnostic plots [Planned]

This milestone establishes the computational particle foundation required for interactive physics visualization, experimentation, and eventual physics-problem solving.

---

# 📋 Milestone 6 — Scene & Asset System

Expand Aetheris into a complete engineering editor.

Planned:

- Scene hierarchy
- Entity management
- Asset browser
- Material editor
- Project serialization
- Undo / Redo
- Selection tools
- Transform gizmos

---

# 🤖 Milestone 7 — AI Integration

Introduce AI as a first-class editor subsystem.

Planned:

- Natural language editing
- Scene generation
- Parameter editing
- Workflow automation
- Intelligent engineering assistance

AI should always interact through public engine interfaces rather than manipulating subsystems directly.

---

# ⚡ Milestone 8 — Advanced Engineering Tools

Expand Aetheris into a professional scientific platform.

Planned:

- Performance profiler
- GPU debugging tools
- Timeline
- Graph editor
- Plugin framework
- Python scripting
- Networking
- Remote control
- Advanced diagnostics

---

# 🌐 Milestone 9 — Natural Interaction

Extend beyond traditional desktop workflows.

Planned:

- Hand tracking
- Gesture recognition
- Voice commands
- Multi-modal interaction
- XR visualization

---

# 🚀 Milestone 10 — Production Release

Prepare Aetheris for long-term public use.

Objectives:

- Performance optimization
- Cross-platform support
- Automated testing
- Packaging
- Documentation completion
- Stable plugin ecosystem
- Public releases

---

# Long-Term Vision

Aetheris aims to become a professional GPU-native scientific simulation platform that combines:

- High-performance simulation
- GPU compute
- Professional editor tooling
- AI-assisted engineering workflows
- Advanced visualization
- Natural interaction
- Extensible modular architecture

Every milestone should strengthen the existing architecture rather than introduce unnecessary redesigns. New systems should integrate into the established framework, preserving modularity, maintainability, and long-term scalability.