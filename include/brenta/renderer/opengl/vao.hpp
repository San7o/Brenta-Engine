// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>

#include <glad/glad.h>

namespace brenta
{

/**
 * @brief Vertex Array Object (VAO)
 *
 * Wrapper for OpenGL Vertex Array Objects
 */
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
  
  void init();
  void destroy();
  void bind() const;
  void unbind() const;
  unsigned int get_id() const;

  /**
   * @brief Set the vertex data
   *
   * @param buffer The buffer object
   * @param index The index of the vertex attribute
   * @param size The number of components per attribute
   * @param type The data type of each component
   * @param is_normalized Whether the data should be normalized
   * @param stride The byte offset between consecutive generic vertex
   * attributes
   * @param pointer The offset of the first component of the first generic
   * vertex attribute in the array
   */
  void set_vertex_data(Buffer &buffer, unsigned int index, GLint size,
                       GLenum type, GLboolean is_normalized, GLsizei stride,
                       const void *pointer);

private:

  unsigned int id;

};

} // namespace brenta
