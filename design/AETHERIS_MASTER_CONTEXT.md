# AETHERIS — Master Context

> **Authoritative Project Context**
>
> This document is the single source of truth for Project Aetheris.
> Any developer or AI agent joining the project should read this document before making architectural or implementation changes.

---

# Project Overview

Project Aetheris is a real-time GPU-accelerated spatial simulation engine built around modern OpenGL compute shaders. It combines GPU physics, computer vision, networking, and generative AI into a modular interactive simulation platform.

The long-term objective is to create a scalable engine capable of simulating hundreds of thousands of particles while allowing natural interaction through hand tracking, voice, and AI-generated behaviors.

---

# Current Project Status

**Project Version**

v0.1.0

**Current Phase**

GPU Compute Foundation Complete

**Current Milestone**

✅ Milestone 1 — GPU Compute Pipeline (Completed)

**Next Active Milestone**

🔄 Milestone 1.5 — Simulation Polish

---

# Milestone 1 Summary

Successfully completed:

* OpenGL 4.3 Compute Context
* RTX 4050 GPU Selection
* Modern OpenGL Function Loader
* SSBO-based GPU Particle Storage
* Compute Shader Pipeline
* GPU Physics Simulation
* Instanced Particle Rendering
* Mouse Spawn Pipeline
* Stable Runtime
* Responsive UI
* Clean Initialization & Shutdown

Major issues resolved during development:

* Incorrect OpenGL 3.3 context
* Hybrid GPU selecting integrated graphics
* SSBO allocation failures
* Access violation (0xC0000005)
* Incorrect instance rendering
* Rendering every allocated particle instead of alive particles
* Spawn pipeline disconnected when network unavailable
* Incorrect MVP matrix order

---

# Core Design Philosophy

The project follows several non-negotiable architectural principles.

* GPU-first simulation
* Modular subsystems
* Minimal CPU↔GPU synchronization
* Runtime scalability
* Future XR compatibility
* AI as a first-class control system

Particle physics must never return to the CPU.

---

# High-Level Architecture

```text
Perception Layer (Python)
        │
        ▼
Network Layer (UDP)
        │
        ▼
Execution Layer (C++)
        │
        ▼
Renderer
    ├── GPUAllocator
    ├── ComputePipeline
    ├── ParticleRenderer
    └── PostProcess
        │
        ▼
GPU Compute Shader
        │
        ▼
Instanced Rendering
```

---

# Rendering Pipeline

Raylib Responsibilities

* Window
* Camera
* UI
* Input
* Basic rendering framework

Modern OpenGL Responsibilities

* Compute shaders
* Shader Storage Buffer Objects
* Instanced rendering
* GPU particle simulation
* Custom particle shaders

CPU responsibilities are limited to spawning particles and high-level engine control.

---

# Particle Pipeline

```text
CPU Input

↓

Spawn Queue

↓

GPUAllocator

↓

SSBO Upload

↓

Compute Shader

↓

Memory Barrier

↓

Particle Renderer

↓

Screen
```

---

# Locked Engineering Decisions

## Rendering

Hybrid architecture.

Raylib is retained for platform abstraction, camera handling, and UI.

Modern OpenGL performs all GPU simulation and particle rendering.

---

## Particle Capacity

Maximum Capacity

500,000 particles

Runtime Active Count

Runtime adjustable.

Only alive particles are rendered.

---

## Memory

Persistent SSBO allocation.

No recreation every frame.

Spawn queue uploads only newly created particles.

---

## Physics

Entirely GPU-based.

The CPU never updates particle positions.

---

## Networking

UDP communication.

Versioned packet protocol.

Supports:

* Tracking packets
* AI JSON payloads

---

## AI Pipeline

```text
InputManager

↓

Prompt Queue

↓

Gemini

↓

Structured JSON

↓

UDP

↓

Engine Settings

↓

GPU Simulation
```

Supports future:

* Keyboard
* Voice
* Gesture
* GUI
* XR

without architectural redesign.

---

# Current Directory Structure

```text
src/

    renderer/
        GPUAllocator
        ComputePipeline
        ParticleRenderer
        Renderer

    shaders/
        compute/
        particle rendering/
        postprocess/

    network.*
    settings.*
    ui.*
    main.cpp

scripts/

    tracker.py

    cognition/
        InputManager
        GeminiClient
```

---

# Current Roadmap

## ✅ Milestone 1

GPU Compute Pipeline

Completed

---

## 🔄 Milestone 1.5

Simulation Polish

Current focus:

* Particle lifetime fade
* Physics tuning
* Lighting improvements
* Diagnostics overlay
* Runtime particle controls

---

## ⏳ Milestone 2

Networking

* Versioned protocol
* JSON payloads
* Runtime AI parameter updates

---

## ⏳ Milestone 3

Gemini Cognition Layer

Natural language control.

---

## ⏳ Milestone 4

Spatial Interaction

* MediaPipe
* Gesture controls
* Temporal wheel
* AI blueprint materialization

---

## ⏳ Milestone 5

Visual Effects

* Bloom
* HDR
* Better materials
* Post-processing
* GPU optimizations

---

# Coding Rules

Never:

* recreate GPU buffers every frame
* call glFinish() in the frame loop
* read back SSBOs every frame
* map GPU buffers every frame
* perform particle physics on the CPU

Always prefer:

* persistent buffers
* batched uploads
* compute shaders
* instanced rendering
* modular subsystems

---

# Known Outstanding Work

Current work items are polish, not architectural blockers.

* Particle lifetime fade
* Physics feel tuning
* Improved lighting
* Runtime diagnostics
* Performance profiling for higher particle counts

---

# Long-Term Vision

Project Aetheris is intended to become a modular real-time simulation engine where GPU physics, AI reasoning, and natural interaction operate together within a unified architecture. Future extensions—including voice, XR, advanced rendering, and new simulation types—should integrate without requiring major redesign of the execution pipeline.

---

**End of Master Context**
