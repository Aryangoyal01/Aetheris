#pragma once
#include "raylib.h"

enum UITab {
    TAB_PARAMETERS,
    TAB_ENTITIES
};

enum ParticleType {
    PARTICLE_TYPE_A, // Singularity Well (Sphere)
    PARTICLE_TYPE_B, // Recursive Repeller (Cube)
    PARTICLE_TYPE_C  // Orbital Pulsar (Cylinder)
};

struct ParticleInstance {
    ParticleType type;      
    Vector3 position;       
    Vector3 velocity;       
    Color color;            
    bool active;            
    float life;             // NEW: Current lifetime remaining
    float maxLife;          // NEW: Total lifetime for fading scale
};

struct EngineSettings {
    UITab activeTab = TAB_PARAMETERS;
    int activeObjectType = 0; 

    float gravityStrength = 9.81f;
    float frictionCoefficient = 0.15f;
    float elasticityValue = 0.75f;
    
    // Increased default for the new delta-time math
    float smoothingFactor = 12.0f; 
};