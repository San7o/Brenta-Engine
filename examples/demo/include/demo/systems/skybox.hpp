// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/skybox.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/ecs/ecs.hpp>

#include <demo/resources/skybox.hpp>

using namespace viotecs;

struct SkyboxSystem : System<None>
{
  void run(std::vector<EntityId> _) const override
  {
    auto* skybox_r = World::get_resource<SkyboxResource>();
    if (!skybox_r) return;

    Renderer::submit_skybox(skybox_r->skybox);
  }
};
