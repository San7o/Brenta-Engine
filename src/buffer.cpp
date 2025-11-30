// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/buffer.hpp>
#include <brenta/logger.hpp>

using namespace brenta;
using namespace brenta::types;

buffer::buffer(GLenum input_target)
{
  this->target = input_target;
  glGenBuffers(1, &id);
  bind();
}

void buffer::copy_data(GLsizeiptr size, const void *data, GLenum usage)
{
  glBufferData(this->target, size, data, usage);
}

void buffer::copy_indices(GLsizeiptr size, const void *data, GLenum usage)
{
  if (this->target != GL_ELEMENT_ARRAY_BUFFER)
    return;
  bind();
  glBufferData(this->target, size, data, usage);
}

void buffer::copy_vertices(GLsizeiptr size, const void *data, GLenum usage)
{
  if (this->target == GL_ELEMENT_ARRAY_BUFFER)
    return;
  bind();
  glBufferData(this->target, size, data, usage);
}

void buffer::bind()
{
  if (this->id == 0)
  {
    ERROR("Buffer not initialized");
    return;
  }
  glBindBuffer(this->target, this->id);
}

void buffer::unbind()
{
  glBindBuffer(this->target, 0);
}

void buffer::destroy()
{
  if (this->id == 0)
  {
    ERROR("Buffer not initialized");
    return;
  }
  glDeleteBuffers(1, &this->id);
}

int buffer::get_id()
{
  return this->id;
}

GLenum buffer::get_target()
{
  return this->target;
}

void buffer::set_id(unsigned int id)
{
  this->id = id;
}

void buffer::set_target(GLenum target)
{
  this->target = target;
}
