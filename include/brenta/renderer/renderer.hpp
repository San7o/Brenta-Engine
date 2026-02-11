// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/model.hpp>

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
  
  static void begin_frame(const Camera& cam);
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
  
  const Model         *m;
  const Shader::Name   material;
  glm::mat4            transform;

  Command() = default;
  Command(const Model* m,
          const Shader::Name material,
          const glm::mat4 transform = glm::mat4(1.0))
    : m(m), material(material), transform(transform) {}

  Renderer::Command& translate(glm::vec3 translation);
  Renderer::Command& rotate(glm::vec3 rotation);
  Renderer::Command& scale(float scale);

};
  
} // namespace brenta
