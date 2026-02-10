// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/vao.hpp>
#include <brenta/logger.hpp>

using namespace brenta::types;

vao::vao()
{
}

vao::~vao()
{
  this->destroy();
}

void vao::init()
{
  glGenVertexArrays(1, &this->vao_id);
  DEBUG("vao: initialized");
}

void vao::destroy()
{
  if (this->get_id() == 0) return;

  glDeleteVertexArrays(1, &this->vao_id);
  DEBUG("vao: destroyed");
}

void vao::bind() const
{
  if (this->get_id() == 0)
  {
    ERROR("vao::bind: not initialized");
    return;
  }
  glBindVertexArray(this->get_id());
}

void vao::unbind() const
{
  glBindVertexArray(0);
}

unsigned int vao::get_id() const
{
  if (vao_id == 0)
    return 0;
  return vao_id;
}

void vao::set_vertex_data(buffer &buffer, unsigned int index, GLint size,
                          GLenum type, GLboolean normalized, GLsizei stride,
                          const void *pointer)
{
  this->bind();
  buffer.bind();
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
  buffer.unbind();
  this->unbind();
}
