// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/translation.hpp>

using namespace brenta::types;

translation::translation()
{
  this->view = glm::mat4(1.0f);
  this->projection = glm::mat4(1.0f);
  this->model = glm::mat4(1.0f);
}

translation::translation(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
{
  this->view = view;
  this->projection = projection;
  this->model = model;
}

void translation::set_view(glm::mat4 view)
{
  this->view = view;
}

void translation::set_projection(int window_width, int window_height,
                                 float fov, float near, float far)
{
  this->projection = glm::perspective(
    glm::radians(fov),
    (float) window_width / (float) window_height, near, far);
}

void translation::set_projection(glm::mat4 projection)
{
  this->projection = projection;
}

void translation::set_model(glm::mat4 new_model)
{
  this->model = new_model;
}

void translation::translate(glm::vec3 translation)
{
  this->model = glm::translate(this->model, translation);
}

// Note: the order of rotations is important
void translation::rotate(glm::vec3 rotation)
{
  this->model = glm::rotate(this->model, glm::radians(rotation.x),
                            glm::vec3(1.0f, 0.0f, 0.0f));
  this->model = glm::rotate(this->model, glm::radians(rotation.y),
                            glm::vec3(0.0f, 1.0f, 0.0f));
  this->model = glm::rotate(this->model, glm::radians(rotation.z),
                            glm::vec3(0.0f, 0.0f, 1.0f));
}

void translation::scale(float scale)
{
  this->model = glm::scale(this->model, glm::vec3(scale));
}

bool translation::set_shader(shader::name_t shader_name)
{
  if (!shader::use(shader_name))
    return false;

  if (!shader::set_mat4(shader_name, "view", this->view) ||
      !shader::set_mat4(shader_name, "projection", this->projection) ||
      !shader::set_mat4(shader_name, "model", this->model))
    return false;

  return true;
}
