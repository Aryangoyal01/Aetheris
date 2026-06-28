#include "time.h"

float Time::s_deltaTime = 0.0f;
float Time::s_totalTime = 0.0f;
uint64_t Time::s_frameCount = 0;

float Time::DeltaTime()
{
    return s_deltaTime;
}

float Time::TotalTime()
{
    return s_totalTime;
}

uint64_t Time::FrameCount()
{
    return s_frameCount;
}

void Time::Update(float deltaTime)
{
    s_deltaTime = deltaTime;
    s_totalTime += deltaTime;
    s_frameCount++;
}
