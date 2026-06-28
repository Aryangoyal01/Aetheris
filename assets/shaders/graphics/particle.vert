#version 430

layout(location = 0) in vec2 aPosition; // Unit quad from -1.0 to 1.0

struct GPUParticle
{
    vec2 position;      // Offset 0
    vec2 velocity;      // Offset 8
    float lifetime;     // Offset 16
    float maxLifetime;  // Offset 20
    float size;         // Offset 24
    uint color;         // Offset 28
    uint isActive;      // Offset 32
    uint padding;       // Offset 36
};

layout(std430, binding = 0) readonly buffer ParticleBuffer
{
    GPUParticle particles[];
};

uniform vec2 u_resolution;
uniform vec2 u_origin;
uniform float u_zoom;

out vec2 vLocalPos;
out vec4 vColor;

void main()
{
    GPUParticle p = particles[gl_InstanceID];

    // If particle is inactive, move it completely off-screen by outputting NaN or far away
    if (p.isActive == 0)
    {
        gl_Position = vec4(10.0, 10.0, 10.0, 1.0); // Outside NDC (-1 to 1)
        vLocalPos = vec2(0.0);
        vColor = vec4(0.0);
        return;
    }

    // Unpack color (RGBA packed in uint)
    uint c = p.color;
    float r = float(c & 0xFFu) / 255.0;
    float g = float((c >> 8u) & 0xFFu) / 255.0;
    float b = float((c >> 16u) & 0xFFu) / 255.0;
    float a = float((c >> 24u) & 0xFFu) / 255.0;
    vColor = vec4(r, g, b, a);
    
    // Scale quad by radius
    vLocalPos = aPosition;
    vec2 worldPos = p.position + aPosition * p.size;
    
    // Calculate NDC (Normalized Device Coordinates)
    vec2 ndc;
    ndc.x = (worldPos.x - u_origin.x) * u_zoom * 2.0 / u_resolution.x;
    ndc.y = (worldPos.y - u_origin.y) * u_zoom * 2.0 / u_resolution.y;

    gl_Position = vec4(ndc, 0.0, 1.0);
}
