// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/renderer.hpp>
#include <brenta/window.hpp>

using namespace brenta;

//
// Static variables
//

std::vector<Renderer::Command> Renderer::render_queue = {};
glm::mat4 Renderer::view;
glm::mat4 Renderer::projection;
glm::vec3 Renderer::cam_position;

//
// Member functions
//

void Renderer::begin_frame(std::shared_ptr<Camera> cam)
{
  Renderer::render_queue.clear();
  Renderer::projection =
    cam->get_projection_matrix(Window::get_width(),
                               Window::get_height());
  Renderer::view = cam->get_view_matrix();
  Renderer::cam_position = cam->get_transform().get_pos();

  return;
}

void Renderer::submit(const Renderer::Command& it)
{
  Renderer::render_queue.push_back(it);
}

void Renderer::end_frame()
{
  Renderer::flush();
}

void Renderer::flush()
{
  for (auto& command : Renderer::render_queue)
  {
    auto& material = command.model->get_material();
    material.apply();
    
    material.shader.set_mat4("view",       Renderer::view);
    material.shader.set_mat4("projection", Renderer::projection);
    material.shader.set_mat4("model",      command.model->get_transform().get_model_matrix());
    material.shader.set_vec3("viewPos",    Renderer::cam_position);

    // TODO: move this to material
    material.shader.set_float("material.shininess", 32.0f);
    
    // shader->set_int("atlasIndex", 0); // TODO

    command.model->draw();
  }

  Renderer::render_queue.clear();
  return;
}
