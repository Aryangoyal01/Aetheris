#version 430

struct ParticleData {
    vec4 position;  // x, y, z, life
    vec4 velocity;  // vx, vy, vz, maxLife
    vec4 color;     // r, g, b, a
    int type;       // 0, 1, 2
    int isActive;     // 1 or 0
    float padding[2];
};

layout(std430, binding = 0) buffer ParticleBuffer {
    ParticleData particles[];
};

uniform float dt;
uniform float gravityStrength;
uniform float frictionCoefficient;
uniform float elasticityValue;

// Telemetry parameters for interaction
uniform vec3 handWorldPos;
uniform int handDetected;
uniform int isPinching;
uniform int activeObjectType;

// Attract/Repulse strength and radius
uniform float interactionStrength; // positive for attract, negative for repulse
uniform float interactionRadius;

layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

void main() {
    uint id = gl_GlobalInvocationID.x;
    
    // Safety boundary check
    if (id >= particles.length()) return;
    if (particles[id].isActive == 0) return;

    // 1. Update Lifetime
    particles[id].position.w -= dt; // position.w stores current life
    if (particles[id].position.w <= 0.0) {
        particles[id].isActive = 0;
        return;
    }

    // 2. Spatial Hand Interactivity
    if (handDetected == 1 && isPinching == 1) {
        vec3 pPos = particles[id].position.xyz;
        vec3 dir = handWorldPos - pPos;
        float dist = length(dir);
        
        if (dist > 0.01 && dist < interactionRadius) {
            // Apply a force depending on activeObjectType or interactionStrength
            // Type 0 (Singularity): Pulls particles in (attractor)
            // Type 1 (Repeller): Pushes particles out (repeller)
            // Type 2 (Orbital): Orbital pull (creates swirl)
            
            vec3 force = vec3(0.0);
            if (particles[id].type == 0) {
                // Attractor force
                force = normalize(dir) * (15.0 / (dist + 0.5));
            } else if (particles[id].type == 1) {
                // Repeller force
                force = -normalize(dir) * (25.0 / (dist + 0.2));
            } else if (particles[id].type == 2) {
                // Orbital pulsar force (gravitational pull + tangential velocity)
                vec3 normDir = normalize(dir);
                vec3 tangent = normalize(cross(normDir, vec3(0.0, 1.0, 0.0)));
                force = normDir * (10.0 / (dist + 0.5)) + tangent * (15.0 / (dist + 0.5));
            }
            
            // Add force to velocity
            particles[id].velocity.xyz += force * dt;
        }
    }

    // 3. Update Physics (Gravity & Motion)
    particles[id].velocity.y -= gravityStrength * dt;
    particles[id].position.xyz += particles[id].velocity.xyz * dt;

    // 4. Ground Collision Resolution
    if (particles[id].position.y <= 0.0) {
        particles[id].position.y = 0.0;
        particles[id].velocity.y = -particles[id].velocity.y * elasticityValue;
        particles[id].velocity.x *= (1.0 - frictionCoefficient);
        particles[id].velocity.z *= (1.0 - frictionCoefficient);
    }
}
