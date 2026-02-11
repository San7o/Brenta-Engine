// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/translation.hpp>
#include <brenta/window.hpp>

using namespace brenta;

//
// Static variables
//

std::vector<Renderer::Target> Renderer::render_queue = {};
glm::mat4 Renderer::view;
glm::mat4 Renderer::projection;
glm::vec3 Renderer::cam_position;

//
// Member functions
//

void Renderer::begin_frame(const Camera& cam)
{
  Renderer::render_queue.clear();
  Renderer::projection =
    cam.get_projection_matrix(Window::get_width(),
                              Window::get_height());
  Renderer::view = cam.get_view_matrix();
  Renderer::cam_position = cam.get_world_pos();

  return;
}

void Renderer::submit(const Renderer::Target& it)
{
  Renderer::render_queue.push_back(it);
}

void Renderer::end_frame()
{
  Renderer::flush();
}

void Renderer::flush()
{
  for (auto i : Renderer::render_queue)
  {
    Translation t = Translation();
    t.set_view(Renderer::view);
    t.set_projection(Renderer::projection);
    t.set_model(i.transform);
    if (!t.set_shader(i.material))
    {
      ERROR("Renderer::flust: error setting translation");
      continue;
    }

    auto shader = Shader::get_shader(i.material);
    if (!shader)
    {
      ERROR("Renderer::flush: error accessing shader named {}",
            i.material);
      continue;
    }
    
    shader->use();
    shader->set_vec3("viewPos", Renderer::cam_position);
    shader->set_float("material.shininess", 32.0f); // TODO
    // shader->set_int("atlasIndex", 0); // TODO

    i.m->draw(i.material);
  }
  return;
}

//
// Target
//

Renderer::Target& Renderer::Target::translate(glm::vec3 translation)
{
  this->transform = glm::translate(this->transform, translation);
  return *this;
}

// Note: the order of rotations is important
Renderer::Target& Renderer::Target::rotate(glm::vec3 rotation)
{
  this->transform = glm::rotate(this->transform, glm::radians(rotation.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));
  this->transform = glm::rotate(this->transform, glm::radians(rotation.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
  this->transform = glm::rotate(this->transform, glm::radians(rotation.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));
  return *this;
}

Renderer::Target& Renderer::Target::scale(float scale)
{
  this->transform = glm::scale(this->transform, glm::vec3(scale));
  return *this;
}
