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
  Shader shader = Shader(prog, "current_light_material_shader");

  // Set uniforms
  auto light_array = "point_lights[" + std::to_string(light_number) + "]"; 
  shader.set_vec3(light_array  + ".position",  this->position);
  shader.set_float(light_array + ".strength",  this->strength);
  shader.set_vec3(light_array  + ".ambient",   this->ambient);
  shader.set_vec3(light_array  + ".diffuse",   this->diffuse);
  shader.set_vec3(light_array  + ".specular",  this->specular);
  shader.set_float(light_array + ".constant",  this->constant);
  shader.set_float(light_array + ".linear",    this->linear);
  shader.set_float(light_array + ".quadratic", this->quadratic);
}

glm::vec3 &PhongPointLight::get_ambient()
{
  return this->ambient;
}

glm::vec3 &PhongPointLight::get_diffuse()
{
  return this->diffuse;
}

glm::vec3 &PhongPointLight::get_specular()
{
  return this->specular;
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
  Shader shader = Shader(prog, "current_dirlight_material_shader");

  // Set uniforms
  shader.set_vec3("dir_light.direction", this->direction);
  shader.set_float("dir_light.strength", this->strength);
  shader.set_vec3("dir_light.ambient",   this->ambient);
  shader.set_vec3("dir_light.diffuse",   this->diffuse);
  shader.set_vec3("dir_light.specular",  this->specular);

  shader.set_bool("use_dir_light", true);
}

glm::vec3 &PhongDirLight::get_ambient()
{
  return this->ambient;
}

glm::vec3 &PhongDirLight::get_diffuse()
{
  return this->diffuse;
}

glm::vec3 &PhongDirLight::get_specular()
{
  return this->specular;
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
  
