// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h> // OpenGL driver

namespace brenta
{

class Buffer
{
public:

  using Id = unsigned int;
  
  enum Target {
    // vertex attributes (positions, normals...)
    Array             = GL_ARRAY_BUFFER,
    // indices for glDrawElements
    ElementArray      = GL_ELEMENT_ARRAY_BUFFER,
    // uniform blocks for shared shader data
    Uniform           = GL_UNIFORM_BUFFER,
    // buffer textures for large data sets
    Texture           = GL_TEXTURE_BUFFER,
    // capturing output from shaders
    TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
    // used as source/destinaiton for copying between buffers
    CopyRead          = GL_COPY_READ_BUFFER,
    // source for texture uploads (glTexImag2D)
    PixelUnpack       = GL_PIXEL_UNPACK_BUFFER,
    // Large, writable data structures
    ShaderStorage     = GL_SHADER_STORAGE_BUFFER,      
  };
  
  enum DataUsage {
    // The data is accessed only once and used by the GPU at most a
    // few times
    StreamDraw   = GL_STREAM_DRAW,
    StreamRead   = GL_STREAM_READ,
    StreamCopy   = GL_STREAM_COPY,
    // The data is accessed only once and used many times
    StaticDraw   = GL_STATIC_DRAW,
    StaticRead   = GL_STATIC_READ,
    StaticCopy   = GL_STATIC_COPY,
    // The data is accessed a lot and used many times
    DynamicDraw  = GL_DYNAMIC_DRAW,
    DynamicRead  = GL_DYNAMIC_READ,
    DynamicCopy  = GL_DYNAMIC_COPY,
  };
  
  Buffer() = default;
  Buffer(Target target);

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
  
  void init(Target target);
  void destroy();
  void bind() const;
  void unbind() const;

  // Getters

  Buffer::Id    &get_id();
  Target        &get_target();

  // Setters

  void set_id(Buffer::Id id);
  void set_target(Target target);

  // Utilities

  void copy_data(const void *data, GLsizeiptr size, DataUsage usage);

protected:
  
  Buffer::Id   id = 0;
  Target       target;

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
  
} // namespace brenta
