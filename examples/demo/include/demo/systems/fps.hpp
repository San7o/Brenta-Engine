// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/text.hpp>
#include <brenta/window.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

struct FPSSystem : System<None>
{
  void run(std::vector<EntityId> _) const override
  {
    auto font = AssetManager::get<Font>("TextFont");
    auto fps = std::to_string(Window::get_time().get_fps());
    brenta::Text::render("FPS: " + fps,
                         25.0f, 25.0f, 0.35f,
                         Color::yellow(), font);
  }
};
