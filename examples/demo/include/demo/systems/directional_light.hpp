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

      if (!light->shader)
      {
        ERROR("Light shader not found");
        continue;
      }

      light->shader->use();

      // Set the light properties
      light->shader->set_vec3("dir_light.direction", light->direction);
      light->shader->set_vec3("dir_light.ambient",   light->ambient);
      light->shader->set_vec3("dir_light.diffuse",   light->diffuse);
      light->shader->set_vec3("dir_light.specular",  light->specular);
      light->shader->set_float("dir_light.strength", light->strength);
      light->shader->set_bool("use_dir_light",        true);
    }

  }
};
