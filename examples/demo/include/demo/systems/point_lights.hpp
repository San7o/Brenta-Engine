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
        std::string lightn = "pointLights[" + std::to_string(counter) + "]";
        shader->set_vec3((lightn + ".ambient").c_str(),    light->ambient);
        shader->set_vec3((lightn + ".diffuse").c_str(),    light->diffuse);
        shader->set_vec3((lightn + ".specular").c_str(),   light->specular);
        shader->set_float((lightn + ".constant").c_str(),  light->constant);
        shader->set_float((lightn + ".linear").c_str(),    light->linear);
        shader->set_float((lightn + ".quadratic").c_str(), light->quadratic);
        shader->set_float((lightn + ".strength").c_str(),  light->strength);
        shader->set_vec3((lightn + ".position").c_str(),   transform->transform.get_pos());
        shader->set_float((lightn + ".strength").c_str(),  light->strength);
        shader->set_int("nPointLights",                    entities.size());
      }
      counter++;
    }
  }
};
