// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/buffer.hpp>
#include <glad/glad.h>

namespace brenta
{

namespace types
{

/**
 * @brief Vertex Array Object (VAO)
 *
 * Wrapper for OpenGL Vertex Array Objects
 */
class vao
{
public:
  /**
   * @brief Vertex Array Object (VAO)
   */
  unsigned int vao_id;

  /**
   * @brief Empty Constructor
   *
   * Does nothing
   */
  vao()
  {
  }
  /**
   * @brief Init Constructor
   *
   * Creates a new VAO
   */
  void init();

  /**
   * @brief Get the VAO
   * @return The VAO
   */
  unsigned int get_vao();
  /**
   * @brief Bind the VAO
   */
  void bind();
  /**
   * @brief Unbind the VAO
   */
  void unbind();
  /**
   * @brief Delete the VAO
   */
  void destroy();
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
  void set_vertex_data(buffer buffer, unsigned int index, GLint size,
                       GLenum type, GLboolean is_normalized, GLsizei stride,
                       const void *pointer);
};

} // namespace types

} // namespace brenta
