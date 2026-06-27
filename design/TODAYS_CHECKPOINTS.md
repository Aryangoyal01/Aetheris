Milestone 1.5 — Checkpoint 1

We are redesigning Aetheris into a professional simulation editor.

Do NOT change the visual appearance yet.

First refactor the UI architecture.

Goals:

* Separate layout calculation from rendering.
* Introduce reusable UI components.
* Eliminate hardcoded coordinates where practical.
* Create a modular panel system.
* Create reusable:

  * Panel
  * Button
  * Toggle
  * Slider
  * Tab
  * Label
  * Status Indicator

The existing functionality must continue to work exactly as before.

Do not add new features.

Do not redesign networking or rendering.

After implementation, summarize:

* New files
* Modified files
* New classes/functions
* Verification steps

Checkpoint 2

Replace the current interface with the new editor layout.

Build the following regions:

1. Top Toolbar
2. Left Sidebar
3. Center Viewport
4. Right Sidebar
5. Bottom Dock

Use placeholder panels where necessary.

No new functionality is required.

Only build the editor layout.

The simulation should continue rendering inside the center viewport.

Everything must resize correctly with the window.

After completion, summarize the implementation.

Checkpoint 3

Implement the Physics panel.

Include controls for:

* Gravity Strength
* Gravity Direction (2D vector)
* Time Scale
* Friction
* Elasticity
* Air Resistance
* Collision Mode

Only connect controls that already exist.

For future controls, create disabled placeholders.

Do not change simulation behavior beyond connecting existing settings.

Maintain the visual design consistency.

Checkpoint 4

Implement the Performance panel.

Display:

* FPS
* Frame Time
* Compute Time
* Render Time
* Alive Particles
* Maximum Capacity
* Memory Usage
* GPU Vendor
* OpenGL Version

If a metric is not yet available, display a placeholder with clear labeling.

Design the panel so metrics can be updated every frame.

Do not implement GPU profiling yet.

Checkpoint 5

Implement the AI panel.

The panel should resemble an AI assistant interface.

Include:

* AI Status
* Listening indicator
* Prompt history
* Input field
* Execute button
* Clear history button

The panel is currently a UI placeholder.

No networking or Gemini logic should be modified.

The goal is to finalize the editor layout only.

Checkpoint 6

Implement the bottom dock.

Create tabs:

* Console
* Logs
* Profiler
* Network
* AI

Only Console and Logs require actual content.

The remaining tabs may contain placeholders.

The dock should be collapsible in the future, but only the visual framework is required now.

Checkpoint 7

Perform a complete visual polish pass.

Goals:

* Modern neon styling
* Consistent spacing
* Responsive layout
* Smooth hover effects
* Consistent typography
* Professional engineering aesthetic

Do not introduce unnecessary animations.

Do not sacrifice readability.

The application should resemble a professional engineering tool rather than a game.

After completion:

1. List every modified file.
2. Explain every architectural change.
3. Describe the verification steps.
4. Identify any remaining UI work for future milestones.
