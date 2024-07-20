#include "engine_time.h"
#include "screen.h"

using namespace ECS;

float Time::currentTime = 0.0f;
float Time::deltaTime = 0.0f;
float Time::lastFrame = 0.0f;

float Time::GetCurrentTime() {
    return Screen::GetTime();
}

float Time::GetDeltaTime() {
    return deltaTime;
}

float Time::GetFPS() {
    return 1.0f / deltaTime;
}

void Time::Update(float newTime) {
    currentTime = newTime;
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
}
