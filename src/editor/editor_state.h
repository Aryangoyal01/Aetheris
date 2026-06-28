#pragma once

enum class InspectorMode
{
    Physics,
    Particles,
    Environment,
    Scene,
    Camera,
    Assets,
    Console,
    Settings
};

struct ImFont;

struct EditorState
{
    InspectorMode activeMode = InspectorMode::Physics;
    
    ImFont* fontTitle = nullptr;
    ImFont* fontHeader = nullptr;
    ImFont* fontDefault = nullptr;
    ImFont* fontCaption = nullptr;
};
