// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/physics.hpp>
#include <demo/components/transform.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

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
          physics_component->acceleration * Window::get_time().get_delta();
      }
      if (physics_component->velocity != glm::vec3(0.0f))
      {
        transform_component->transform
          .set_pos(transform_component->transform.get_pos() +
                   physics_component->velocity * Window::get_time().get_delta());
      }
    }
  }
};
