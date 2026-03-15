// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/vec3.hpp>

namespace brenta
{

// Virtual point light class
class PointLight
{
public:

  glm::vec3 position = glm::vec3(0.0);
  float     strength = 1.0f;

  float constant  = 1.0f;
  float linear    = 0.09f;
  float quadratic = 0.00032f;
  
  PointLight()          = default;
  virtual ~PointLight() = default;
  
  virtual void apply(int light_number) = 0;

};

} // namespace brenta
