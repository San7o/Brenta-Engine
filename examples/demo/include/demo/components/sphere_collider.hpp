// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/ecs/ecs.hpp>

class SphereColliderEcsComponent : public viotecs::Component
{
public:
  
  float radius;
  
  SphereColliderEcsComponent() : radius(1.0f) {}
  SphereColliderEcsComponent(float radius) : radius(radius) {}
  
};
