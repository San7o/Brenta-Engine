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

struct DirectionalLightComponent : Component
{
  glm::vec3 direction;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float     strength;
  std::vector<Shader::Name> shaders;
  
  DirectionalLightComponent()
      : direction(glm::vec3(0.0f)), ambient(glm::vec3(0.0f)),
        diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), strength(0.0f)
  {
  }
  
  DirectionalLightComponent(glm::vec3 direction, glm::vec3 ambient,
                            glm::vec3 diffuse, glm::vec3 specular,
                            float strength,
                            std::vector<Shader::Name> shaders)
      : direction(direction), ambient(ambient), diffuse(diffuse),
        specular(specular), strength(strength), shaders(shaders)
  {
  }
};
