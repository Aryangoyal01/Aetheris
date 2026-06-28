# AETHERIS — GPU ARCHITECTURE

> This document defines the long-term GPU simulation architecture of Aetheris.

---

# Philosophy

Aetheris is a GPU-native simulation engine.

The CPU coordinates.

The GPU computes.

The GPU owns simulation state.

The CPU should never perform particle simulation once GPU parity is achieved.

---

# CPU Reference

The existing CPU simulation is retained only for:

- algorithm verification
- debugging
- regression testing

It is not part of the production runtime.

---

# GPU Responsibilities

The GPU performs:

- particle integration
- gravity
- collisions
- constraints
- force fields
- future fluid simulation
- future spatial partitioning

---

# CPU Responsibilities

The CPU performs:

- application lifecycle
- input
- spawning
- AI
- networking
- editor
- resource management

The CPU does not update particles.

---

# Simulation Pipeline

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

---

# Memory Ownership

World owns logical particle state.

GPU owns physical particle memory.

The renderer consumes GPU buffers directly.

Avoid CPU readbacks.

---

# Buffer Layout

Future GPU buffers include:

- Particle Buffer
- Spawn Queue
- Uniform Buffer
- Debug Buffer

---

# Synchronization

Synchronization should be explicit.

Use OpenGL memory barriers only where required.

Avoid unnecessary GPU stalls.

---

# Long-Term Goal

Millions of particles simulated entirely on the GPU with minimal CPU overhead.