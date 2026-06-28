# Spawn Pipeline

Mouse

↓

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

Future systems use the same pipeline.

Hand Tracking

↓

SpawnRequest

Voice

↓

AI

↓

SpawnRequest

Networking

↓

SpawnRequest

Every interaction becomes a SpawnRequest or another structured command.

The simulation never depends on the source of the request.