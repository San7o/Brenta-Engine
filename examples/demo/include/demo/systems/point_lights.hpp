// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/transform.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

// Load the lights on the shaders
struct PointLightsSystem : System<TransformComponent, PointLightComponent>
{
  void run(std::vector<EntityId> entities) const override
  {
    if (entities.empty())
      return;

    int counter = 0;
    for (auto entity : entities)
    {
      if (counter >= 4)
      {
        ERROR("Only 4 lights are supported");
        break;
      }
      auto transform = World::entity_to_component<TransformComponent>(entity);

      auto light = World::entity_to_component<PointLightComponent>(entity);

      if (!light->shader)
      {
        ERROR("Light shader not found");
        continue;
      }
      light->shader->use();

      // Set the light properties
      std::string lightn = "point_lights[" + std::to_string(counter) + "]";
      light->shader->set_vec3(lightn + ".ambient",    light->ambient);
      light->shader->set_vec3(lightn + ".diffuse",    light->diffuse);
      light->shader->set_vec3(lightn + ".specular",   light->specular);
      light->shader->set_float(lightn + ".constant",  light->constant);
      light->shader->set_float(lightn + ".linear",    light->linear);
      light->shader->set_float(lightn + ".quadratic", light->quadratic);
      light->shader->set_float(lightn + ".strength",  light->strength);
      light->shader->set_vec3(lightn + ".position",   transform->transform.get_pos());
      light->shader->set_float(lightn + ".strength",  light->strength);
      light->shader->set_int("n_point_lights",                    entities.size());

      counter++;
    }
  }
};
