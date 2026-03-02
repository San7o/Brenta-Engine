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

class PointLightRenderSystem : public viotecs::System<PointLightEcsComponent,
                                                      TransformEcsComponent>
{
public:
  
  void run(std::vector<viotecs::EntityId> entities) const override
  {
    for (auto& e : entities)
    {
      auto transform =
        viotecs::World::entity_to_component<TransformEcsComponent>(e);
      auto point_light =
        viotecs::World::entity_to_component<PointLightEcsComponent>(e);
      point_light->draw(transform->transform);
    }
  }
};

} // namespace brenta
