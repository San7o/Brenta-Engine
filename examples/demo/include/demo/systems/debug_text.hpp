// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/text.hpp>

#include <demo/resources/camera.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

struct DebugTextSystem : System<None>
{
  void run(std::vector<EntityId> _) const override
  {
    auto            color            = Color::yellow();
    const float     offset           = 20.0f;
    brenta::Camera *cam              = World::get_resource<CameraResource>()->cam;
    auto            camera_world_pos = cam->get_transform().get_pos();
    auto            font             = AssetManager::get<Font>("TextFont");

    Text::render("FPS: " + std::to_string(Window::get_time().get_fps()), 25.0f,
                 Window::get_height() - 30.0f, 0.35f, color, font);

    Text::render("CameraX: " + std::to_string(camera_world_pos.x), 25.0f,
                 Window::get_height() - 30.0f - offset, 0.35f, color, font);

    Text::render("CameraY: " + std::to_string(camera_world_pos.y), 25.0f,
                 Window::get_height() - 30.0f - offset * 2, 0.35f, color, font);

    Text::render("CameraZ: " + std::to_string(camera_world_pos.z), 25.0f,
                 Window::get_height() - 30.0f - offset * 3, 0.35f, color, font);

    auto camera_pos = cam->get_pos();
    try
    {
      Camera::Spherical scam = std::get<Camera::Spherical>(camera_pos);
        
      Text::render("CenterX: " + std::to_string(scam.center.x),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 4, 0.35f,
                   color, font);

      Text::render("CenterY: " + std::to_string(scam.center.y),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 5, 0.35f,
                   color, font);

      Text::render("CenterZ: " + std::to_string(scam.center.z),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 6, 0.35f,
                   color, font);
      
      Text::render("Theta: " + std::to_string(scam.theta),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 7, 0.35f, color, font);
      
      Text::render("Phi: " + std::to_string(scam.phi), 25.0f,
                   Window::get_height() - 30.0f
                   - offset * 8, 0.35f, color, font);

      Text::render("Radius: " + std::to_string(scam.radius),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 9, 0.35f, color, font);
    }
    catch ([[maybe_unused]] const std::bad_variant_access& ex)
    {
      Camera::Aircraft acam = std::get<Camera::Aircraft>(camera_pos);
    
      Text::render("Yaw: " + std::to_string(acam.yaw),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 4, 0.35f, color, font);

      Text::render("Pitch: " + std::to_string(acam.pitch), 25.0f,
                   Window::get_height() - 30.0f
                   - offset * 6, 0.35f, color, font);

      Text::render("Roll: " + std::to_string(acam.roll),
                   25.0f, Window::get_height() - 30.0f
                   - offset * 6, 0.35f, color, font);

    }

  }
};
