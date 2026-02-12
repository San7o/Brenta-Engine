// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/vao.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

Vao::~Vao()
{
  this->destroy();
  return;
}

void Vao::init()
{
  glGenVertexArrays(1, &this->id);
  DEBUG("Vao: initialized");
  return;
}

void Vao::destroy()
{
  if (this->get_id() == 0) return;

  glDeleteVertexArrays(1, &this->id);
  DEBUG("Vao: destroyed");
  return;
}

void Vao::bind() const
{
  if (this->get_id() == 0)
  {
    ERROR("Vao::bind: not initialized");
    return;
  }
  glBindVertexArray(this->get_id());
  return;
}

void Vao::unbind() const
{
  glBindVertexArray(0);
  return;
}

unsigned int Vao::get_id() const
{
  if (id == 0)
    return 0;
  return id;
}

void Vao::link_buffer(Buffer &vbo,
                      unsigned int layout_index,
                      GLint components,
                      GLenum type,
                      GLboolean normalized,
                      GLsizei stride,
                      const void *offset)
{
  this->bind();
  vbo.bind();
  glVertexAttribPointer(layout_index, components, type,
                        normalized, stride, offset);
  glEnableVertexAttribArray(layout_index);
  return;
}

