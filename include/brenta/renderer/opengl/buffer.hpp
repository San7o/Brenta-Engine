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

  enum DataUsage {
    // The data is set only once and used by the GPU at most a few
    // times
    Stream   = GL_STREAM_DRAW,
    // The data is set only once and used many times
    Static   = GL_STATIC_DRAW,
    // The data is changed a lot and used many times
    Dynamic  = GL_DYNAMIC_DRAW,
  };
  
  unsigned int id;
  GLenum       target;

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

  void copy_data(const void *data, GLsizeiptr size, DataUsage usage);
};

//
// Vertex buffer obejcts
// ---------------------
//  
// VBOs store vertex data in the GPU as raw bytes: it
// does not know the structure of the data. For this reason, we use
// a vao.
typedef Buffer Vbo;

//
// Element buffer objects
// ----------------------
//
// Used to aboid duplicating vertices by reusing them with indices
typedef Buffer Ebo;

//
// Feedback buffer objects
// ------------------------
//
// These are used to save the state of some uniforms after draw call
typedef Buffer Fbo;
  
//
// Uniform buffer objects
// ----------------------
//
// These are used to set multiple uniforms with a single command
typedef Buffer Ubo;
  
} // namespace brenta
