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

tenno::vector<Renderer::Command> Renderer::render_queue             = {};
tenno::vector<tenno::shared_ptr<PointLight>> Renderer::point_lights = {};
std::optional<tenno::shared_ptr<DirLight>>   Renderer::dir_light    = {};
glm::mat4 Renderer::view;
glm::mat4 Renderer::projection;
glm::vec3 Renderer::cam_position;

//
// Member functions
//

void Renderer::begin_frame()
{
  Renderer::render_queue.clear();
  Renderer::point_lights.clear();
  Renderer::dir_light = {};
  return;
}

void Renderer::begin_frame(Camera& cam)
{
  Renderer::render_queue.clear();
  Renderer::point_lights.clear();
  Renderer::dir_light = {};
  Renderer::set_camera(cam);
  return;
}

void Renderer::set_camera(Camera& cam)
{
  Renderer::projection =
    cam.get_projection_matrix(Window::get_width(),
                              Window::get_height());
  Renderer::view = cam.get_view_matrix();
  Renderer::cam_position = cam.get_transform().get_pos();
}

void Renderer::submit(const Renderer::Command& it)
{
  Renderer::render_queue.push_back(it);
}

void Renderer::submit_point_light(tenno::shared_ptr<PointLight> point_light)
{
  Renderer::point_lights.push_back(point_light);
}

void Renderer::submit_point_lights(const tenno::vector<tenno::shared_ptr<PointLight>> &point_lights)
{
  Renderer::point_lights.reserve(Renderer::point_lights.size()
                                 + point_lights.size());
  Renderer::point_lights.insert(Renderer::point_lights.end(),
                                point_lights.begin(),
                                point_lights.end());
}

void Renderer::submit_dir_light(tenno::shared_ptr<DirLight> dir_light)
{
  Renderer::dir_light = dir_light;
}

void Renderer::end_frame()
{
  Renderer::flush();
}

void Renderer::flush()
{
  for (auto& command : Renderer::render_queue)
  {
    auto material = command.model->get_material();
    material->apply();

    // Setup all lights
    int lights_number = 0;
    if (Renderer::point_lights.size() > 0)
    {
      for (auto& point_light : Renderer::point_lights)
      {
        point_light->apply(lights_number);
        lights_number++;
      }
    }
    material->shader->set_int("n_point_lights", lights_number);

    if (Renderer::dir_light)
      Renderer::dir_light.value()->apply();

    material->shader->set_mat4("view",       Renderer::view);
    material->shader->set_mat4("projection", Renderer::projection);
    material->shader->set_mat4("model",
                             // Node world position
                             command.world_matrix *
                             // Local transform
                             command.model->get_transform().get_model_matrix());
    material->shader->set_vec3("view_pos",    Renderer::cam_position);

    // Geometry
    command.model->draw();
  }

  Renderer::render_queue.clear();
  return;
}
