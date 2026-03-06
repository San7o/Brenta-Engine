// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/opaque_pass.hpp>

using namespace brenta;

OpaquePass::OpaquePass(tenno::shared_ptr<FrameBuffer> fb)
{
  this->fb = fb;
}

void OpaquePass::begin()
{
  this->fb->bind();
}

void OpaquePass::end()
{
  this->fb->unbind();
}

void OpaquePass::execute(const Renderer::RenderData& data)
{
  for (auto& command : data.opaque_queue)
  {
    auto material = command.model->material;
    material->apply();

    // Setup all lights
    int lights_number = 0;
    if (data.point_lights.size() > 0)
    {
      for (auto& point_light : data.point_lights)
      {
        point_light->apply(lights_number);
        lights_number++;
      }
    }
    material->shader->set_int("n_point_lights", lights_number);

    if (data.dir_light)
      data.dir_light.value()->apply();

    material->shader->set_mat4("view",       data.view);
    material->shader->set_mat4("projection", data.projection);
    material->shader->set_mat4("model",
                               // Node world position
                               command.world_matrix *
                               // Local transform
                               command.model->transform.get_model_matrix());
    material->shader->set_vec3("view_pos",    data.cam_position);

    // Geometry
    command.model->draw();
  }
}
