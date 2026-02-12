// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/physics.hpp>
#include <demo/components/sphere_collider.hpp>
#include <demo/components/transform.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace viotecs;
using namespace viotecs::types;

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance);

struct CollisionsSystem : system<SphereColliderComponent, TransformComponent>
{
  void run(std::vector<entity_id> matches) const override
  {
    if (matches.empty())
      return;

    for (long unsigned int i = 0; i < matches.size(); i++)
    {
      for (long unsigned int j = 0; j < matches.size(); j++)
      {
        if (i == j)
          continue;

        entity e1 = matches[i];
        entity e2 = matches[j];
        auto sphere_component1 = e1.get_component<SphereColliderComponent>();
        auto transform_component1 = e1.get_component<TransformComponent>();
        auto sphere_component2 = e2.get_component<SphereColliderComponent>();
        auto transform_component2 = e2.get_component<TransformComponent>();

        float distance = glm::distance(transform_component1->transform.get_pos(),
                                       transform_component2->transform.get_pos());
        if (distance < sphere_component1->radius + sphere_component2->radius)
        {
          auto physics_component1 = e1.get_component<PhysicsComponent>();
          auto physics_component2 = e2.get_component<PhysicsComponent>();
          if (physics_component1 == nullptr || physics_component2 == nullptr)
          {
            continue;
          }
          if (physics_component1 != nullptr && physics_component2 == nullptr)
          {
            transform_component1->transform
              .set_pos(ResolveCollision(transform_component1->transform.get_pos(),
                                        transform_component2->transform.get_pos(),
              sphere_component1->radius, sphere_component2->radius, distance));
            physics_component1->velocity = glm::vec3(0.0f);
          }
          else if (physics_component1 == nullptr
                   && physics_component2 != nullptr)
          {
            transform_component2->transform
              .set_pos(ResolveCollision(transform_component2->transform.get_pos(),
                                        transform_component1->transform.get_pos(),
              sphere_component2->radius, sphere_component1->radius, distance));
            physics_component2->velocity = glm::vec3(0.0f);
          }
          else
          {
            transform_component1->transform
              .set_pos(ResolveCollision(transform_component1->transform.get_pos(),
                                        transform_component2->transform.get_pos(),
              sphere_component1->radius, sphere_component2->radius, distance));
            transform_component2->transform
              .set_pos(ResolveCollision(transform_component2->transform.get_pos(),
                                        transform_component1->transform.get_pos(),
              sphere_component2->radius, sphere_component1->radius, distance));
            physics_component1->velocity = -physics_component1->velocity;
            physics_component2->velocity = -physics_component2->velocity;
          }
        }
      }
    }
  }
};

glm::vec3 ResolveCollision(glm::vec3 position1, glm::vec3 position2,
                           float radius1, float radius2, float distance)
{
  return position1
         + glm::normalize(position1 - position2)
             * (radius1 + radius2 - distance);
}
