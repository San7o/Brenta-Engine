// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/dir_light.hpp>

using namespace brenta;

glm::vec3 &DirLight::get_direction()
{
  return this->direction;
}

float &DirLight::get_strength()
{
  return this->strength;
}
  
