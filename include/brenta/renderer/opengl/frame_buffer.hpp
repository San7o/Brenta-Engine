// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <cassert>

namespace brenta
{

/**
 * @brief FrameBuffer class
 *
 * This class is a wrapper around OpenGL frame buffers, providing
 * functions to create, bind, unbind, delete and rescale the frame
 * buffer.
 */
class FrameBuffer : public Buffer
{
public:
  
  GLuint texture_id;
  GLuint render_buffer_id;
  GLenum color_format;

  FrameBuffer() {}
  FrameBuffer(int width, int height, GLenum format = GL_RGBA);
  ~FrameBuffer();
  
  void bind();
  void unbind();

  void destroy();
  void copy_data(GLsizeiptr size,
                 const void *data,
                 Buffer::DataUsage usage);

  void rescale(int width, int height);
  void set_color_format(GLenum color_format);
};

} // namespace brenta
