# Aetheris Engineering Rules

## Editor Architecture Rules

**Editor controls must never modify the simulation directly.**

The flow must always be:
Editor UI -> EditorState / Requests -> SimulationSettings or Command -> Simulation / Spawn System -> World -> Renderer

The editor should never mutate the World or ParticleSystem directly.

If an action requires modifying particles (Clear, Spawn, Reset, etc.), route it through the appropriate engine system.

This rule will remain in effect for the lifetime of Aetheris.

## Editor Modes (Configuration vs Execution)

The editor must clearly distinguish between Configuration (Edit Mode) and Execution (Play/Pause Modes).
- **Edit Mode:** All simulation configurations are active and editable.
- **Play / Pause Modes:** All simulation configuration settings (e.g., Gravity, Capacity, Spawn Rate, etc.) must be strictly read-only and visually greyed out (`ImGui::BeginDisabled`). The camera and playback controls remain active.
