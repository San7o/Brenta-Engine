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
        if (shader::get_id(shader) == (unsigned int) 0)
        {
          ERROR("Light shader not found with name: {}", shader);
          continue;
        }
        shader::use(shader);

        /* Set the light properties */
        shader::set_vec3(shader, "dirLight.direction", light->direction);
        shader::set_vec3(shader, "dirLight.ambient", light->ambient);
        shader::set_vec3(shader, "dirLight.diffuse", light->diffuse);
        shader::set_vec3(shader, "dirLight.specular", light->specular);
        shader::set_float(shader, "dirLight.strength", light->strength);
        shader::set_bool(shader, "useDirLight", true);
      }
    }
  }
};
