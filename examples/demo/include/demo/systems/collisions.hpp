// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>
#include <brenta/ecs/components/transform_ecs_component.hpp>

#include <demo/components/physics.hpp>
#include <demo/components/sphere_collider.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace viotecs;
using namespace brenta;

glm::vec3 ResolveCollision(glm::vec3 position1,
                           glm::vec3 position2,
                           float radius1,
                           float radius2,
                           float distance);

struct CollisionsSystem : System<SphereColliderEcsComponent,
                                 PhysicsEcsComponent,
                                 TransformEcsComponent>
{
  void run(std::vector<EntityId> matches) const override
  {
    if (matches.empty())
      return;

    for (long unsigned int i = 0; i < matches.size(); i++)
    {
      for (long unsigned int j = 0; j < matches.size(); j++)
      {
        if (i == j)
          continue;

        Entity e1 = matches[i];
        Entity e2 = matches[j];
        auto sphere_component1    = e1.get_component<SphereColliderEcsComponent>();
        auto transform_component1 = e1.get_component<TransformEcsComponent>();
        auto sphere_component2    = e2.get_component<SphereColliderEcsComponent>();
        auto transform_component2 = e2.get_component<TransformEcsComponent>();

        float distance = glm::distance(transform_component1->transform.get_pos(),
                                       transform_component2->transform.get_pos());
        
        if (distance < sphere_component1->radius + sphere_component2->radius)
        {
          auto physics_component1 = e1.get_component<PhysicsEcsComponent>();
          auto physics_component2 = e2.get_component<PhysicsEcsComponent>();
          if (physics_component1 == nullptr || physics_component2 == nullptr)
            continue;
          
          if (physics_component1 != nullptr && physics_component2 == nullptr)
          {
            auto new_pos =
              ResolveCollision(transform_component1->transform.get_pos(),
                               transform_component2->transform.get_pos(),
                               sphere_component1->radius,
                               sphere_component2->radius,
                               distance);
              
            transform_component1->transform.set_pos(new_pos);
            physics_component1->velocity = glm::vec3(0.0f);
          }
          else if (physics_component1 == nullptr
                   && physics_component2 != nullptr)
          {
            auto new_pos =
              ResolveCollision(transform_component2->transform.get_pos(),
                               transform_component1->transform.get_pos(),
                               sphere_component2->radius,
                               sphere_component1->radius,
                               distance);
            
            transform_component2->transform.set_pos(new_pos);
            physics_component2->velocity = glm::vec3(0.0f);
          }
          else
          {
            auto new_pos1 =
              ResolveCollision(transform_component1->transform.get_pos(),
                               transform_component2->transform.get_pos(),
                               sphere_component1->radius,
                               sphere_component2->radius,
                               distance);
            transform_component1->transform.set_pos(new_pos1);

            auto new_pos2 =
              ResolveCollision(transform_component2->transform.get_pos(),
                               transform_component1->transform.get_pos(),
                               sphere_component2->radius,
                               sphere_component1->radius,
                               distance);
            
            transform_component2->transform.set_pos(new_pos2);
            
            physics_component1->velocity = -physics_component1->velocity;
            physics_component2->velocity = -physics_component2->velocity;
          }
        }
      }
    }
  }
};

glm::vec3 ResolveCollision(glm::vec3 position1,
                           glm::vec3 position2,
                           float radius1,
                           float radius2,
                           float distance)
{
  return position1
    + glm::normalize(position1 - position2)
    * (radius1 + radius2 - distance);
}
