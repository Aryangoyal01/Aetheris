#include "gpu_particle_buffer.h"
#include "core/log.h"
#include <string>

#if defined(_WIN32)
extern "C" __declspec(dllimport) void* __stdcall wglGetProcAddress(const char*);
#endif

// OpenGL Constants
constexpr uint32_t GL_SHADER_STORAGE_BUFFER = 0x90D2;
constexpr uint32_t GL_DYNAMIC_DRAW          = 0x88E8;
constexpr uint32_t GL_DYNAMIC_STORAGE_BIT   = 0x0100;

// Function Pointers
typedef void (__stdcall *PFNGLGENBUFFERSPROC)(int, uint32_t*);
typedef void (__stdcall *PFNGLBINDBUFFERPROC)(uint32_t, uint32_t);
typedef void (__stdcall *PFNGLBUFFERDATAPROC)(uint32_t, ptrdiff_t, const void*, uint32_t);
typedef void (__stdcall *PFNGLBUFFERSUBDATAPROC)(uint32_t, ptrdiff_t, ptrdiff_t, const void*);
typedef void (__stdcall *PFNGLDELETEBUFFERSPROC)(int, const uint32_t*);
typedef void (__stdcall *PFNGLBINDBUFFERBASEPROC)(uint32_t, uint32_t, uint32_t);
typedef void (__stdcall *PFNGLGETBUFFERSUBDATAPROC)(uint32_t, ptrdiff_t, ptrdiff_t, void*);

typedef void (__stdcall *PFNGLCREATEBUFFERSPROC)(int, uint32_t*);
typedef void (__stdcall *PFNGLNAMEDBUFFERSTORAGEPROC)(uint32_t, ptrdiff_t, const void*, uint32_t);
typedef void (__stdcall *PFNGLNAMEDBUFFERSUBDATAPROC)(uint32_t, ptrdiff_t, ptrdiff_t, const void*);
typedef void (__stdcall *PFNGLGETNAMEDBUFFERSUBDATAPROC)(uint32_t, ptrdiff_t, ptrdiff_t, void*);

static PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
static PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
static PFNGLBUFFERDATAPROC glBufferData = nullptr;
static PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
static PFNGLBINDBUFFERBASEPROC glBindBufferBase = nullptr;
static PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = nullptr;

static PFNGLCREATEBUFFERSPROC glCreateBuffers = nullptr;
static PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage = nullptr;
static PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData = nullptr;
static PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData = nullptr;

static bool s_glLoaded = false;

static void LoadGLFunctions()
{
    if (s_glLoaded) return;

#if defined(_WIN32)
    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
    glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)wglGetProcAddress("glGetBufferSubData");

    glCreateBuffers = (PFNGLCREATEBUFFERSPROC)wglGetProcAddress("glCreateBuffers");
    glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)wglGetProcAddress("glNamedBufferStorage");
    glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glNamedBufferSubData");
    glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glGetNamedBufferSubData");
#endif

    s_glLoaded = true;
}

GPUParticleBuffer::GPUParticleBuffer()
{
}

GPUParticleBuffer::~GPUParticleBuffer()
{
    Destroy();
}

void GPUParticleBuffer::Create(size_t capacity)
{
    LoadGLFunctions();

    if (m_ssbo != 0)
    {
        Log::Warning("GPUParticleBuffer::Create called but SSBO already exists. Ignoring.");
        return;
    }

    m_capacity = capacity;
    m_stagingBuffer.reserve(m_capacity);

    size_t bufferSize = m_capacity * sizeof(GPUParticle);

    if (glCreateBuffers && glNamedBufferStorage)
    {
        // Use Direct State Access (OpenGL 4.5)
        glCreateBuffers(1, &m_ssbo);
        glNamedBufferStorage(m_ssbo, bufferSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
    }
    else if (glGenBuffers)
    {
        // Fallback to older OpenGL 4.3 bindings
        glGenBuffers(1, &m_ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    else
    {
        Log::Error("OpenGL buffer functions not available!");
        return;
    }

    std::string msgSize = "Buffer Size: " + std::to_string(bufferSize) + " bytes";
    std::string msgCap = "Particle Capacity: " + std::to_string(m_capacity);
    
    Log::Info("SSBO Created");
    Log::Info(msgSize.c_str());
    Log::Info(msgCap.c_str());
    Log::Info("Upload System Ready");
}

void GPUParticleBuffer::Destroy()
{
    if (m_ssbo != 0)
    {
        if (glDeleteBuffers)
        {
            glDeleteBuffers(1, &m_ssbo);
        }
        m_ssbo = 0;
        m_capacity = 0;
        m_uploadedCount = 0;
        Log::Info("SSBO Destroyed");
    }
}

void GPUParticleBuffer::Upload(const ParticleSystem& particleSystem)
{
    if (m_ssbo == 0) return;

    const auto& cpuParticles = particleSystem.GetParticles();
    size_t count = cpuParticles.size();
    if (count == 0 || count <= m_uploadedCount) return;

    if (count > m_capacity)
    {
        count = m_capacity; // Do not overflow SSBO
    }

    size_t newParticlesCount = count - m_uploadedCount;

    // Prepare staging buffer
    m_stagingBuffer.clear();
    for (size_t i = m_uploadedCount; i < count; ++i)
    {
        const auto& p = cpuParticles[i];
        
        GPUParticle gpuP;
        gpuP.position = p.position;
        gpuP.velocity = p.velocity;
        gpuP.lifetime = p.lifetime;
        gpuP.maxLifetime = p.maxLifetime;
        gpuP.size = p.size;
        
        // Pack RGBA into uint32_t (ABGR in memory on little endian for compatibility if needed, but RGBA here)
        gpuP.color = (static_cast<uint32_t>(p.a) << 24) |
                     (static_cast<uint32_t>(p.b) << 16) |
                     (static_cast<uint32_t>(p.g) << 8) |
                     (static_cast<uint32_t>(p.r));
                     
        gpuP.isActive = p.active ? 1 : 0;
        gpuP.padding = 0;
        
        m_stagingBuffer.push_back(gpuP);
    }

    size_t uploadSize = newParticlesCount * sizeof(GPUParticle);
    size_t uploadOffset = m_uploadedCount * sizeof(GPUParticle);

    if (glNamedBufferSubData)
    {
        // Direct State Access (OpenGL 4.5)
        glNamedBufferSubData(m_ssbo, uploadOffset, uploadSize, m_stagingBuffer.data());
    }
    else if (glBindBuffer && glBufferSubData)
    {
        // Fallback for OpenGL 4.3
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, uploadOffset, uploadSize, m_stagingBuffer.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
    
    m_uploadedCount = count;
}

void GPUParticleBuffer::Download(std::vector<GPUParticle>& outParticles)
{
    if (m_ssbo == 0) return;
    
    outParticles.resize(m_capacity);
    size_t bufferSize = m_capacity * sizeof(GPUParticle);
    
    if (glGetNamedBufferSubData)
    {
        glGetNamedBufferSubData(m_ssbo, 0, bufferSize, outParticles.data());
    }
    else if (glBindBuffer && glGetBufferSubData)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, bufferSize, outParticles.data());
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}

void GPUParticleBuffer::Bind(uint32_t bindingIndex)
{
    if (m_ssbo != 0 && glBindBufferBase)
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, m_ssbo);
    }
}

size_t GPUParticleBuffer::Capacity() const
{
    return m_capacity;
}
