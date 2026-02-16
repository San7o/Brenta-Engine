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

std::vector<Renderer::Command> Renderer::render_queue           = {};
std::vector<std::shared_ptr<PointLight>> Renderer::point_lights = {};
std::optional<std::shared_ptr<DirLight>> Renderer::dir_light    = {};
glm::mat4 Renderer::view;
glm::mat4 Renderer::projection;
glm::vec3 Renderer::cam_position;

//
// Member functions
//

void Renderer::begin_frame(std::shared_ptr<Camera> cam,
                           std::vector<std::shared_ptr<PointLight>> point_lights,
                           std::optional<std::shared_ptr<DirLight>> dir_light)
{
  Renderer::render_queue.clear();
  Renderer::projection =
    cam->get_projection_matrix(Window::get_width(),
                               Window::get_height());
  Renderer::view = cam->get_view_matrix();
  Renderer::cam_position = cam->get_transform().get_pos();
  Renderer::point_lights = point_lights;
  Renderer::dir_light    = dir_light;

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

    if (Renderer::point_lights.size() > 0)
    {
      material.shader.set_int("n_point_lights", Renderer::point_lights.size());
      for (std::size_t i = 0; i < Renderer::point_lights.size(); ++i)
        Renderer::point_lights[i]->apply(i);
    }
    if (Renderer::dir_light)
      Renderer::dir_light.value()->apply();
    
    material.shader.set_mat4("view",       Renderer::view);
    material.shader.set_mat4("projection", Renderer::projection);
    material.shader.set_mat4("model",      command.model->get_transform().get_model_matrix());
    material.shader.set_vec3("view_pos",    Renderer::cam_position);

    command.model->draw();
  }

  Renderer::render_queue.clear();
  return;
}
