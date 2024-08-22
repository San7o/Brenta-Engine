#pragma once

#include "systems/debug_text_system.hpp"
#include "engine.hpp"
#include "ecs.hpp"

#include <vector>

using namespace ECS;
using namespace ECS::Types;

struct DebugTextSystem : System<None> {

    void run(std::vector<Entity> _) const override {
        auto camera_pos = camera.GetPosition();

        const float offset = 20.0f;

        auto color = glm::vec3(1.0f, 0.9f, 0.0f);

        Text::RenderText(
                "FPS: " + std::to_string(Time::GetFPS()),
                25.0f, Screen::GetHeight() - 30.0f, 0.35f,
                color);

        Text::RenderText(
                "CameraX: " + std::to_string(camera_pos.x),
                25.0f, Screen::GetHeight() - 30.0f - offset, 0.35f,
                color);

        Text::RenderText(
                "CameraY: " + std::to_string(camera_pos.y),
                25.0f, Screen::GetHeight() - 30.0f - offset*2, 0.35f,
                color);

        Text::RenderText(
                "CameraZ: " + std::to_string(camera_pos.z),
                25.0f, Screen::GetHeight() - 30.0f - offset*3, 0.35f,
                color);

        Text::RenderText(
                "CenterX: " + std::to_string(camera.center.x),
                25.0f, Screen::GetHeight() - 30.0f - offset*4, 0.35f,
                color);

        Text::RenderText(
                "CenterY: " + std::to_string(camera.center.y),
                25.0f, Screen::GetHeight() - 30.0f - offset*5, 0.35f,
                color);

        Text::RenderText(
                "CenterZ: " + std::to_string(camera.center.z),
                25.0f, Screen::GetHeight() - 30.0f - offset*6, 0.35f,
                color);

        Text::RenderText(
                "Theta: " + std::to_string(camera.sphericalCoordinates.theta),
                25.0f, Screen::GetHeight() - 30.0f - offset*7, 0.35f,
                color);

        Text::RenderText(
                "Phi: " + std::to_string(camera.sphericalCoordinates.phi),
                25.0f, Screen::GetHeight() - 30.0f - offset*8, 0.35f,
                color);

        Text::RenderText(
                "Radius: " + std::to_string(camera.sphericalCoordinates.radius),
                25.0f, Screen::GetHeight() - 30.0f - offset*9, 0.35f,
                color);
    }
};
