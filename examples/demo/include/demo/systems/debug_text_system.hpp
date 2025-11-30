// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/systems/debug_text_system.hpp>

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;
using namespace viotecs::types;

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
                      screen::get_height() - 30.0f - offset * 2, 0.35f, color);

    text::render_text("CameraZ: " + std::to_string(camera_pos.z), 25.0f,
                      screen::get_height() - 30.0f - offset * 3, 0.35f, color);

    text::render_text("CenterX: " + std::to_string(default_camera.center.x),
                      25.0f, screen::get_height() - 30.0f - offset * 4, 0.35f,
                      color);

    text::render_text("CenterY: " + std::to_string(default_camera.center.y),
                      25.0f, screen::get_height() - 30.0f - offset * 5, 0.35f,
                      color);

    text::render_text("CenterZ: " + std::to_string(default_camera.center.z),
                      25.0f, screen::get_height() - 30.0f - offset * 6, 0.35f,
                      color);

    text::render_text(
      "Theta: " + std::to_string(default_camera.spherical_coordinates.theta),
      25.0f, screen::get_height() - 30.0f - offset * 7, 0.35f, color);

    text::render_text(
      "Phi: " + std::to_string(default_camera.spherical_coordinates.phi), 25.0f,
      screen::get_height() - 30.0f - offset * 8, 0.35f, color);

    text::render_text(
      "Radius: " + std::to_string(default_camera.spherical_coordinates.radius),
      25.0f, screen::get_height() - 30.0f - offset * 9, 0.35f, color);
  }
};
