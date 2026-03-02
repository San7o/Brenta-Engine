// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PhysicsEcsComponent : public viotecs::Component
{
public:
  
  float     mass;
  float     density;
  glm::vec3 velocity;
  glm::vec3 acceleration;
  bool      isElastic;

  PhysicsEcsComponent()
      : mass(0.0f), density(0.0f), velocity(glm::vec3(0.0f)),
        acceleration(glm::vec3(0.0f)), isElastic(false) {}
  PhysicsEcsComponent(float mass,
                      float density,
                      glm::vec3 velocity,
                      glm::vec3 acceleration,
                      bool isElastic)
      : mass(mass), density(density), velocity(velocity),
        acceleration(acceleration), isElastic(isElastic) {}
  
};
