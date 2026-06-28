#include "application.h"


#include <imgui.h>
#include "core/log.h"
#include "core/time.h"
#include "input/input.h"
#include <raylib.h>
#include <rlgl.h>
#include <string>

extern "C" const unsigned char* __stdcall glGetString(unsigned int name);

#if defined(_WIN32)
extern "C" __declspec(dllimport) void* __stdcall wglGetProcAddress(const char*);
#endif

typedef void (__stdcall *PFNGLMEMORYBARRIERPROC)(unsigned int);
static PFNGLMEMORYBARRIERPROC s_glMemoryBarrier = nullptr;


void Application::Initialize()
{
    SetTraceLogLevel(LOG_WARNING); // Suppress Raylib INFO logs

    Log::Info("========================================");
    Log::Info("                AETHERIS                ");
    Log::Info("========================================");

    Log::Info("--- ENGINE ---");

#if defined(_MSC_VER)
    const char* compiler = "MSVC";
#elif defined(__clang__)
    const char* compiler = "Clang";
#elif defined(__GNUC__)
    const char* compiler = "GCC";
#else
    const char* compiler = "Unknown";
#endif

#if defined(_M_X64) || defined(__amd64__)
    const char* arch = "x64";
#elif defined(_M_IX86) || defined(__i386__)
    const char* arch = "x86";
#elif defined(__aarch64__)
    const char* arch = "ARM64";
#else
    const char* arch = "Unknown";
#endif

#if defined(NDEBUG)
    const char* buildType = "Release";
#else
    const char* buildType = "Debug";
#endif

    std::string buildInfo = std::string("Build: ") + buildType + " [" + compiler + " " + arch + "]";
    Log::Info(buildInfo.c_str());

    Log::Info("--- STARTUP ---");

    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    // Initialize window with temporary size to properly initialize GLFW and monitors
    InitWindow(1280, 720, "Aetheris");

    int monitor = GetCurrentMonitor();
    int monitorW = GetMonitorWidth(monitor);
    int monitorH = GetMonitorHeight(monitor);
    SetWindowSize(monitorW, monitorH);
    SetWindowPosition(0, 0);

    // SetTargetFPS(60); // Temporarily removed for benchmark

    m_renderer.SetViewport(&m_viewport);

    // Checkpoint 3.1: OpenGL Verification
    Log::Info("--- GRAPHICS ---");
    const char* glVersion = (const char*)glGetString(0x1F02); // GL_VERSION
    const char* glslVersion = (const char*)glGetString(0x8B8C); // GL_SHADING_LANGUAGE_VERSION
    const char* glVendor = (const char*)glGetString(0x1F00); // GL_VENDOR
    const char* glRenderer = (const char*)glGetString(0x1F01); // GL_RENDERER

    std::string glV = std::string("OpenGL Version: ") + (glVersion ? glVersion : "Unknown");
    std::string glslV = std::string("GLSL Version: ") + (glslVersion ? glslVersion : "Unknown");
    std::string vend = std::string("GPU Vendor: ") + (glVendor ? glVendor : "Unknown");
    std::string rend = std::string("GPU Renderer: ") + (glRenderer ? glRenderer : "Unknown");

    Log::Info(glV.c_str());
    Log::Info(glslV.c_str());
    Log::Info(vend.c_str());
    Log::Info(rend.c_str());

    if (rlGetVersion() < RL_OPENGL_43)
    {
        Log::Error("OpenGL 4.3 or greater is required. Initialization failed.");
        CloseWindow();
        exit(-1);
    }

    Log::Info("--- COMPUTE ---");

    // Load MemoryBarrier via wglGetProcAddress
#if defined(_WIN32)
    s_glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)wglGetProcAddress("glMemoryBarrier");
#endif

    // Load validation compute shader
    if (!m_computeShader.Load("assets/shaders/compute/simulate.comp"))
    {
        Log::Error("Failed to load compute simulate shader.");
    }
    else
    {
        Log::Info("GPU Simulation Enabled");
    }
    
    // Initialize GPU Particle Buffer
    m_particleBuffer.Create(m_settings.maxParticles);

    // Initialize GPU Renderer
    m_renderer.Initialize();
    
    // Initialize Editor
    m_editor.Initialize(&m_renderer, &m_viewport);

    Log::Info("Aetheris initialized successfully");
}

void Application::Run()
{
    Log::Info("--- RUNTIME ---");
    Log::Info("Entering main loop");

    while (!WindowShouldClose())
    {
        Time::Update(GetFrameTime());
        Input::Update();

        BeginDrawing();
        ClearBackground(BLACK);

        m_editor.BeginFrame();

        // Window resize is now handled via ViewportPanel sending size back to Renderer/Viewport
        // m_viewport.Resize(...) is no longer driven by the main window here

        // Viewport interaction is strictly gated by whether the user is hovering the viewport panel
        bool isViewportHovered = m_editor.IsViewportHovered();
        Point2 localMouse = m_editor.GetViewportLocalMouse();

        if (isViewportHovered)
        {
            // Zoom
            float wheel = Input::MouseWheel();
            if (wheel != 0.0f)
            {
                float factor = 1.0f + wheel * 0.1f;
                m_viewport.ZoomAtPoint(localMouse.x, localMouse.y, factor);
            }

            // Pan
            if (Input::MouseButtonDown(MOUSE_BUTTON_MIDDLE))
            {
                Point2 delta = Input::MouseDelta();
                m_viewport.Pan(delta.x, delta.y);
            }
        }
        
        bool isPressed = false;
        bool isDown = false;
        
        if (isViewportHovered)
        {
            isPressed = Input::MouseButtonPressed(MOUSE_BUTTON_LEFT);
            isDown = Input::MouseButtonDown(MOUSE_BUTTON_LEFT);
        }
        
        Point2 worldPos = m_viewport.ScreenToWorld(localMouse);
        
        SpawnRequest req;
        req.position = worldPos;
        req.velocity = { 0.0f, 0.0f };
        req.radius = m_settings.defaultRadius;
        req.lifetime = m_settings.defaultLifetime;
        req.color = WHITE;

        m_spawnSystem.Update(m_world, Time::DeltaTime(), isPressed, isDown, req, m_settings);

        // GPU Upload
        m_particleBuffer.Upload(m_world.GetParticleSystem());

        // Compute Shader Execution
        m_computeShader.Bind();
        m_particleBuffer.Bind(0); // Bind SSBO to binding index 0

        // Set Uniforms
        m_computeShader.SetUniformFloat("u_deltaTime", Time::DeltaTime());
        m_computeShader.SetUniformFloat("u_gravity", m_settings.gravity);
        m_computeShader.SetUniformFloat("u_restitution", m_settings.restitution);
        m_computeShader.SetUniformInt("u_collisionMode", static_cast<int>(m_settings.collisionMode));
        m_computeShader.SetUniformVec4("u_bounds", World::BOUNDS_LEFT, World::BOUNDS_RIGHT, World::BOUNDS_BOTTOM, World::BOUNDS_TOP);

        // Dispatch enough workgroups to cover all particles (64 threads per workgroup)
        uint32_t groupX = (m_settings.maxParticles + 63) / 64;
        m_computeShader.Dispatch(groupX, 1, 1);
        
        if (s_glMemoryBarrier)
        {
            s_glMemoryBarrier(0xFFFFFFFF); // GL_ALL_BARRIER_BITS
        }
        rlDisableShader(); // Unbind compute shader

        // Render
        m_renderer.BeginFrame();
        m_renderer.Clear();

        m_renderer.GetGridRenderer().Render(m_viewport);
        
        // We ensure the particle buffer is bound for the vertex shader to fetch from
        m_particleBuffer.Bind(0); 
        m_renderer.RenderParticles(m_settings.maxParticles);
        
        m_renderer.EndFrame();

        m_editor.Update();
        m_editor.EndFrame();

        EndDrawing();
    }
    
    m_editor.Destroy();
}

void Application::Shutdown()
{
    Log::Info("--- SHUTDOWN ---");
    Log::Info("Cleaning up resources");
    
    m_particleBuffer.Destroy();
    m_computeShader.Unload();
    CloseWindow();
    
    Log::Info("Aetheris terminated cleanly");
}
