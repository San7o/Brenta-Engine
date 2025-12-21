// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/model.hpp>
#include <brenta/shader.hpp>
#include <brenta/camera.hpp>
#include <brenta/model.hpp>

#include <glm/glm.hpp>

#pragma once

namespace brenta
{

class renderer
{
public:

  class item;

  renderer() = delete;
  ~renderer() = delete;
  
  static void begin_frame(const camera& cam);
  static void submit(const renderer::item& it);
  static void end_frame();

private:

  static glm::mat4 projection;
  static glm::mat4 view;
  static glm::vec3 cam_position;
  static std::vector<item> render_queue;
  
  static void flush();

};

class renderer::item
{
public:
  const model* m;
  const shader::name_t material;
  glm::mat4 transform;

  item() = default;
  item(const model* m,
       const shader::name_t material,
       const glm::mat4 transform = glm::mat4(1.0))
    : m(m), material(material), transform(transform) {}

  renderer::item& translate(glm::vec3 translation);
  renderer::item& rotate(glm::vec3 rotation);
  renderer::item& scale(float scale);

};
  
} // namespace brenta
