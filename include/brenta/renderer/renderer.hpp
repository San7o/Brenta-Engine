// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/point_light.hpp>
#include <brenta/renderer/dir_light.hpp>
#include <brenta/text.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <vector>

#pragma once

namespace brenta
{

//
// Renderer
// --------
//
// The renderer is a logical abtraction that is responsible to draw a
// "rendering unit" aka render command. When the renderer is flushed,
// or when `end_frame()` is called, all render commands will be
// drawn.
//
// Its usage usually looks like this:
//
//    Renderer::begin_frame(camera);
//
//    Renderer::submit({world_matrix, model});
//    Renderer::submit({world_matrix2, model2});
//    Renderer::submit_point_light(light);
//    // ...
//
//    Renderer::end_frame();  // draws everything    
//
class Renderer
{
public:

  class Command;
  
  Renderer() = delete;
  ~Renderer() = delete;

  // You can begin a frame withouth a camera, but eventually you will
  // have to call set_camera if you want to see anything
  static void begin_frame();
  static void begin_frame(Camera &cam);
  static void set_camera(Camera &cam);

  // Transparent commands are rendered after non-transparent ones
  static void submit(const Renderer::Command& it, bool transparent = false);
  static void submit_point_light(tenno::shared_ptr<PointLight> point_light);
  static void submit_point_lights(const tenno::vector<tenno::shared_ptr<PointLight>>& point_light);
  static void submit_dir_light(tenno::shared_ptr<DirLight> dir_light);
  static void submit_text(const Text& text);

  static void end_frame();

  // Draw and clear state
  static void flush();

private:

  static glm::mat4 projection;
  static glm::mat4 view;
  static glm::vec3 cam_position;
  static tenno::vector<tenno::shared_ptr<PointLight>>  point_lights;
  static std::optional<tenno::shared_ptr<DirLight>>    dir_light;
  static tenno::vector<Command> render_queue;
  static tenno::vector<Command> transparent_render_queue;
  static tenno::vector<Text>    text_render_queue;

  static void flush_command(const Renderer::Command& command);
  
};

class Renderer::Command
{
public:

  glm::mat4                    world_matrix;
  tenno::shared_ptr<Model>     model;
  
  Command() = default;
  Command(glm::mat4 world_matrix,
          tenno::shared_ptr<Model> model)
    : world_matrix(world_matrix), model(model) {}
};

} // namespace brenta
