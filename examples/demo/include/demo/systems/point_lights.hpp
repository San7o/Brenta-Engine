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
        std::string lightn = "point_lights[" + std::to_string(counter) + "]";
        shader->set_vec3(lightn + ".ambient",    light->ambient);
        shader->set_vec3(lightn + ".diffuse",    light->diffuse);
        shader->set_vec3(lightn + ".specular",   light->specular);
        shader->set_float(lightn + ".constant",  light->constant);
        shader->set_float(lightn + ".linear",    light->linear);
        shader->set_float(lightn + ".quadratic", light->quadratic);
        shader->set_float(lightn + ".strength",  light->strength);
        shader->set_vec3(lightn + ".position",   transform->transform.get_pos());
        shader->set_float(lightn + ".strength",  light->strength);
        shader->set_int("n_point_lights",                    entities.size());
      }

      counter++;
    }
  }
};
