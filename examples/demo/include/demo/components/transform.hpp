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
  glm::vec3 position;
  glm::vec3 rotation;
  float scale;

  TransformComponent()
      : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
  {
  }
  TransformComponent(glm::vec3 position, glm::vec3 rotation, float scale)
      : position(position), rotation(rotation), scale(scale)
  {
  }
};
