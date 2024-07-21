#include "systems/debug_text_system.h"

#include "engine.h"

using namespace ECS;
using namespace ECS::Types;

void InitDebugTextSystem() {

    auto debug_text = std::make_shared<System>("DebugText", []() {
        auto camera_pos = Camera::GetPosition();

        const float offset = 20.0f;

        Text::RenderText(
                "FPS: " + std::to_string(Time::GetFPS()),
                25.0f, Screen::GetHeight() - 30.0f, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));

        Text::RenderText(
                "CameraX: " + std::to_string(camera_pos.x),
                25.0f, Screen::GetHeight() - 30.0f - offset, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));

        Text::RenderText(
                "CameraY: " + std::to_string(camera_pos.y),
                25.0f, Screen::GetHeight() - 30.0f - offset*2, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));

        Text::RenderText(
                "CameraZ: " + std::to_string(camera_pos.z),
                25.0f, Screen::GetHeight() - 30.0f - offset*3, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));

        Text::RenderText(
                "Yaw: " + std::to_string(Camera::GetYaw()),
                25.0f, Screen::GetHeight() - 30.0f - offset*4, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));

        Text::RenderText(
                "Pitch: " + std::to_string(Camera::GetPitch()),
                25.0f, Screen::GetHeight() - 30.0f - offset*5, 0.35f,
                glm::vec3(1.0f, 0.9f, 0.0f));
    });

    World::AddSystem(debug_text);
    
}
