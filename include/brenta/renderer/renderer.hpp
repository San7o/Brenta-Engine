// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/material.hpp>

#include <glm/glm.hpp>

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
  
  static void begin_frame(Camera& cam);
  static void submit(const Renderer::Command& it);
  static void end_frame();

private:

  static glm::mat4 projection;
  static glm::mat4 view;
  static glm::vec3 cam_position;
  static std::vector<Command> render_queue;
  
  static void flush();

};

class Renderer::Command
{
public:
  
  Model        *model;
  
  Command() = default;
  Command(Model *model)
    : model(model) {}
};
  
} // namespace brenta
