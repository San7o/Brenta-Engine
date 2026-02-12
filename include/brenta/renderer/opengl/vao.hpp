// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>

#include <glad/glad.h>

namespace brenta
{

//
// Vertex array object
// -------------------
//
// Specifies how to read the data from another buffer, usually the
// VBO which contains vertex data.
//
// Use the `link_buffer` function for this, after having initialized
// and bound the object.
//
// You usually follow this patter:
//  
//    Vao vao;
//    vao.init();
//    vao.bind();
//
//    Buffer vbo;
//    vbo.init(Buffer::Target::Array);
//    vbo.bind();
//
//    vbo.copy_data(vertices, sizeof(vertices), Buffer::DataUsage::StaticDraw);
//    vao.link_buffer(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
class Vao
{
public:

  Vao() {}
  constexpr Vao(const Vao& other)            = delete;
  constexpr Vao& operator=(const Vao& other) = delete;
  constexpr Vao(Vao&& other) noexcept
  {
    this->id = other.id;
    other.id = 0;
  }
  constexpr Vao& operator=(Vao&& other) noexcept
  {
    this->id = other.id;
    other.id = 0;
    return *this;
  }

  ~Vao();
  
  void         init();
  void         destroy();
  void         bind() const;
  void         unbind() const;
  unsigned int get_id() const;

  // Specifies the buffer data layout, and makes it accessible in
  // the [layout_index] location
  //
  // Args:
  // - vbo           the buffer containing the vertex data.
  // - layout_index: the `location` index specified in the shader
  //            (e.g., layout(location = 0))
  // - components:   the number of components per attribute
  // - type:         the data type of each component (e.g. GL_FLOAT)
  // - normalized:   whether the data should be mapped to [0, 1] or
  //                 [-1, 1] range
  // - stride:       the byte distance between the start of one
  //                 attribute and the next (0 lets OpenGL calculate
  //                 it based on 'type' and 'components')
  // - offset:       the byte offset of the first attribute in the buffer
  void link_buffer(const Buffer &vbo,
                   unsigned int layout_index,
                   GLint components,
                   GLenum type,
                   GLboolean normalized,
                   GLsizei stride,
                   const void *offset);

private:

  unsigned int id;

};

} // namespace brenta
