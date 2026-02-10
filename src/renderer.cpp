// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer.hpp>
#include <brenta/window.hpp>
#include <brenta/translation.hpp>

using namespace brenta;

//
// Static variables
//

std::vector<renderer::item> renderer::render_queue = {};
glm::mat4 renderer::view;
glm::mat4 renderer::projection;
glm::vec3 renderer::cam_position;

//
// Member functions
//

void renderer::begin_frame(const camera& cam)
{
  renderer::render_queue.clear();
  renderer::projection =
    cam.get_projection_matrix(window::get_width(),
                              window::get_height());
  renderer::view = cam.get_view_matrix();
  renderer::cam_position = cam.get_world_pos();
}

void renderer::submit(const renderer::item& it)
{
  renderer::render_queue.push_back(it);
}

void renderer::end_frame()
{
  renderer::flush();
}

void renderer::flush()
{
  for (auto i : renderer::render_queue)
  {
    brenta::types::translation t = brenta::types::translation();
    t.set_view(renderer::view);
    t.set_projection(renderer::projection);
    t.set_model(i.transform);
    t.set_shader(i.material);

    shader::set_vec3(i.material, "viewPos", renderer::cam_position);
    shader::set_float(i.material, "material.shininess", 32.0f); // TODO
    //shader::set_int(i.material, "atlasIndex", 0); // TODO

    i.m->draw(i.material);
  }
}

//
// Item
//

renderer::item& renderer::item::translate(glm::vec3 translation)
{
  this->transform = glm::translate(this->transform, translation);
  return *this;
}

// Note: the order of rotations is important
renderer::item& renderer::item::rotate(glm::vec3 rotation)
{
  this->transform = glm::rotate(this->transform, glm::radians(rotation.x),
                                glm::vec3(1.0f, 0.0f, 0.0f));
  this->transform = glm::rotate(this->transform, glm::radians(rotation.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
  this->transform = glm::rotate(this->transform, glm::radians(rotation.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));
  return *this;
}

renderer::item& renderer::item::scale(float scale)
{
  this->transform = glm::scale(this->transform, glm::vec3(scale));
  return *this;
}
