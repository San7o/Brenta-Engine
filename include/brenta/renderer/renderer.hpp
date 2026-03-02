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
// The renderer is a logical abtraction that is responsible to draw
// a "rendering unit" aka render command. It takes all the
// information for a render call and it performs the drawing,
// calculating view, projection and model matrixes.
//
class Renderer
{
public:

  class Command;
  
  Renderer() = delete;
  ~Renderer() = delete;
  
  static void begin_frame(tenno::shared_ptr<Camera> cam);
  
  static void submit(const Renderer::Command& it);
  static void submit_point_light(tenno::shared_ptr<PointLight> point_light);
  static void submit_point_lights(const tenno::vector<tenno::shared_ptr<PointLight>>& point_light);
  static void submit_dir_light(tenno::shared_ptr<DirLight> dir_light);

  static void end_frame();

  // Draw and clear state
  static void flush();

private:

  static glm::mat4 projection;
  static glm::mat4 view;
  static glm::vec3 cam_position;
  static tenno::vector<tenno::shared_ptr<PointLight>>  point_lights;
  static std::optional<tenno::shared_ptr<DirLight>>  dir_light;
  static tenno::vector<Command> render_queue;
  
};

class Renderer::Command
{
public:

  glm::mat4                    world_matrix;
  tenno::shared_ptr<Model>     model;
  
  Command() = default;
  Command(glm::mat4 world_matrix,
          tenno::shared_ptr<Model>    model)
    : world_matrix(world_matrix), model(model)
  {}
};

} // namespace brenta
