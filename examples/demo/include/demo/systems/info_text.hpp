// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/asset.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/ecs/ecs.hpp>

using namespace viotecs;

struct InfoTextSystem : System<None>
{
  void run(std::vector<EntityId> _) const override
  {
    auto font = AssetManager::get<Font>("TextFont");
    Renderer::submit_text({
        "Move with Shift / Ctrl / Alt + Mouse" ,
        10.0f,
        10.0f,
        0.7f,
        Color::yellow(),
        font
      });
  }
};
