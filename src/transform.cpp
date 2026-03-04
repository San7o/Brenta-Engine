// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/transform.hpp>

using namespace brenta;

Transform::Transform(double x, double y, double z)
{
  this->position = glm::vec3(x, y, z);
  this->dirty    = true;
  return;
}

Transform::Transform(glm::vec3 vec)
{
  this->position = vec;
  this->dirty    = true;
  return;
}

glm::mat4 Transform::get_model_matrix()
{
  if (this->dirty)
  {
    glm::mat4 T = glm::translate(glm::mat4(1.0f), this->position);
    glm::mat4 R = glm::mat4_cast(this->rotation);
    glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scaling);

    this->model_matrix = T * R * S;
    this->dirty        = false;
  }

  return this->model_matrix;
}

glm::vec3 Transform::get_pos() const
{
  return this->position;
}
  
float Transform::get_x() const
{
  return this->position.x;
}

float Transform::get_y() const
{
  return this->position.y;
}

float Transform::get_z() const
{
  return this->position.z;
}

glm::quat Transform::get_rotation() const
{
  return this->rotation;
}

Transform& Transform::set_pos(glm::vec3 new_pos)
{
  this->position = new_pos;
  this->dirty    = true;
  return *this;
}

Transform& Transform::set_x(float x)
{
  this->position.x = x;
  this->dirty      = true;
  return *this;
}

Transform& Transform::set_y(float y)
{
  this->position.y = y;
  this->dirty      = true;
  return *this;
}

Transform& Transform::set_z(float z)
{
  this->position.z = z;
  this->dirty      = true;
  return *this;
}
  
Transform& Transform::translate(const glm::vec3& translation)
{
  this->position = this->position + translation;
  this->dirty    = true;
  return *this;
}

Transform& Transform::rotate(const glm::quat &rotation)
{
  this->rotation *= rotation;
  this->dirty     = true;
  return *this;
}

Transform& Transform::rotate_x(float degrees)
{
  this->rotation *= glm::angleAxis(glm::radians(degrees),
                                   glm::vec3(1.0f, 0.0f, 0.0f));
  this->dirty     = true;
  return *this;
}

Transform& Transform::rotate_y(float degrees)
{
  this->rotation *= glm::angleAxis(glm::radians(degrees),
                                   glm::vec3(0.0f, 1.0f, 0.0f));
  this->dirty     = true;
  return *this;
}

Transform& Transform::rotate_z(float degrees)
{
  this->rotation *= glm::angleAxis(glm::radians(degrees),
                                   glm::vec3(0.0f, 0.0f, 1.0f));
  this->dirty     = true;
  return *this;
}

Transform& Transform::scale(const glm::vec3& scale)
{
  this->scaling = scale;
  this->dirty   = true;
  return *this;
}
