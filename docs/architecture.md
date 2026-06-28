# AETHERIS — ARCHITECTURE

> This document defines the architectural rules of Aetheris.
>
> Every subsystem, feature, and future milestone must conform to the principles described here.
>
> Architecture is considered stable unless explicitly redesigned.

---

# Architecture Philosophy

Aetheris is built using a modular subsystem architecture.

Each subsystem has:

* one responsibility
* clear ownership
* minimal dependencies
* deterministic behavior

Subsystems communicate through well-defined interfaces.

No subsystem should perform another subsystem's responsibility.

---

# High-Level Architecture

```text
                           Application
                                │
 ┌──────────────┬───────────────┼──────────────┬──────────────┐
 │              │               │              │              │
Input   SimulationSettings   Viewport    SpawnSystem   ISimulation
 │              │               │              │              │
 └──────────────┴───────────────┴──────────────┴──────────────┘
                                │
                                ▼
                              World
                                │
                         ParticleSystem
                                │
                                ▼
                            Renderer
                                │
                                ▼
                              Screen
```

The Application owns all major subsystems.

It is responsible only for orchestration.

---

# Subsystem Responsibilities

## Application

Responsible for:

* initialization
* shutdown
* update order
* subsystem lifetime
* configuration
* main loop

Application should never implement simulation logic.

---

## Input

Responsible for:

* keyboard
* mouse
* future controllers
* future hand tracking input

Input only reports state.

Input never modifies the world.

---

## Spawn System

Responsible for:

* converting user intent into particle creation
* spawn timing
* spawn limits
* SpawnRequest processing

SpawnSystem never performs simulation.

---

## World

The World is the single source of truth.

World owns:

* particles
* future entities
* simulation state

Only Simulation modifies World state.

Other systems read the World.

---

## Simulation

Simulation updates the World.

Responsibilities include:

* gravity
* collisions
* lifetime
* future constraints
* force fields
* procedural behaviors

Simulation never performs rendering.

---

## Renderer

Renderer visualizes the World.

Renderer performs:

* grid rendering
* sandbox rendering
* particle rendering
* future debug rendering
* future editor overlays

Renderer never modifies World state.

---

## Viewport

Viewport converts between:

Screen

↓

World

and

World

↓

Screen

Viewport owns:

* zoom
* origin
* projection

Viewport never affects simulation.

---

# Data Ownership

Application owns subsystem lifetime.

World owns simulation data.

Simulation owns particle updates.

Renderer owns visualization only.

Viewport owns projection state.

Input owns input state.

Ownership must never overlap.

---

# Dependency Rules

Allowed:

Application

↓

All subsystems

Input

↓

None

SpawnSystem

↓

World

SimulationSettings

Simulation

↓

World

SimulationSettings

Renderer

↓

World

Viewport

Viewport

↓

Core Math

Forbidden:

Renderer → Simulation

Simulation → Renderer

Input → Renderer

Input → Simulation

Renderer → Input

World → Renderer

World → Simulation

Circular dependencies of any kind.

---

# Update Order

Every frame:

```text
Input

↓

Spawn System

↓

Simulation

↓

Renderer

↓

Present Frame
```

This order must remain stable.

---

# GPU Transition

Current:

```text
Spawn

↓

CPU Simulation

↓

CPU Renderer
```

Future:

```text
Spawn

↓

GPU Upload

↓

Compute Shader

↓

Persistent SSBO

↓

GPU Rendering
```

After GPU migration:

* GPU owns particle memory.
* CPU becomes orchestration only.
* CPU simulation remains only as a reference implementation.

---

# Future Systems

Future subsystems include:

* AI
* Networking
* Hand Tracking
* Voice
* XR
* Scripting

None of these communicate directly with Renderer or Simulation.

They communicate through structured requests and the World.

---

# Architectural Principles

Always follow:

* Single Responsibility Principle
* One source of truth
* Composition over coupling
* Deterministic behavior
* Explicit ownership
* Clear dependency direction

Never introduce:

* duplicated state
* duplicated constants
* hidden ownership
* circular dependencies
* temporary compatibility layers

---

# Architecture Stability

This architecture is intended to remain stable throughout the lifetime of the project.

New features should extend the architecture rather than redesign it.

Fundamental architectural changes should be extremely rare and only occur when they significantly improve correctness, maintainability, or extensibility.
