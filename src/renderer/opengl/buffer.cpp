// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

Buffer::Buffer(Buffer::Target target)
{
  this->init(target);
  return;
}

Buffer::~Buffer()
{
  return;
}

void Buffer::init(Buffer::Target target)
{
  this->target = target;
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

Buffer::Id &Buffer::get_id()
{
  return this->id;
}

Buffer::Target &Buffer::get_target()
{
  return this->target;
}

void Buffer::set_id(Buffer::Id id)
{
  this->id = id;
}

void Buffer::set_target(Buffer::Target target)
{
  this->target = target;
}

void Buffer::copy_data(const void *data, GLsizeiptr size,
                       Buffer::DataUsage usage)
{
  glBufferData(this->target, size, data, (GLenum) usage);
  return;
}
