// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/translation.hpp>
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

      brenta::Translation t = brenta::Translation();
      t.set_view(cam->get_view_matrix());
      t.set_projection(cam->get_projection_matrix(Window::get_width(),
                                                  Window::get_height()));

      t.set_model(glm::mat4(1.0f));
      t.translate(transform_component->position);
      t.rotate(transform_component->rotation);
      t.scale(transform_component->scale);

      t.set_shader(shader_name);

      shader->set_vec3("viewPos", cam->get_world_pos());
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
