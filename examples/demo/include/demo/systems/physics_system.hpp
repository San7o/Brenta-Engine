// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/engine.hpp>
#include <demo/components/physics_component.hpp>
#include <demo/components/transform_component.hpp>
#include <demo/systems/physics_system.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <viotecs/viotecs.hpp>

using namespace viotecs;
using namespace viotecs::types;

struct PhysicsSystem : system<PhysicsComponent, TransformComponent>
{
  void run(std::vector<entity_id> matches) const override
  {
    if (matches.empty())
      return;

    for (auto match : matches)
    {
      auto physics_component =
        world::entity_to_component<PhysicsComponent>(match);

      auto transform_component =
        world::entity_to_component<TransformComponent>(match);

      if (physics_component->acceleration != glm::vec3(0.0f))
      {
        physics_component->velocity +=
          physics_component->acceleration * time::get_delta_time();
      }
      if (physics_component->velocity != glm::vec3(0.0f))
      {
        transform_component->position +=
          physics_component->velocity * time::get_delta_time();
      }
    }
  }
};
