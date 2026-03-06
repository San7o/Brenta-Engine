// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/dir_light.hpp>
#include <brenta/renderer/point_light.hpp>
#include <brenta/renderer/opengl/shader.hpp>

#include <glad/glad.h>

namespace brenta
{

class PhongPointLight : public PointLight
{
public:

  glm::vec3 ambient  = glm::vec3(0.1f, 0.1f, 0.1f);
  glm::vec3 diffuse  = glm::vec3(0.5f, 0.5f, 0.5f);
  glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
  
  PhongPointLight() = default;

  void apply(int light_number) override;
  
  PhongPointLight &set_ambient(glm::vec3 ambient);
  PhongPointLight &set_diffuse(glm::vec3 diffuse);
  PhongPointLight &set_specular(glm::vec3 specular);
  PhongPointLight &set_position(glm::vec3 position);  
  PhongPointLight &set_strength(float strength);
  PhongPointLight &set_constant(float constant);
  PhongPointLight &set_linear(float linear);
  PhongPointLight &set_quadratic(float quadratic);
  
private:

};

class PhongDirLight : public DirLight
{
public:

  glm::vec3 ambient  = glm::vec3(0.7f, 0.7f, 0.7f);
  glm::vec3 diffuse  = glm::vec3(0.7f, 0.7f, 0.7f);
  glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
  
  PhongDirLight() = default;
  
  void apply() override;

  PhongDirLight &set_ambient(glm::vec3 ambient);
  PhongDirLight &set_diffuse(glm::vec3 diffuse);
  PhongDirLight &set_specular(glm::vec3 specular);
  PhongDirLight &set_direction(glm::vec3 direction);  
  PhongDirLight &set_strength(float strength);
  
private:

};  
  
} // namespace brenta
