// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/directional_light.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

struct DirectionalLightSystem : system<DirectionalLightComponent>
{
  void run(std::vector<entity_id> entities) const override
  {
    if (entities.empty())
      return;

    for (auto entity : entities)
    {
      auto light =
        world::entity_to_component<DirectionalLightComponent>(entity);

      for (auto shader : light->shaders)
      {
        if (Shader::get_id(shader) == (unsigned int) 0)
        {
          ERROR("Light shader not found with name: {}", shader);
          continue;
        }
        Shader::use(shader);

        /* Set the light properties */
        Shader::set_vec3(shader, "dirLight.direction", light->direction);
        Shader::set_vec3(shader, "dirLight.ambient", light->ambient);
        Shader::set_vec3(shader, "dirLight.diffuse", light->diffuse);
        Shader::set_vec3(shader, "dirLight.specular", light->specular);
        Shader::set_float(shader, "dirLight.strength", light->strength);
        Shader::set_bool(shader, "useDirLight", true);
      }
    }
  }
};
