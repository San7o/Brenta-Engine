// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/pipeline.hpp>
#include <brenta/window.hpp>

using namespace brenta;

//
// Static variables
//

Renderer::RenderData Renderer::data = {};

//
// Member functions
//

void Renderer::begin_frame()
{
  Renderer::clear();
  return;
}

void Renderer::begin_frame(Camera& cam)
{
  Renderer::clear();
  Renderer::set_camera(cam);
  return;
}

void Renderer::set_camera(Camera& cam)
{
  Renderer::data.projection =
    cam.get_projection_matrix(Window::get_width(),
                              Window::get_height());
  Renderer::data.view         = cam.get_view_matrix();
  Renderer::data.cam_position = cam.get_transform().get_pos();
}

void Renderer::submit(const Renderer::Command& it, bool transparent)
{
  if (transparent)
    Renderer::data.transparent_queue.push_back(it);
  else
    Renderer::data.opaque_queue.push_back(it);
}

void Renderer::submit_point_light(tenno::shared_ptr<PointLight> point_light)
{
  Renderer::data.point_lights.push_back(point_light);
}

void Renderer::submit_point_lights(const tenno::vector<tenno::shared_ptr<PointLight>> &point_lights)
{
  Renderer::data.point_lights.reserve(Renderer::data.point_lights.size()
                                      + point_lights.size());
  Renderer::data.point_lights.insert(Renderer::data.point_lights.end(),
                                     point_lights.begin(),
                                     point_lights.end());
}

void Renderer::submit_dir_light(tenno::shared_ptr<DirLight> dir_light)
{
  Renderer::data.dir_light = dir_light;
}

void Renderer::submit_text(const Text& text)
{
  Renderer::data.ui_queue.push_back(text);
}

void Renderer::submit_skybox(tenno::shared_ptr<Skybox> skybox)
{
  Renderer::data.skybox = skybox;
}

void Renderer::end_frame(tenno::shared_ptr<RenderPipeline> pipeline)
{
  Renderer::flush(pipeline);
}

void Renderer::clear()
{
  Renderer::data.opaque_queue.clear();
  Renderer::data.transparent_queue.clear();
  Renderer::data.ui_queue.clear();
  Renderer::data.point_lights.clear();
  Renderer::data.dir_light = {};
  Renderer::data.skybox    = {};
}

void Renderer::flush(tenno::shared_ptr<RenderPipeline> pipeline)
{
  pipeline->execute(Renderer::data);
  Renderer::clear();
  return;
}
