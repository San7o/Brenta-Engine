// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/glm.hpp>

namespace brenta
{

class DirLight
{
public:

  DirLight() = default;
  
  virtual ~DirLight() = default;
  virtual void apply() = 0;

  glm::vec3 &get_direction();
  float     &get_strength();
  
protected:

  glm::vec3 direction = glm::vec3(0.2f, -1.0f, -0.3f);
  float     strength  = 1.0f;
  
};

} // namespace brenta
