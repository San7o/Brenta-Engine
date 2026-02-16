// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/directional_light.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

struct DirectionalLightSystem : System<DirectionalLightComponent>
{
  void run(std::vector<EntityId> entities) const override
  {
    if (entities.empty())
      return;

    for (auto entity : entities)
    {
      auto light =
        World::entity_to_component<DirectionalLightComponent>(entity);

      for (auto shader_name : light->shaders)
      {
        auto shader = Shader::get_shader(shader_name);
        if (!shader)
        {
          ERROR("Light shader not found with name: {}",
                shader->get_name());
          continue;
        }

        shader->use();

        // Set the light properties
        shader->set_vec3("dirLight.direction", light->direction);
        shader->set_vec3("dirLight.ambient",   light->ambient);
        shader->set_vec3("dirLight.diffuse",   light->diffuse);
        shader->set_vec3("dirLight.specular",  light->specular);
        shader->set_float("dirLight.strength", light->strength);
        shader->set_bool("useDirLight",        true);
      }
    }
  }
};
