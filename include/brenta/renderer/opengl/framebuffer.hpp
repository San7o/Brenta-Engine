// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>

#include <cassert>

namespace brenta
{

//
// Framebuffer
// -----------
//
// The framebuffer stores a Texture that is rendered into after a
// draw call. You usually use it like this:
//
//      FrameBuffer fb(Window::get_width(), Window::get_height());
//      // setup...
//
//      fb.bind();
//      Draw();
//      fb.unbind();
//
class FrameBuffer : public Buffer
{
public:
  
  GLuint texture_id;
  GLuint render_buffer_id;
  GLenum color_format;
  GLenum channel_type = GL_UNSIGNED_BYTE;
  int    width;
  int    height;

  // Used for profiling
  int        memory = 0;
  static int tot_memory;
  
  FrameBuffer() = default;
  FrameBuffer(int width, int height, GLenum format = GL_RGBA);

  FrameBuffer(FrameBuffer&& other)
  {
    this->texture_id       = other.texture_id;
    this->render_buffer_id = other.render_buffer_id;
    this->color_format     = other.color_format;
    this->width            = other.width;
    this->height           = other.height;
    this->target           = other.target;
    this->id               = other.id;
    this->memory           = other.memory;

    other.id     = 0;
    other.memory = 0;
  }
  FrameBuffer &operator=(FrameBuffer&& other)
  {
    this->texture_id       = other.texture_id;
    this->render_buffer_id = other.render_buffer_id;
    this->color_format     = other.color_format;
    this->width            = other.width;
    this->height           = other.height;
    this->target           = other.target;
    this->id               = other.id;
    this->memory           = other.memory;
    
    other.id     = 0;
    other.memory = 0;
    
    return *this;
  }
  
  ~FrameBuffer();
  
  void bind()   const override;
  void unbind() const override;

  void destroy();

  void rescale(int width, int height);
  void set_color_format(GLenum color_format);

};

} // namespace brenta
