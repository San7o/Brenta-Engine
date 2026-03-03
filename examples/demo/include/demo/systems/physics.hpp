// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>

#include <demo/components/physics.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace viotecs;
using namespace brenta;

struct PhysicsSystem : System<PhysicsEcsComponent,
                              TransformEcsComponent>
{
  void run(std::vector<EntityId> matches) const override
  {
    if (matches.empty())
      return;

    for (auto match : matches)
    {
      auto physics_component =
        World::entity_to_component<PhysicsEcsComponent>(match);

      auto transform_component =
        World::entity_to_component<TransformEcsComponent>(match);

      auto dt = Window::get_time().get_delta();
      if (physics_component->acceleration != glm::vec3(0.0f))
      {
        physics_component->velocity += physics_component->acceleration * dt;
      }
      if (physics_component->velocity != glm::vec3(0.0f))
      {
        transform_component->transform
          .set_pos(transform_component->transform.get_pos() +
                   physics_component->velocity * dt);
      }
    }
  }
};
