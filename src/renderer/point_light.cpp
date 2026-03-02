// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/point_light.hpp>

using namespace brenta;

glm::vec3 &PointLight::get_position()
{
  return this->position;
}

float &PointLight::get_strength()
{
  return this->strength;
}

float &PointLight::get_constant()
{
  return this->constant;
}

float &PointLight::get_linear()
{
  return this->linear;
}

float &PointLight::get_quadratic()
{
  return this->quadratic;
}
