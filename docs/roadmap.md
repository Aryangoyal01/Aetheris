# AETHERIS — Roadmap

> This document outlines the long-term development roadmap of Aetheris.
>
> Each milestone represents a major stage in the evolution of the engine. Detailed implementation history is maintained separately in `checkpoints.md`.

---

# Project Progress

| Milestone                                | Status     |
| ---------------------------------------- | ---------- |
| Foundation                               | ✅ Complete |
| Workspace & Rendering                    | ✅ Complete |
| Physics Simulation                       | ✅ Complete |
| Professional Editor (UI Foundation v1.0) | ✅ Complete |
| GPU Compute Pipeline                     | 🚧 Next    |
| Scene & Asset System                     | ⏳ Planned  |
| AI Integration                           | ⏳ Planned  |
| Advanced Editor Tools                    | ⏳ Planned  |
| Production Release                       | ⏳ Future   |

---

# ✅ Milestone 0 — Engine Foundation

Established the core infrastructure required for the engine.

Completed:

* Application framework
* Build system
* Logging
* Time management
* Input system
* Mathematical foundation
* Documentation

---

# ✅ Milestone 1 — Interactive Workspace

Built the interactive 2D simulation environment.

Completed:

* Cartesian workspace
* Orthographic viewport
* World ↔ Screen projection
* Infinite grid
* Camera controls
* Particle visualization

---

# ✅ Milestone 2 — Physics Foundation

Implemented the reference CPU simulation.

Completed:

* World model
* Particle system
* Spawn system
* Gravity
* Collision handling
* Simulation settings
* Deterministic update loop

This implementation serves as the correctness reference for future GPU execution.

---

# ✅ Milestone 3 — Professional Editor (UI Foundation v1.0)

Transformed the engine into a professional desktop editor.

Completed:

* Modern editor interface
* Dockable workspace
* Navigation rail
* Inspector
* Console
* Toolbar
* Status telemetry
* Reusable design system
* Custom editor widgets

The editor foundation is considered stable. Future UI work will be driven by new functionality rather than global redesigns.

---

# 🚧 Milestone 4 — GPU Compute Pipeline

Current Development Focus.

Objectives:

* Compute shader framework
* GPU particle storage (SSBO)
* GPU simulation
* GPU rendering
* CPU/GPU validation
* Fully GPU-native particle pipeline

The CPU implementation will remain available as a reference implementation.

---

# 📋 Milestone 5 — Scene & Editor Systems

Expand the editor into a complete content creation environment.

Planned systems:

* Scene hierarchy
* Entity management
* Asset browser
* Material editor
* Project serialization
* Undo / Redo
* Selection & Gizmos

---

# 🤖 Milestone 6 — AI Integration

Introduce AI as a native editor subsystem.

Planned capabilities:

* Natural language interaction
* Scene generation
* Parameter editing
* Workflow automation
* Intelligent editor assistance

AI should operate through engine interfaces rather than direct subsystem manipulation.

---

# ⚡ Milestone 7 — Advanced Tooling

Expand Aetheris into a professional engineering platform.

Planned systems:

* Performance profiler
* Timeline
* Graph editor
* Plugin framework
* Python scripting
* Networking
* Remote control
* Advanced debugging tools

---

# 🌐 Milestone 8 — Natural Interaction

Extend beyond traditional desktop interaction.

Planned capabilities:

* Hand tracking
* Gesture recognition
* Voice commands
* Multi-modal workflows
* XR exploration

---

# 🚀 Milestone 9 — Production Release

Prepare Aetheris for long-term use and public distribution.

Objectives:

* Performance optimization
* Cross-platform support
* Packaging
* Automated testing
* Documentation completion
* Stable plugin ecosystem
* Public releases

---

# Long-Term Vision

Aetheris aims to become a professional GPU-native scientific simulation platform that combines:

* High-performance particle simulation
* Modern editor tooling
* AI-assisted workflows
* Advanced visualization
* Natural interaction
* Extensible architecture

Every milestone should strengthen the existing architecture rather than introduce unnecessary redesigns, ensuring the engine remains modular, maintainable, and scalable throughout its evolution.
