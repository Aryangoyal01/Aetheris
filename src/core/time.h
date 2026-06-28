#pragma once

#include <cstdint>

class Time
{
public:
    static float DeltaTime();
    static float TotalTime();
    static uint64_t FrameCount();

    static void Update(float deltaTime);

private:
    static float s_deltaTime;
    static float s_totalTime;
    static uint64_t s_frameCount;
};
