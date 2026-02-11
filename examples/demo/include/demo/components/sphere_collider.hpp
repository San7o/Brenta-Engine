// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct SphereColliderComponent : component
{
  float radius;
  
  SphereColliderComponent() : radius(1.0f)
  {
  }
  SphereColliderComponent(float radius) : radius(radius)
  {
  }
};
