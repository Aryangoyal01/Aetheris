#pragma once

#include "world/particle_system.h"
#include "core_math/bounds.h"
#include <vector>
#include <cstdint>
#include <cstddef>

// GPUParticle Layout
// Mirrors the CPU Particle exactly.
// Explicitly designed to adhere to GLSL std430 alignment rules.
struct alignas(8) GPUParticle
{
    Point2 position;        // Offset 0,  Align 8
    Point2 velocity;        // Offset 8,  Align 8
    float lifetime;         // Offset 16, Align 4
    float maxLifetime;      // Offset 20, Align 4
    float size;             // Offset 24, Align 4
    
    // std430 minimum type is 32-bit for alignment, so pack r,g,b,a into a single uint32_t
    uint32_t color;         // Offset 28, Align 4
    
    // GLSL bool is 4 bytes, so we use uint32_t to match
    uint32_t isActive;      // Offset 32, Align 4
    
    // Explicit padding to ensure struct size is a multiple of its largest alignment (8)
    uint32_t padding;       // Offset 36, Align 4
};

// Compile-time verification for std430 layout
static_assert(sizeof(GPUParticle) == 40, "GPUParticle size must be 40 bytes");
static_assert(alignof(GPUParticle) == 8, "GPUParticle alignment must be 8 bytes");
static_assert(offsetof(GPUParticle, position) == 0, "Offset of position must be 0");
static_assert(offsetof(GPUParticle, velocity) == 8, "Offset of velocity must be 8");
static_assert(offsetof(GPUParticle, lifetime) == 16, "Offset of lifetime must be 16");
static_assert(offsetof(GPUParticle, maxLifetime) == 20, "Offset of maxLifetime must be 20");
static_assert(offsetof(GPUParticle, size) == 24, "Offset of size must be 24");
static_assert(offsetof(GPUParticle, color) == 28, "Offset of color must be 28");
static_assert(offsetof(GPUParticle, isActive) == 32, "Offset of isActive must be 32");
static_assert(offsetof(GPUParticle, padding) == 36, "Offset of padding must be 36");

class GPUParticleBuffer
{
public:
    GPUParticleBuffer();
    ~GPUParticleBuffer();

    // Prevent copying
    GPUParticleBuffer(const GPUParticleBuffer&) = delete;
    GPUParticleBuffer& operator=(const GPUParticleBuffer&) = delete;

    void Create(size_t capacity);
    void Destroy();
    void Upload(const ParticleSystem& particleSystem);
    void Download(std::vector<GPUParticle>& outParticles);
    void Bind(uint32_t bindingIndex);
    
    size_t Capacity() const;

private:
    uint32_t m_ssbo = 0;
    size_t m_capacity = 0;
    size_t m_uploadedCount = 0;
    
    // Staging buffer to avoid per-frame allocations during conversion
    std::vector<GPUParticle> m_stagingBuffer;
};
