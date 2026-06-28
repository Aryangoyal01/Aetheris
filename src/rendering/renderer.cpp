#include "renderer.h"
#include "viewport/viewport.h"
#include "world/world.h"
#include "raylib.h"
#include "rlgl.h"
#include "shader/shader.h"
#include "core/log.h"
#include <vector>

// Needed for native OpenGL calls since Raylib's rlgl might hide instanced rendering nuances without setup
// But we can just load the raw gl functions
#if defined(_WIN32)
extern "C" __declspec(dllimport) void* __stdcall wglGetProcAddress(const char*);
typedef void (__stdcall *PFNGLDRAWELEMENTSINSTANCEDPROC)(unsigned int mode, int count, unsigned int type, const void *indices, int instancecount);
static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = nullptr;
#endif

Renderer::Renderer() = default;

Renderer::~Renderer()
{
    if (m_vao != 0) rlUnloadVertexArray(m_vao);
    if (m_vbo != 0) rlUnloadVertexBuffer(m_vbo);
    if (m_ebo != 0) rlUnloadVertexBuffer(m_ebo);
    if (m_shaderId != 0) rlUnloadShaderProgram(m_shaderId);
    if (m_renderTarget.id != 0) UnloadRenderTexture(m_renderTarget);
}

void Renderer::Initialize()
{
#if defined(_WIN32)
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
#endif

    m_shaderId = ShaderProgram::CompileGraphics("assets/shaders/graphics/particle.vert", "assets/shaders/graphics/particle.frag");
    if (m_shaderId != 0)
    {
        Log::Info("GPU Renderer Initialized");
    }

    // Quad geometry (unit quad from -1 to 1)
    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    m_vao = rlLoadVertexArray();
    rlEnableVertexArray(m_vao);

    m_vbo = rlLoadVertexBuffer(vertices, sizeof(vertices), false);
    rlSetVertexAttribute(0, 2, RL_FLOAT, false, 0, 0);
    rlEnableVertexAttribute(0);

    m_ebo = rlLoadVertexBufferElement(indices, sizeof(indices), false);

    rlDisableVertexArray();

    Log::Info("Instanced Quad Renderer Ready");
    Log::Info("Direct GPU Rendering Enabled");
}

void Renderer::BeginFrame()
{
    if (m_renderTarget.id != 0)
    {
        BeginTextureMode(m_renderTarget);
    }
}

void Renderer::Clear()
{
    ClearBackground(BLACK);
}

void Renderer::RenderParticles(uint32_t particleCount)
{
    if (!m_viewport || m_shaderId == 0 || particleCount == 0) return;
    if (!glDrawElementsInstanced) return;

    // Use our custom shader
    rlEnableShader(m_shaderId);

    // Set viewport uniforms
    int resLoc = rlGetLocationUniform(m_shaderId, "u_resolution");
    if (resLoc != -1)
    {
        float res[2] = { m_viewport->Width(), m_viewport->Height() };
        rlSetUniform(resLoc, res, RL_SHADER_UNIFORM_VEC2, 1);
    }

    int originLoc = rlGetLocationUniform(m_shaderId, "u_origin");
    if (originLoc != -1)
    {
        float origin[2] = { m_viewport->OriginX(), m_viewport->OriginY() };
        rlSetUniform(originLoc, origin, RL_SHADER_UNIFORM_VEC2, 1);
    }

    int zoomLoc = rlGetLocationUniform(m_shaderId, "u_zoom");
    if (zoomLoc != -1)
    {
        float zoom = m_viewport->Zoom();
        rlSetUniform(zoomLoc, &zoom, RL_SHADER_UNIFORM_FLOAT, 1);
    }

    // Bind quad VAO and draw instanced
    rlEnableVertexArray(m_vao);
    
    // We assume the SSBO is already bound at binding point 0 by the application
    
    // Draw
    glDrawElementsInstanced(0x0004, 6, 0x1405, nullptr, particleCount); // GL_TRIANGLES, 6 indices, GL_UNSIGNED_INT

    rlDisableVertexArray();
    rlDisableShader();
}

void Renderer::EndFrame()
{
    if (m_renderTarget.id != 0)
    {
        EndTextureMode();
    }
}

void Renderer::ResizeTarget(int width, int height)
{
    if (width <= 0 || height <= 0) return;

    if (m_targetWidth != width || m_targetHeight != height)
    {
        if (m_renderTarget.id != 0)
        {
            UnloadRenderTexture(m_renderTarget);
        }
        
        m_renderTarget = LoadRenderTexture(width, height);
        m_targetWidth = width;
        m_targetHeight = height;
    }
}

void Renderer::SetViewport(const Viewport* viewport)
{
    m_viewport = viewport;
}

GridRenderer& Renderer::GetGridRenderer()
{
    return m_gridRenderer;
}
