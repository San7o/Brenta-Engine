/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include "ecs.hpp"
#include "engine.hpp"
#include "systems/debug_text_system.hpp"

#include <vector>

using namespace Brenta::ECS;
using namespace Brenta::ECS::Types;

struct DebugTextSystem : System<None>
{
    void run(std::vector<Entity> _) const override
    {
        auto camera_pos = camera.GetPosition();

        const float offset = 20.0f;

        auto color = glm::vec3(1.0f, 0.9f, 0.0f);

        Text::RenderText("FPS: " + std::to_string(Time::GetFPS()), 25.0f,
                         Screen::GetHeight() - 30.0f, 0.35f, color);

        Text::RenderText("CameraX: " + std::to_string(camera_pos.x), 25.0f,
                         Screen::GetHeight() - 30.0f - offset, 0.35f, color);

        Text::RenderText("CameraY: " + std::to_string(camera_pos.y), 25.0f,
                         Screen::GetHeight() - 30.0f - offset * 2, 0.35f,
                         color);

        Text::RenderText("CameraZ: " + std::to_string(camera_pos.z), 25.0f,
                         Screen::GetHeight() - 30.0f - offset * 3, 0.35f,
                         color);

        Text::RenderText("CenterX: " + std::to_string(camera.center.x), 25.0f,
                         Screen::GetHeight() - 30.0f - offset * 4, 0.35f,
                         color);

        Text::RenderText("CenterY: " + std::to_string(camera.center.y), 25.0f,
                         Screen::GetHeight() - 30.0f - offset * 5, 0.35f,
                         color);

        Text::RenderText("CenterZ: " + std::to_string(camera.center.z), 25.0f,
                         Screen::GetHeight() - 30.0f - offset * 6, 0.35f,
                         color);

        Text::RenderText(
            "Theta: " + std::to_string(camera.sphericalCoordinates.theta),
            25.0f, Screen::GetHeight() - 30.0f - offset * 7, 0.35f, color);

        Text::RenderText(
            "Phi: " + std::to_string(camera.sphericalCoordinates.phi), 25.0f,
            Screen::GetHeight() - 30.0f - offset * 8, 0.35f, color);

        Text::RenderText(
            "Radius: " + std::to_string(camera.sphericalCoordinates.radius),
            25.0f, Screen::GetHeight() - 30.0f - offset * 9, 0.35f, color);
    }
};
