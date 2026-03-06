// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/phong.hpp>

using namespace brenta;

void PhongPointLight::apply(int light_number)
{
  // Get the current shader
  GLint prog = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

  // Set uniforms
  auto light_array = "point_lights[" + std::to_string(light_number) + "]"; 
  Shader::set_vec3(prog, light_array  + ".position",  this->position);
  Shader::set_float(prog, light_array + ".strength",  this->strength);
  Shader::set_vec3(prog, light_array  + ".ambient",   this->ambient);
  Shader::set_vec3(prog, light_array  + ".diffuse",   this->diffuse);
  Shader::set_vec3(prog, light_array  + ".specular",  this->specular);
  Shader::set_float(prog, light_array + ".constant",  this->constant);
  Shader::set_float(prog, light_array + ".linear",    this->linear);
  Shader::set_float(prog, light_array + ".quadratic", this->quadratic);
}

PhongPointLight &PhongPointLight::set_ambient(glm::vec3 ambient)
{
  this->ambient = ambient;
  return *this;
}

PhongPointLight &PhongPointLight::set_diffuse(glm::vec3 diffuse)
{
  this->diffuse = diffuse;
  return *this;
}

PhongPointLight &PhongPointLight::set_specular(glm::vec3 specular)
{
  this->specular = specular;
  return *this;
}

PhongPointLight &PhongPointLight::set_position(glm::vec3 position)
{
  this->position = position;
  return *this;
}

PhongPointLight &PhongPointLight::set_strength(float strength)
{
  this->strength = strength;
  return *this;
}

PhongPointLight &PhongPointLight::set_constant(float constant)
{
  this->constant = constant;
  return *this;
}

PhongPointLight &PhongPointLight::set_linear(float linear)
{
  this->linear = linear;
  return *this;
}

PhongPointLight &PhongPointLight::set_quadratic(float quadratic)
{
  this->quadratic = quadratic;
  return *this;
}
  
void PhongDirLight::apply()
{
  // Get the current shader
  GLint prog = 0;
  glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

  // Set uniforms
  Shader::set_vec3(prog, "dir_light.direction", this->direction);
  Shader::set_float(prog, "dir_light.strength", this->strength);
  Shader::set_vec3(prog, "dir_light.ambient",   this->ambient);
  Shader::set_vec3(prog, "dir_light.diffuse",   this->diffuse);
  Shader::set_vec3(prog, "dir_light.specular",  this->specular);

  Shader::set_bool(prog, "use_dir_light", true);
}

PhongDirLight &PhongDirLight::set_ambient(glm::vec3 ambient)
{
  this->ambient = ambient;
  return *this;
}

PhongDirLight &PhongDirLight::set_diffuse(glm::vec3 diffuse)
{
  this->diffuse = diffuse;
  return *this;
}

PhongDirLight &PhongDirLight::set_specular(glm::vec3 specular)
{
  this->specular = specular;
  return *this;
}

PhongDirLight &PhongDirLight::set_direction(glm::vec3 direction)
{
  this->direction = direction;
  return *this;
}

PhongDirLight &PhongDirLight::set_strength(float strength)
{
  this->strength = strength;
  return *this;
}
  
