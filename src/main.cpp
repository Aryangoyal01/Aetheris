#include "raylib.h"
#include "raymath.h"
#include "settings.h"
#include "ui.h"
#include "network.h"
#include <vector>

constexpr int MAX_PARTICLES = 1000;
constexpr float TIMEOUT_MAX = 0.20f;

int main()
{
    InitWindow(800, 600, "Aetheris Engine - Production Matrix");
    int monitor = GetCurrentMonitor();
    SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
    ToggleFullscreen();
    SetTargetFPS(60);

    EngineNetwork::Initialize(8080);
    EngineSettings settings;

    float trackingSensitivityX = 1.35f, trackingSensitivityY = 1.20f;
    float horizontalOffset = 0.0f, verticalOffset = 0.0f;
    bool invertHorizontal = false;

    Vector2 smoothedHandPos = { 400.0f, 300.0f };
    bool lastKnownPinch = false, activeHandVisible = false;
    float trackingTimeout = 0.0f;

    Camera3D camera = { { 0.0f, 25.0f, 35.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, CAMERA_PERSPECTIVE };

    // Added life and maxLife to initialization
    std::vector<ParticleInstance> particlePool(MAX_PARTICLES, {PARTICLE_TYPE_A, {0, 0, 0}, {0, 0, 0}, WHITE, false, 0.0f, 0.0f});
    int nextParticleIndex = 0;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // [PHASE 1] INPUT CALIBRATION
        if (IsKeyPressed(KEY_I)) invertHorizontal = !invertHorizontal;
        if (IsKeyDown(KEY_UP))    trackingSensitivityY += 0.5f * dt;
        if (IsKeyDown(KEY_DOWN))  trackingSensitivityY -= 0.5f * dt;
        if (IsKeyDown(KEY_RIGHT)) trackingSensitivityX += 0.5f * dt;
        if (IsKeyDown(KEY_LEFT))  trackingSensitivityX -= 0.5f * dt;
        
        trackingSensitivityX = Clamp(trackingSensitivityX, 0.1f, 5.0f);
        trackingSensitivityY = Clamp(trackingSensitivityY, 0.1f, 5.0f);

        // [PHASE 2] UDP NETWORK POLLING
        EngineNetwork::TrackingData tracking = EngineNetwork::PollTrackingData();
        
        if (tracking.hasNewPacket) {
            trackingTimeout = TIMEOUT_MAX;
            activeHandVisible = tracking.handDetected;
            lastKnownPinch = tracking.isPinching;

            // [PHASE 3] SILKY SMOOTH RESOLUTION (Frame-Independent)
            float rawNormX = (invertHorizontal) ? (1.0f - tracking.normX) : tracking.normX;
            float rawNormY = tracking.normY;
            
            static Vector2 smoothedNorm = { 0.5f, 0.5f };
            // Exponential Decay Smoothing: Fluid regardless of FPS
            float smoothStep = 1.0f - exp(-settings.smoothingFactor * dt); 
            smoothedNorm.x = Lerp(smoothedNorm.x, rawNormX, smoothStep);
            smoothedNorm.y = Lerp(smoothedNorm.y, rawNormY, smoothStep);

            float normX = (smoothedNorm.x - 0.5f) * trackingSensitivityX + 0.5f + horizontalOffset;
            float normY = (smoothedNorm.y - 0.5f) * trackingSensitivityY + 0.5f + verticalOffset;
            
            smoothedHandPos = { Clamp(normX, 0.0f, 1.0f) * GetScreenWidth(), 
                                Clamp(normY, 0.0f, 1.0f) * GetScreenHeight() };
        } else {
            trackingTimeout -= dt;
            if (trackingTimeout <= 0.0f) activeHandVisible = false;
        }

        // [PHASE 4] GESTURE ARBITRATION (The "Hold to Summon" Machine)
        static float pinchDuration = 0.0f;
        static float continuousSpawnTimer = 0.0f;
        bool triggerSpawnThisFrame = false;

        bool isCurrentlyInteracting = activeHandVisible ? lastKnownPinch : IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        if (isCurrentlyInteracting) {
            if (pinchDuration == 0.0f) triggerSpawnThisFrame = true; // Initial click fires immediately
            pinchDuration += dt;
            
            if (pinchDuration > 0.3f) { // If held for 0.3s, start the particle hose
                continuousSpawnTimer += dt;
                if (continuousSpawnTimer > 0.04f) { // Spawn every 0.04 seconds
                    triggerSpawnThisFrame = true;
                    continuousSpawnTimer = 0.0f;
                }
            }
        } else {
            pinchDuration = 0.0f;
            continuousSpawnTimer = 0.0f;
        }

        EngineUI::HandleSpatialInteractivity(settings, smoothedHandPos, lastKnownPinch, activeHandVisible);

        // [PHASE 5] SPATIOTEMPORAL SIMULATION
        if (triggerSpawnThisFrame && smoothedHandPos.x > EngineUI::PANEL_WIDTH)
        {
            Ray ray = GetScreenToWorldRay(smoothedHandPos, camera);
            
            if (ray.direction.y < -0.001f) 
            {
                // Dynamic depth based on screen Y (higher on screen = deeper into the 3D scene)
                float screenDepthPct = 1.0f - (smoothedHandPos.y / GetScreenHeight());
                float targetDepth = Lerp(15.0f, 40.0f, screenDepthPct);
                
                float distance = -ray.position.y / ray.direction.y;
                if (distance > targetDepth) distance = targetDepth; // Cap depth

                Vector3 spawnTarget = Vector3Add(ray.position, Vector3Scale(ray.direction, distance));
                if (spawnTarget.y < 1.0f) spawnTarget.y = 1.0f;

                Color colors[] = {SKYBLUE, PURPLE, GOLD};
                float lifespan = (float)GetRandomValue(30, 60) / 10.0f; // Live for 3 to 6 seconds

                particlePool[nextParticleIndex] = {
                    (ParticleType)settings.activeObjectType,
                    spawnTarget,
                    {(float)GetRandomValue(-8, 8), (float)GetRandomValue(5, 20), (float)GetRandomValue(-8, 8)},
                    colors[settings.activeObjectType],
                    true,
                    lifespan, lifespan // Set initial life
                };
                nextParticleIndex = (nextParticleIndex + 1) % MAX_PARTICLES;
            }
        }

        // [PHASE 6] PHYSICS & LIFECYCLE UPDATES
        for (auto& p : particlePool)
        {
            if (!p.active) continue;

            // Update Lifecycle
            p.life -= dt;
            if (p.life <= 0.0f) {
                p.active = false;
                continue;
            }

            // Update Physics
            p.velocity.y -= settings.gravityStrength * dt;
            p.position = Vector3Add(p.position, Vector3Scale(p.velocity, dt));

            if (p.position.y <= 0.0f) {
                p.position.y = 0.0f;
                p.velocity.y = -p.velocity.y * settings.elasticityValue;
                p.velocity.x *= (1.0f - settings.frictionCoefficient);
                p.velocity.z *= (1.0f - settings.frictionCoefficient);
            }
        }

        // [PHASE 7] RENDER
        BeginDrawing();
            ClearBackground(GetColor(0x020204FF)); // Slightly tinted deep void
            
            BeginMode3D(camera);
                DrawGrid(60, 2.0f); // Larger grid
                
                // Draw particles with Additive Blending for a glowing effect
                BeginBlendMode(BLEND_ADDITIVE);
                for (const auto& p : particlePool) {
                    if (!p.active) continue;
                    
                    // Shrink as they die
                    float scaleMultiplier = p.life / p.maxLife; 
                    Color renderColor = Fade(p.color, scaleMultiplier); // Fade out

                    if (p.type == PARTICLE_TYPE_A) DrawSphere(p.position, 0.5f * scaleMultiplier, renderColor);
                    else if (p.type == PARTICLE_TYPE_B) DrawCube(p.position, 0.8f * scaleMultiplier, 0.8f * scaleMultiplier, 0.8f * scaleMultiplier, renderColor);
                    else DrawCylinder(p.position, 0.4f * scaleMultiplier, 0.4f * scaleMultiplier, 1.0f * scaleMultiplier, 8, renderColor);
                }
                EndBlendMode();

            EndMode3D();

            // Pass the pinch state to the UI for reactive rendering
            EngineUI::DrawDashboard(settings, smoothedHandPos, activeHandVisible, (trackingTimeout > 0), isCurrentlyInteracting);
        EndDrawing();
    }

    EngineNetwork::Shutdown();
    CloseWindow();
    return 0;
}