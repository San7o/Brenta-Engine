// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <viotecs/viotecs.hpp>

using namespace viotecs;

struct TransformComponent : component
{
  Transform transform;
  
  TransformComponent() = default;
  TransformComponent(const Transform& transform)
    : transform(transform) {}
};
