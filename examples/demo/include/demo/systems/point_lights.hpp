// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <demo/components/transform.hpp>

#include <viotecs/viotecs.hpp>

#include <vector>

using namespace viotecs;

/* Load the lights on the shaders */
struct PointLightsSystem : system<TransformComponent, PointLightComponent>
{
  void run(std::vector<entity_id> entities) const override
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
      auto transform = world::entity_to_component<TransformComponent>(entity);

      auto light = world::entity_to_component<PointLightComponent>(entity);

      for (auto shader : light->shaders)
      {
        if (Shader::get_id(shader) == (unsigned int) 0)
        {
          ERROR("Light shader not found with name: {}", shader);
          continue;
        }
        Shader::use(shader);

        /* Set the light properties */
        std::string lightn = "pointLights[" + std::to_string(counter) + "]";
        Shader::set_vec3(shader, (lightn + ".ambient").c_str(), light->ambient);
        Shader::set_vec3(shader, (lightn + ".diffuse").c_str(), light->diffuse);
        Shader::set_vec3(shader, (lightn + ".specular").c_str(),
                         light->specular);
        Shader::set_float(shader, (lightn + ".constant").c_str(),
                          light->constant);
        Shader::set_float(shader, (lightn + ".linear").c_str(), light->linear);
        Shader::set_float(shader, (lightn + ".quadratic").c_str(),
                          light->quadratic);
        Shader::set_float(shader, (lightn + ".strength").c_str(),
                          light->strength);
        Shader::set_vec3(shader, (lightn + ".position").c_str(),
                         transform->position);
        Shader::set_float(shader, (lightn + ".strength").c_str(),
                          light->strength);
        Shader::set_int(shader, "nPointLights", entities.size());
      }
      counter++;
    }
  }
};
