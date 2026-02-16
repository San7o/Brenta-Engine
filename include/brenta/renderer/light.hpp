// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/glm.hpp>

namespace brenta
{

// Virtual light class
class PointLight
{
public:

  PointLight() = default;
  
  virtual ~PointLight() = default;
  virtual void apply(int light_number) = 0;

  glm::vec3 &get_position();
  float     &get_strength();
  float     &get_constant();
  float     &get_linear();
  float     &get_quadratic();
  
protected:

  glm::vec3 position = glm::vec3(0.0);
  float     strength = 1.0f;

  float constant  = 1.0f;
  float linear    = 0.09f;
  float quadratic = 0.00032f;
  
};

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
