// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/window.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/model_ecs_component.hpp>

#include <demo/components/sprite_animation.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace viotecs;
using namespace brenta;

struct SpriteAnimationSystem : System<SpriteAnimationEcsComponent,
                                      ModelEcsComponent>
{
  void run(std::vector<EntityId> matches) const override
  {
    for (auto& entity : matches)
    {
      auto dt = Window::get_time().delta;

      auto e = Entity(entity);
      auto sprite_animation =
        e.get_component<SpriteAnimationEcsComponent>();
      
      if (sprite_animation->time_passed + 1 == sprite_animation->duration)
      {
        sprite_animation->time_passed = 0;
        sprite_animation->atlas_index =
          (sprite_animation->atlas_index + 1) % sprite_animation->atlas_size;
        
        auto model =
          e.get_component<ModelEcsComponent>();

        model->model->get_material()->set_int("atlasIndex",
                                              sprite_animation->atlas_index);
        return;
      }
      else
      {
        sprite_animation->time_passed++;
      }
    }
  }
};
