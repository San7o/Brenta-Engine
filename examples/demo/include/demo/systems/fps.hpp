// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/gui/text.hpp>
#include <brenta/window.hpp>
#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;
using namespace viotecs::types;

struct FPSSystem : system<none>
{
  void run(std::vector<entity_id> _) const override
  {
    brenta::text::render_text("FPS: " + std::to_string(window::get_time().get_fps()),
                              25.0f, 25.0f, 0.35f, glm::vec3(1.0f, 0.9f, 0.0f));
  }
};
