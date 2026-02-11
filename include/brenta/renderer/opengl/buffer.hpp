// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h> // OpenGL driver

namespace brenta
{

/**
 * @brief Buffer wrapper around OpenGL buffer objects
 *
 * This class is a wrapper around OpenGL buffer objects. It provides a
 * simple interface to create, bind, unbind and delete buffer objects.
 */
class Buffer
{
public:

  unsigned int id;
  GLenum target;

  Buffer() = default;
  Buffer(GLenum input_target);

  constexpr Buffer(const Buffer&)            = delete;
  constexpr Buffer& operator=(const Buffer&) = delete;

  constexpr Buffer(Buffer&& other) noexcept
  {
    this->id = other.id;
    this->target = other.target;
    other.id = 0;
  }
  constexpr Buffer& operator=(Buffer&& other) noexcept
  {
    this->id = other.id;
    this->target = other.target;
    other.id = 0;
    return *this;
  }

  ~Buffer();
  
  void init(GLenum input_target);
  void destroy();
  void bind();
  void unbind();

  // Getters

  int    get_id();
  GLenum get_target();

  // Setters

  void set_id(unsigned int id);
  void set_target(GLenum target);

  // Utilities

  // copy data inside buffer
  void copy_data(GLsizeiptr size, const void *data, GLenum usage);
  void copy_vertices(GLsizeiptr size, const void *data, GLenum usage);
  void copy_indices(GLsizeiptr size, const void *data, GLenum usage);
};

} // namespace brenta
