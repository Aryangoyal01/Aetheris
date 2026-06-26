#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "settings.h"
#include "ui.h"
#include "network.h"
#include "renderer/renderer.h"
#include "renderer/gl_helpers.h"
#include <cmath>
#include <vector>

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

static Renderer g_renderer;

constexpr float TIMEOUT_MAX = 0.20f;

int main() {
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(800, 600, "Aetheris Engine - GPU Pipeline");

    int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
    SetTargetFPS(60);

    if (!LoadGLFunctions()) {
        g_renderer.Shutdown();
        CloseWindow();
        return 1;
    }

    if (!g_renderer.Init("src/shaders/particle_shader.glsl")) {
        g_renderer.Shutdown();
        CloseWindow();
        return 1;
    }

    EngineNetwork::Initialize(8080);
    EngineSettings settings;

    float trackingSensitivityX = 1.35f, trackingSensitivityY = 1.20f;
    float horizontalOffset = 0.0f, verticalOffset = 0.0f;
    bool invertHorizontal = false;

    Vector2 smoothedHandPos = { 400.0f, 300.0f };
    bool lastKnownPinch = false, activeHandVisible = false;
    float trackingTimeout = 0.0f;

    Camera3D camera = { { 0.0f, 25.0f, 35.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (IsKeyPressed(KEY_I)) invertHorizontal = !invertHorizontal;
        if (IsKeyDown(KEY_UP))    trackingSensitivityY += 0.5f * dt;
        if (IsKeyDown(KEY_DOWN))  trackingSensitivityY -= 0.5f * dt;
        if (IsKeyDown(KEY_RIGHT)) trackingSensitivityX += 0.5f * dt;
        if (IsKeyDown(KEY_LEFT))  trackingSensitivityX -= 0.5f * dt;

        trackingSensitivityX = Clamp(trackingSensitivityX, 0.1f, 5.0f);
        trackingSensitivityY = Clamp(trackingSensitivityY, 0.1f, 5.0f);

        EngineNetwork::TrackingData tracking = EngineNetwork::PollTrackingData();
        EngineNetwork::AIPayload aiPayload = EngineNetwork::PollAIPayload();

        static float targetGravity = 9.81f;
        static float targetFriction = 0.15f;
        static float targetElasticity = 0.75f;
        static float targetTimeScale = 1.0f;

        if (aiPayload.hasUpdate) {
            targetGravity = aiPayload.gravity;
            targetFriction = aiPayload.friction;
            targetElasticity = aiPayload.elasticity;
            targetTimeScale = aiPayload.timeScale;
            if (aiPayload.spawnType >= 0 && aiPayload.spawnType <= 2) {
                settings.activeObjectType = aiPayload.spawnType;
            }
        }

        float lerpRate = 1.0f - expf(-3.0f * dt);
        settings.gravityStrength = Lerp(settings.gravityStrength, targetGravity, lerpRate);
        settings.frictionCoefficient = Lerp(settings.frictionCoefficient, targetFriction, lerpRate);
        settings.elasticityValue = Lerp(settings.elasticityValue, targetElasticity, lerpRate);
        settings.timeScale = Lerp(settings.timeScale, targetTimeScale, lerpRate);

        if (tracking.hasNewPacket) {
            trackingTimeout = TIMEOUT_MAX;
            activeHandVisible = tracking.handDetected;
            lastKnownPinch = tracking.isPinching;

            float rawNormX = (invertHorizontal) ? (1.0f - tracking.normX) : tracking.normX;
            float rawNormY = tracking.normY;

            static Vector2 smoothedNorm = { 0.5f, 0.5f };
            float smoothStep = 1.0f - expf(-settings.smoothingFactor * dt);
            smoothedNorm.x = Lerp(smoothedNorm.x, rawNormX, smoothStep);
            smoothedNorm.y = Lerp(smoothedNorm.y, rawNormY, smoothStep);

            float normX = (smoothedNorm.x - 0.5f) * trackingSensitivityX + 0.5f + horizontalOffset;
            float normY = (smoothedNorm.y - 0.5f) * trackingSensitivityY + 0.5f + verticalOffset;

            smoothedHandPos = { Clamp(normX, 0.0f, 1.0f) * GetScreenWidth(),
                                Clamp(normY, 0.0f, 1.0f) * GetScreenHeight() };
        } else {
            trackingTimeout -= dt;
            if (trackingTimeout <= 0.0f) activeHandVisible = false;
            smoothedHandPos = GetMousePosition();
        }

        static float pinchDuration = 0.0f;
        static float continuousSpawnTimer = 0.0f;
        bool triggerSpawnThisFrame = false;

        bool isCurrentlyInteracting = activeHandVisible ? lastKnownPinch : IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        if (isCurrentlyInteracting) {
            if (pinchDuration == 0.0f) triggerSpawnThisFrame = true;
            pinchDuration += dt;

            if (pinchDuration > 0.3f) {
                continuousSpawnTimer += dt;
                if (continuousSpawnTimer > 0.04f) {
                    triggerSpawnThisFrame = true;
                    continuousSpawnTimer = 0.0f;
                }
            }
        } else {
            pinchDuration = 0.0f;
            continuousSpawnTimer = 0.0f;
        }

        EngineUI::HandleSpatialInteractivity(settings, smoothedHandPos, lastKnownPinch, activeHandVisible);

        if (triggerSpawnThisFrame && smoothedHandPos.x > EngineUI::PANEL_WIDTH) {
            Ray ray = GetScreenToWorldRay(smoothedHandPos, camera);

            if (ray.direction.y < -0.001f) {
                float screenDepthPct = 1.0f - (smoothedHandPos.y / GetScreenHeight());
                float targetDepth = Lerp(15.0f, 40.0f, screenDepthPct);

                float distance = -ray.position.y / ray.direction.y;
                if (distance > targetDepth) distance = targetDepth;

                Vector3 spawnTarget = Vector3Add(ray.position, Vector3Scale(ray.direction, distance));
                if (spawnTarget.y < 1.0f) spawnTarget.y = 1.0f;

                Color colors[] = {SKYBLUE, PURPLE, GOLD};
                float lifespan = (float)GetRandomValue(30, 60) / 10.0f;

                GPUParticle p{};
                p.position = { spawnTarget.x, spawnTarget.y, spawnTarget.z, lifespan };
                p.velocity = { (float)GetRandomValue(-8, 8), (float)GetRandomValue(5, 20), (float)GetRandomValue(-8, 8), lifespan };
                Color c = colors[settings.activeObjectType];
                p.color = { c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, 1.0f };
                p.type = settings.activeObjectType;
                p.active = 1;

                g_renderer.EnqueueSpawn(p);
            }
        }

        g_renderer.FlushSpawnQueue();

        float handWorldX = 0.0f, handWorldY = 0.0f, handWorldZ = 0.0f;
        if (smoothedHandPos.x > EngineUI::PANEL_WIDTH) {
            Ray ray = GetScreenToWorldRay(smoothedHandPos, camera);
            if (ray.direction.y < -0.001f) {
                float dist = -ray.position.y / ray.direction.y;
                Vector3 hit = Vector3Add(ray.position, Vector3Scale(ray.direction, fminf(dist, 40.0f)));
                handWorldX = hit.x;
                handWorldY = hit.y;
                handWorldZ = hit.z;
            }
        }

        uint32_t activeCount = settings.activeParticleCount;

        g_renderer.DispatchCompute(activeCount, dt * settings.timeScale, settings,
                                   handWorldX, handWorldY, handWorldZ,
                                   activeHandVisible, lastKnownPinch);

        BeginDrawing();
        ClearBackground(GetColor(0x020204FF));

        BeginMode3D(camera);
        DrawGrid(60, 2.0f);

        rlDisableBackfaceCulling();
        uint32_t renderCount = g_renderer.GetAllocator().GetAliveCount();
        g_renderer.RenderParticles(renderCount);
        rlEnableBackfaceCulling();

        EndMode3D();

        EngineUI::DrawDashboard(settings, smoothedHandPos, activeHandVisible, (trackingTimeout > 0), isCurrentlyInteracting);

        DrawText(TextFormat("GPU Particles: %u | FPS: %d", g_renderer.GetAllocator().GetAliveCount(), GetFPS()), EngineUI::PANEL_WIDTH + 20, 10, 16, GREEN);

        EndDrawing();
    }

    EngineNetwork::Shutdown();
    g_renderer.Shutdown();
    CloseWindow();
    return 0;
}
