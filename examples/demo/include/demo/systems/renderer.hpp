// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/opengl/shader.hpp>
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

    brenta::Camera *cam = world::get_resource<CameraResource>()->cam;

    for (auto match : matches)
    {
      // Get the model component
      auto model_component = world::entity_to_component<ModelComponent>(match);

      auto transform_component =
        world::entity_to_component<TransformComponent>(match);

      Model *m = &model_component->mod;
      auto shader_name = model_component->shader;
      auto shader = Shader::get_shader(shader_name);
      if (!shader) continue;

      shader->use();
      shader->set_mat4("view", cam->get_view_matrix());
      shader->set_mat4("projection",
                       cam->get_projection_matrix(Window::get_width(),
                                                  Window::get_height()));
      shader->set_mat4("model", transform_component->transform.get_model_matrix());
      shader->set_vec3("viewPos", cam->get_transform().get_pos());
      shader->set_float("material.shininess", model_component->shininess);

      // Animation control
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
        shader->set_int("atlasSize", model_component->atlasSize);
        shader->set_int("atlasIndex", model_component->atlasIndex);
      }
      else
      {
        shader->set_int("atlasIndex", 0);
      }

      m->draw(shader_name);
    }
  }
};
