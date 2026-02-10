// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <cassert>

namespace brenta
{

namespace types
{

/**
 * @brief FrameBuffer class
 *
 * This class is a wrapper around OpenGL frame buffers, providing
 * functions to create, bind, unbind, delete and rescale the frame
 * buffer.
 */
class framebuffer : public buffer
{
public:
  GLuint texture_id;
  GLuint render_buffer_id;
  /**
   * @brief Color format of the frame buffer
   * Default is GL_RGBA
   */
  GLenum format;
  /**
   * @brief Empty constructor
   * Does nothing
   */
  framebuffer()
  {
  }
  /**
   * @brief Constructor
   * Creates a framebuffer with a texture and a render buffer
   * @param width Width of the framebuffer
   * @param height Height of the framebuffer
   */
  framebuffer(int width, int height, GLenum format = GL_RGBA);
  /**
   * @brief Destructor
   * Deletes the framebuffer and its texture
   */
  ~framebuffer();
  
  void bind();
  void unbind();
  /**
   * @brief Delete the framebuffer and its texture
   */
  void destroy();
  void copy_data([[maybe_unused]] GLsizeiptr size,
                 [[maybe_unused]] const void *data,
                 [[maybe_unused]] GLenum usage)
  {
    assert(false && "TODO");
  }

  void copy_vertices([[maybe_unused]] GLsizeiptr size,
                     [[maybe_unused]] const void *data,
                     [[maybe_unused]] GLenum usage)
  {
    assert(false && "TODO");
  }
  void copy_indices([[maybe_unused]] GLsizeiptr size,
                    [[maybe_unused]] const void *data,
                    [[maybe_unused]] GLenum usage)
  {
    assert(false && "TODO");
  }
  /**
   * @brief Rescale the framebuffer
   * @param width New width of the framebuffer
   * @param height New height of the framebuffer
   */
  void rescale(int width, int height);
  /**
   * @brief Set the format of the framebuffer
   * @param format New format of the framebuffer
   */
  void set_format(GLenum format);
};

} // namespace types

} // namespace brenta
