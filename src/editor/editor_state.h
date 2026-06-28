#pragma once

enum class InspectorMode
{
    Scene,
    Physics,
    Rendering,
    Simulation,
    Export
};

struct EditorState
{
    InspectorMode activeMode = InspectorMode::Physics;
};
