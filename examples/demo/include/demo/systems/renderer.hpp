// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/engine.hpp>
#include <demo/components/model.hpp>
#include <demo/components/player.hpp>
#include <demo/components/transform.hpp>

#include <viotecs/viotecs.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#define ANIMATION_SPEED 24

using namespace viotecs;

struct RendererSystem : system<ModelComponent, TransformComponent>
{
  void run(std::vector<entity_id> matches) const override
  {
    if (matches.empty())
    {
      INFO("RendererSystem: No matches");
      return;
    }

    for (auto match : matches)
    {
      /* Get the model component */
      auto model_component = world::entity_to_component<ModelComponent>(match);

      auto transform_component =
        world::entity_to_component<TransformComponent>(match);

      auto myModel = model_component->mod;
      auto default_shader = model_component->shader;

      brenta::types::translation t = brenta::types::translation();
      t.set_view(default_camera.get_view_matrix());
      t.set_projection(default_camera.get_projection_matrix(window::get_width(),
                                                            window::get_height()));

      t.set_model(glm::mat4(1.0f));
      t.translate(transform_component->position);
      t.rotate(transform_component->rotation);
      t.scale(transform_component->scale);

      t.set_shader(default_shader);

      shader::set_vec3(default_shader, "viewPos",
                       default_camera.get_position());
      shader::set_float(default_shader, "material.shininess",
                        model_component->shininess);

      /* Animation control */
      if (model_component->hasAtlas)
      {
        if (model_component->elapsedFrames > ANIMATION_SPEED)
        {
          model_component->elapsedFrames = 0;
          model_component->atlasIndex++;
          if (model_component->atlasIndex >= model_component->atlasSize)
          {
            model_component->atlasIndex = 0;
          }
        }
        else
        {
          model_component->elapsedFrames++;
        }
        shader::set_int(default_shader, "atlasSize",
                        model_component->atlasSize);
        shader::set_int(default_shader, "atlasIndex",
                        model_component->atlasIndex);
      }
      else
      {
        shader::set_int(default_shader, "atlasIndex", 0);
      }

      myModel.draw(default_shader);
    }
  }
};
