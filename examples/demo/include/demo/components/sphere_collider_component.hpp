// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace brenta;
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
