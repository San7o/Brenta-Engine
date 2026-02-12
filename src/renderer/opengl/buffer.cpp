// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

Buffer::Buffer(GLenum input_target)
{
  this->init(input_target);
  return;
}

Buffer::~Buffer()
{
  return;
}

void Buffer::init(GLenum input_target)
{
  this->target = input_target;
  glGenBuffers(1, &id);
  
  DEBUG("buffer: initialized");
  return;
}

void Buffer::destroy()
{
  if (this->id == 0) return;

  glDeleteBuffers(1, &this->id);
  this->id = 0;

  DEBUG("buffer: destroyed");
  return;
}

void Buffer::bind()
{
  if (this->id == 0)
  {
    ERROR("Buffer::bind: not initialized");
    return;
  }
  glBindBuffer(this->target, this->id);
  return;
}

void Buffer::unbind()
{
  glBindBuffer(this->target, 0);
  return;
}

int Buffer::get_id()
{
  return this->id;
}

GLenum Buffer::get_target()
{
  return this->target;
}

void Buffer::set_id(unsigned int id)
{
  this->id = id;
}

void Buffer::set_target(GLenum target)
{
  this->target = target;
}

void Buffer::copy_data(const void *data, GLsizeiptr size,
                       Buffer::DataUsage usage)
{
  glBufferData(this->target, size, data, (GLenum) usage);
  return;
}
