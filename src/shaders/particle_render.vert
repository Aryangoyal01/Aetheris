#version 430

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

struct ParticleData {
    vec4 position;
    vec4 velocity;
    vec4 color;
    int type;
    int isActive;
    float padding[2];
};

layout(std430, binding = 0) readonly buffer ParticleBuffer {
    ParticleData particles[];
};

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMatrix;
uniform vec3 viewPos;

out vec3 fragNormal;
out vec3 fragWorldPos;
out vec4 fragColor;
out float fragLife;

void main() {
    ParticleData p = particles[gl_InstanceID];

    float lifeFraction = clamp(p.position.w / max(p.velocity.w, 0.01), 0.0, 1.0);

    vec3 worldOffset = p.position.xyz;
    vec3 worldPos = vertexPosition + worldOffset;
    gl_Position = mvp * vec4(worldPos, 1.0);

    fragNormal = normalMatrix * vertexNormal;
    fragWorldPos = worldPos;
    fragColor = vec4(p.color.rgb, 1.0);
    fragLife = lifeFraction;
}
