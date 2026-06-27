# AETHERIS — Milestone 1.5 Specification

## Objective

Transform Aetheris from a working GPU simulation into a professional simulation editor by redesigning the user interface and editor layout while preserving the existing GPU compute pipeline.

This milestone focuses entirely on the editor framework, visual design, layout architecture, and future extensibility.

No major AI, networking, or physics redesigns are part of this milestone unless required to support the editor.

---

# Deliverables

At the completion of Milestone 1.5 the application should resemble a professional simulation editor rather than a technology demonstration.

The editor must be fully functional even if some buttons and panels contain placeholder content.

---

# Design Goals

* Modern neon-themed interface
* Minimal visual clutter
* Simulation remains the primary focus
* Fully modular layout
* Scalable to future milestones
* Responsive to different resolutions
* Consistent spacing, typography, and colors

---

# Editor Layout

The application shall be divided into five major regions.

## 1. Top Toolbar

Contains:

* Project title
* Simulation mode
* Play
* Pause
* Reset
* Screenshot
* Settings
* AI status
* FPS
* GPU status

This bar remains fixed.

---

## 2. Left Sidebar

Contains collapsible modules.

### Physics

Controls:

* Gravity Strength
* Gravity Direction (2D vector)
* Time Scale
* Friction
* Elasticity
* Air Resistance
* Collision Mode

---

### Particles

Controls:

* Particle Type
* Spawn Rate
* Lifetime
* Size
* Color
* Glow
* Trail

---

### Environment

Controls:

* Background
* Grid
* Lighting
* Wind
* Boundaries

---

### AI

Contains:

* AI status
* Listening state
* Prompt history
* Execute command
* Undo previous command

Initially placeholder.

---

### Performance

Displays:

* FPS
* Frame time
* GPU time
* Compute time
* Render time
* Alive particles
* Maximum capacity
* Memory usage

---

### Scene

Contains:

* Save
* Load
* Reset
* Export
* Import

Initially placeholder.

---

## 3. Central Viewport

Primary focus of the application.

The viewport will display a 2D simulation.

Requirements:

* Infinite dark background
* Minimal grid
* Responsive resizing
* Smooth camera pan
* Future zoom support
* Simulation occupies maximum available space

No unnecessary UI overlays.

---

## 4. Right Sidebar

Inspector panel.

Displays:

* Selected particle
* Position
* Velocity
* Lifetime
* Type
* Color
* Internal identifier

If nothing is selected, display engine information.

---

## 5. Bottom Dock

Tabbed interface.

Tabs:

* Console
* Logs
* Profiler
* Network
* AI

Initially only Console and Logs require implementation.

---

# UI Theme

Primary colors:

Background:
#090B14

Panels:
#111827

Borders:
Neon cyan

Highlights:
Bright cyan

Warnings:
Amber

Errors:
Red

Text:
White and light gray

No bright gradients or excessive glow.

The interface should resemble a modern engineering tool rather than a game.

---

# Simulation

The simulation shall become fully 2D.

Requirements:

* Gravity configurable in any direction
* Mouse interaction preserved
* Hand tracking preserved
* Future AI interaction preserved

No perspective camera.

---

# Performance

The redesigned UI shall not noticeably reduce simulation performance.

Target:

60 FPS or higher with current workload.

---

# Technical Tasks

* Refactor ui.cpp into modular panel rendering
* Separate layout calculation from widget rendering
* Introduce reusable panel drawing helpers
* Introduce reusable button component
* Introduce reusable slider component
* Introduce reusable tab component
* Introduce reusable status widget
* Support window resizing
* Remove hardcoded coordinates where possible

---

# Out of Scope

The following items are explicitly deferred:

* Networking redesign
* AI implementation changes
* Scene serialization
* Shader hot reload
* GPU profiling improvements
* Post-processing pipeline
* Physics redesign

---

# Acceptance Criteria

Milestone 1.5 is complete when:

* The application visually resembles a professional simulation editor.
* Every planned editor region exists.
* Navigation between panels functions correctly.
* The simulation remains fully operational.
* Existing GPU compute functionality is preserved.
* The project is ready for future feature integration without further UI redesign.
