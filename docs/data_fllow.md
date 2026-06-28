# Data Flow

Every subsystem communicates through explicit ownership.

Input

↓

SpawnRequest

↓

SpawnSystem

↓

World

↓

Simulation

↓

Renderer

↓

Screen

Future systems follow the same rule.

Hand Tracking

↓

SpawnRequest

↓

World

Voice

↓

AI

↓

SpawnRequest

↓

World

Networking

↓

Commands

↓

World

No subsystem bypasses the World.    