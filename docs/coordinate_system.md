# Coordinate System

There is exactly one world coordinate system.

Positive X points right.

Positive Y points upward.

Screen Y increases downward.

Projection converts between Screen and World.

There are exactly two conversion functions:

- ScreenToWorld()
- WorldToScreen()

These must remain exact inverses.

No subsystem may implement its own coordinate conversion.

The viewport owns:

- origin
- zoom

The world owns:

- positions
- velocities

Simulation always operates in World coordinates.

Rendering performs the conversion.