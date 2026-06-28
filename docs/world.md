# World Model

The simulation exists entirely in a Cartesian coordinate system.

There is no perspective projection.

There is no hidden third dimension.

There is no camera rotation.

There is only one world coordinate system.

World Coordinates

(x, y)

Every particle exists only in world coordinates.

Rendering projects world coordinates onto the screen using an orthographic projection.

Zoom changes only projection scale.

Pan changes only the visible world origin.

ScreenToWorld() and WorldToScreen() must always remain exact mathematical inverses.

The world is finite during early milestones and may become configurable in future versions.


# Engine Rules

There is exactly one World.

There is exactly one coordinate system.

There is exactly one projection implementation.

Simulation never renders.

Renderer never simulates.

Application orchestrates only.

World is the source of truth.

GPU owns particle memory.

CPU owns orchestration.

Every checkpoint requires manual verification.

No feature may violate these rules.