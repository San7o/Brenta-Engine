// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/translation.hpp>

using namespace brenta;

void Translation::set_view(glm::mat4 view)
{
  this->view = view;
  return;
}

void Translation::set_projection(int window_width, int window_height,
                                 float fov, float near, float far)
{
  this->projection = glm::perspective(
    glm::radians(fov),
    (float) window_width / (float) window_height, near, far);
  return;
}

void Translation::set_projection(glm::mat4 projection)
{
  this->projection = projection;
  return;
}

void Translation::set_model(glm::mat4 new_model)
{
  this->model = new_model;
  return;
}

void Translation::translate(glm::vec3 translation)
{
  this->model = glm::translate(this->model, translation);
  return;
}

// Note: the order of rotations is important
void Translation::rotate(glm::vec3 rotation)
{
  this->model = glm::rotate(this->model, glm::radians(rotation.x),
                            glm::vec3(1.0f, 0.0f, 0.0f));
  this->model = glm::rotate(this->model, glm::radians(rotation.y),
                            glm::vec3(0.0f, 1.0f, 0.0f));
  this->model = glm::rotate(this->model, glm::radians(rotation.z),
                            glm::vec3(0.0f, 0.0f, 1.0f));
}

void Translation::scale(float scale)
{
  this->model = glm::scale(this->model, glm::vec3(scale));
}

bool Translation::set_shader(Shader::Name shader_name)
{
  if (!Shader::use(shader_name))
    return false;

  if (!Shader::set_mat4(shader_name, "view", this->view) ||
      !Shader::set_mat4(shader_name, "projection", this->projection) ||
      !Shader::set_mat4(shader_name, "model", this->model))
    return false;

  return true;
}
