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
  
  Model         *model;
  const Material       material;

  Command() = default;
  Command(Model *model,
          const Material &material)
    : model(model), material(material) {}
};
  
} // namespace brenta
