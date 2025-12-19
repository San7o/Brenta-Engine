// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/vao.hpp>

using namespace brenta::types;

void vao::init()
{
  glGenVertexArrays(1, &this->vao_id);
}

unsigned int vao::get_vao()
{
  if (vao_id == 0)
  {
    ERROR("vao: not initialized");
    return 0;
  }
  return vao_id;
}

void vao::bind()
{
  if (this->get_vao() == 0)
  {
    ERROR("vao: not initialized");
    return;
  }
  glBindVertexArray(this->get_vao());
}

void vao::unbind()
{
  glBindVertexArray(0);
}

void vao::set_vertex_data(buffer buffer, unsigned int index, GLint size,
                          GLenum type, GLboolean normalized, GLsizei stride,
                          const void *pointer)
{
  bind();
  buffer.bind();
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
  buffer.unbind();
  unbind();
}

void vao::destroy()
{
  if (this->get_vao() == 0)
  {
    ERROR("vao: not initialized");
    return;
  }
  glDeleteVertexArrays(1, &this->vao_id);
}
