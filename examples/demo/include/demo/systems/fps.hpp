// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/asset.hpp>
#include <brenta/text.hpp>
#include <brenta/window.hpp>
#include <brenta/ecs/ecs.hpp>

using namespace viotecs;

struct FPSSystem : System<None>
{
  void run(std::vector<EntityId> _) const override
  {
    auto font = AssetManager::get<Font>("TextFont");
    auto fps = std::to_string(Window::get_time().get_fps());
    
    Renderer::submit_text({
        "FPS: " + fps,
        25.0f,
        25.0f,
        0.35f,
        Color::yellow(),
        font
      });
  }
};
