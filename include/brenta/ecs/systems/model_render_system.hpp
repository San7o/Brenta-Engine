// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/logger.hpp>

#include <vector>

namespace brenta
{

class ModelRenderSystem : public viotecs::System<ModelEcsComponent,
                                                 TransformEcsComponent>
{
public:
  
  void run(std::vector<viotecs::EntityId> entities) const override
  {
    for (auto& e : entities)
    {
      auto model =
        viotecs::World::entity_to_component<ModelEcsComponent>(e);
      auto transform =
        viotecs::World::entity_to_component<TransformEcsComponent>(e);
      
      model->draw(transform->transform.get_model_matrix());
    }
  }
};

} // namespace brenta
