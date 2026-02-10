// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/logger.hpp>

using namespace brenta;
using namespace brenta::types;

buffer::buffer(GLenum input_target)
{
  this->init(input_target);
}

buffer::~buffer()
{
  this->destroy();
}

void buffer::copy_data(GLsizeiptr size, const void *data, GLenum usage)
{
  glBufferData(this->target, size, data, usage);
}

void buffer::copy_indices(GLsizeiptr size, const void *data, GLenum usage)
{
  if (this->target != GL_ELEMENT_ARRAY_BUFFER)
    return;
  
  this->bind();
  glBufferData(this->target, size, data, usage);
}

void buffer::copy_vertices(GLsizeiptr size, const void *data, GLenum usage)
{
  if (this->target == GL_ELEMENT_ARRAY_BUFFER)
    return;
  
  this->bind();
  glBufferData(this->target, size, data, usage);
}

void buffer::init(GLenum input_target)
{
  this->target = input_target;
  glGenBuffers(1, &id);
  
  DEBUG("buffer: initialized");
}

void buffer::destroy()
{
  if (this->id == 0) return;

  glDeleteBuffers(1, &this->id);
  this->id = 0;

  DEBUG("buffer: destroyed");
}

void buffer::bind()
{
  if (this->id == 0)
  {
    ERROR("buffer::bind: not initialized");
    return;
  }
  glBindBuffer(this->target, this->id);
}

void buffer::unbind()
{
  glBindBuffer(this->target, 0);
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
