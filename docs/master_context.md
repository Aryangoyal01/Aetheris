# AETHERIS — MASTER CONTEXT

> **Authoritative Project Context**
>
> This document is the single source of truth for Project Aetheris.
> Every developer, contributor, or AI agent must read and understand this document before making architectural or implementation decisions.

---

# Project Overview

Aetheris is a modern, GPU-first **2D simulation platform** that combines real-time physics, artificial intelligence, computer vision, natural interaction, and professional tooling into a unified environment.

It is **not** a game engine.

It is **not** a 3D engine.

It is a professional simulation workspace built around a mathematically correct Cartesian world.

The long-term objective is to simulate hundreds of thousands (eventually millions) of entities entirely on the GPU while allowing interaction through traditional UI, AI, voice, networking, computer vision, and future XR interfaces.

---

# Vision

The engine should resemble professional scientific and creative software rather than a traditional game.

Inspirations include:

* Desmos
* GeoGebra
* Houdini
* Blender (workflow philosophy)
* TouchDesigner
* Processing

Core values:

* Mathematical correctness
* Performance
* Predictability
* Modularity
* Extensibility
* Simplicity

---

# Fundamental Design Philosophy

## One World

There is exactly one World.

Every subsystem communicates through the World.

The World is the single source of truth.

---

## True 2D

Everything exists only in:

(x, y)

There is:

* no perspective
* no hidden Z axis
* no Camera2D
* no Camera3D
* no ray-plane intersection
* no rotation
* no pitch
* no yaw

The engine behaves as a Cartesian graph.

---

## Viewport Philosophy

The viewport behaves like a graphing calculator.

Zoom changes only scale.

Pan changes only origin.

Simulation never knows the viewport exists.

Rendering never modifies simulation.

World coordinates are absolute.

---

# Core Architecture

The engine is organized into independent subsystems.

Application

↓

Input

↓

Spawn System

↓

World

↓

Simulation

↓

Renderer

↓

Viewport

↓

Screen

The Application orchestrates.

Every subsystem has exactly one responsibility.

No circular dependencies are allowed.

---

# Current Simulation Architecture

The CPU implementation exists only as the reference implementation.

Its purpose is:

* verify algorithms
* validate correctness
* provide regression testing

The production engine will use GPU compute.

Once GPU parity is achieved:

* GPU becomes the authoritative simulation backend.
* CPU simulation remains only as a reference implementation for debugging and validation.

---

# World Model

The World owns all simulation state.

Current entities include particles.

Future entities may include:

* constraints
* emitters
* fields
* attractors
* rigid bodies
* simulation tools

No subsystem except Simulation modifies World state.

---

# Rendering Philosophy

Renderer visualizes the World.

Renderer never owns simulation.

Renderer never modifies simulation.

Future renderer responsibilities include:

* particles
* grids
* sandbox
* gizmos
* debug overlays
* editor visualization
* selection
* GPU instanced rendering

---

# GPU Philosophy

GPU compute is a foundational technology.

The GPU owns particle memory.

Simulation executes entirely on the GPU.

The CPU coordinates.

The GPU computes.

The intended pipeline is:

Spawn Requests

↓

GPU Upload

↓

Persistent SSBO

↓

Compute Shader

↓

Memory Barrier

↓

GPU Renderer

↓

Screen

Avoid:

* unnecessary CPU readback
* duplicate particle storage
* CPU-side simulation after GPU transition

---

# Physics Philosophy

Physics is deterministic.

The CPU reference defines expected behavior.

The GPU implementation must reproduce that behavior.

Current features:

* gravity
* lifetime
* world bounds
* collision modes
* spawning

Future features:

* constraints
* attractors
* force fields
* fluids
* procedural emitters
* custom solvers

---

# Coordinate System

There is exactly one coordinate system.

Everything exists in World coordinates.

Only two conversion functions exist:

ScreenToWorld()

WorldToScreen()

They must remain exact mathematical inverses.

No subsystem performs its own coordinate conversion.

---

# Editor Philosophy

The editor is a first-class subsystem.

It controls the simulation.

It never owns simulation.

Future layout:

Toolbar

Left Sidebar

Viewport

Inspector

Bottom Dock

The simulation viewport is always the primary focus.

---

# Artificial Intelligence

AI is a first-class subsystem.

AI communicates through structured engine commands.

AI never manipulates rendering directly.

Future capabilities:

* natural language
* procedural generation
* scene editing
* simulation control
* parameter tuning
* assistant workflows
* autonomous agents

---

# Natural Interaction

Traditional mouse and keyboard are only one input method.

Future input systems include:

* hand tracking
* gestures
* voice
* XR
* networking
* AI agents

Every interaction ultimately produces structured engine commands.

No subsystem should know the origin of an interaction.

---

# Engineering Principles

Always prefer:

* single responsibility
* deterministic behavior
* composition over inheritance
* composition over coupling
* modularity
* readability
* maintainability

Avoid:

* duplicated systems
* duplicated constants
* hidden state
* unnecessary abstraction
* temporary hacks
* architectural debt

Delete obsolete code rather than preserving compatibility layers.

---

# Development Workflow

Development proceeds through milestones.

Each milestone is divided into checkpoints.

Every checkpoint must:

* compile
* preserve previous behavior
* be independently testable
* stop after implementation
* undergo manual visual verification

No checkpoint should require future redesign.

---

# Current Roadmap

Milestone 0

Foundation

✓ Complete

---

Milestone 1

2D Workspace Foundation

✓ Complete

---

Milestone 2

CPU Reference Simulation

✓ Complete

---

Milestone 3

GPU Compute Infrastructure

(Current)

---

Milestone 4

GPU-Native Simulation

---

Milestone 5

Professional Editor

---

Milestone 6

Artificial Intelligence

---

Milestone 7

Natural Interaction

* Hand Tracking
* Gestures
* Voice

---

Milestone 8

Networking

---

Milestone 9

Production

* Optimization
* Serialization
* Plugins
* Advanced Rendering
* Packaging

---

# Success Criteria

Aetheris succeeds when it becomes a stable, extensible, GPU-native simulation platform where:

* GPU physics
* AI
* networking
* computer vision
* natural interaction
* professional editor tooling

operate together inside one coherent architecture without requiring fundamental redesign.

Every implementation decision should move the engine closer to that goal.
