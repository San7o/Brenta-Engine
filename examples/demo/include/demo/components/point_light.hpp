// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;

struct PointLightComponent : component
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  // Attenuation
  float constant; // at least 1
  float linear;
  float quadratic;
  float strength;
  std::vector<brenta::shader::name_t> shaders;

  PointLightComponent()
      : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)),
        specular(glm::vec3(0.0f)), constant(1.0f), linear(0.0f),
        quadratic(0.0f), strength(0.0f)
  {
  }
  PointLightComponent(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                      float constant, float linear, float quadratic,
                      float strength,
                      std::vector<brenta::shader::name_t> shaders)
      : ambient(ambient), diffuse(diffuse), specular(specular),
        constant(constant), linear(linear), quadratic(quadratic),
        strength(strength), shaders(shaders)
  {
  }
};
