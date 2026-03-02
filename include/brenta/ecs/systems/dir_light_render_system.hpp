// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>

#include <vector>

namespace brenta
{

class DirLightRenderSystem : public viotecs::System<DirLightEcsComponent>
{
public:
  
  void run(std::vector<viotecs::EntityId> entities) const override
  {
    for (auto& e : entities)
    {
      auto dir_light =
        viotecs::World::entity_to_component<DirLightEcsComponent>(e);
      dir_light->draw();
    }
  }
};

} // namespace brenta
