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
  int width;
  int height;

  FrameBuffer() {}
  FrameBuffer(int width, int height, GLenum format = GL_RGBA);
  ~FrameBuffer();
  
  void bind();
  void unbind();

  void destroy();

  void rescale(int width, int height);
  void set_color_format(GLenum color_format);
};

} // namespace brenta
