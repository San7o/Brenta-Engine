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

using namespace brenta::ecs;
using namespace brenta::ecs::types;

struct DebugTextSystem : system<none>
{
    void run(std::vector<entity_t> _) const override
    {
        auto camera_pos = default_camera.get_position();

        const float offset = 20.0f;

        auto color = glm::vec3(1.0f, 0.9f, 0.0f);

        text::render_text("FPS: " + std::to_string(time::get_fps()), 25.0f,
                          screen::get_height() - 30.0f, 0.35f, color);

        text::render_text("CameraX: " + std::to_string(camera_pos.x), 25.0f,
                          screen::get_height() - 30.0f - offset, 0.35f, color);

        text::render_text("CameraY: " + std::to_string(camera_pos.y), 25.0f,
                          screen::get_height() - 30.0f - offset * 2, 0.35f,
                          color);

        text::render_text("CameraZ: " + std::to_string(camera_pos.z), 25.0f,
                          screen::get_height() - 30.0f - offset * 3, 0.35f,
                          color);

        text::render_text("CenterX: " + std::to_string(default_camera.center.x),
                          25.0f, screen::get_height() - 30.0f - offset * 4,
                          0.35f, color);

        text::render_text("CenterY: " + std::to_string(default_camera.center.y),
                          25.0f, screen::get_height() - 30.0f - offset * 5,
                          0.35f, color);

        text::render_text("CenterZ: " + std::to_string(default_camera.center.z),
                          25.0f, screen::get_height() - 30.0f - offset * 6,
                          0.35f, color);

        text::render_text(
            "Theta: "
                + std::to_string(default_camera.spherical_coordinates.theta),
            25.0f, screen::get_height() - 30.0f - offset * 7, 0.35f, color);

        text::render_text(
            "Phi: " + std::to_string(default_camera.spherical_coordinates.phi),
            25.0f, screen::get_height() - 30.0f - offset * 8, 0.35f, color);

        text::render_text(
            "Radius: "
                + std::to_string(default_camera.spherical_coordinates.radius),
            25.0f, screen::get_height() - 30.0f - offset * 9, 0.35f, color);
    }
};
