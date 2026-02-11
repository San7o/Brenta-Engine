// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/frame_buffer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>

#include <stdexcept>

using namespace brenta;

FrameBuffer::FrameBuffer(int width, int height, GLenum color_format)
{
  this->color_format = color_format;

  glGenFramebuffers(1, &this->id);
  if (this->id == 0)
  {
    ERROR("framebuffer: error generating framebuffer");
    throw std::runtime_error("framebuffer: error generating framebuffer");
  }
  glBindFramebuffer(GL_FRAMEBUFFER, this->id);
  check_error();

  glGenTextures(1, &this->texture_id);
  if (this->texture_id == 0)
  {
    ERROR("framebuffer: error generating texture");
    exit(1);
  }
  glBindTexture(GL_TEXTURE_2D, this->texture_id);
  check_error();

  glTexImage2D(GL_TEXTURE_2D, 0, this->color_format, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         this->texture_id, 0);

  glGenRenderbuffers(1, &this->render_buffer_id);
  glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer_id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, this->render_buffer_id);
  check_error();

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    ERROR("framebuffer: not complete");
    exit(1);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  return;
}

FrameBuffer::~FrameBuffer()
{
  glDeleteFramebuffers(1, &this->id);
  glDeleteTextures(1, &this->texture_id);
  return;
}

void FrameBuffer::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->id);
  check_error();
  return;
}

void FrameBuffer::unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  check_error();
  return;
}

void FrameBuffer::destroy()
{
  glDeleteFramebuffers(1, &this->id);
  glDeleteTextures(1, &this->texture_id);
  return;
}

void FrameBuffer::rescale(int width, int height)
{
  glBindFramebuffer(GL_FRAMEBUFFER, this->id);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    ERROR("framebuffer: not complete");
    return;
  }

  glBindTexture(GL_TEXTURE_2D, this->texture_id);
  check_error();

  glTexImage2D(GL_TEXTURE_2D, 0, this->color_format, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         this->texture_id, 0);
  glBindTexture(GL_TEXTURE_2D, 0);
  check_error();

  glBindRenderbuffer(GL_RENDERBUFFER, this->render_buffer_id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, this->render_buffer_id);

  Window::set_width_height(width, height);
  return;
}

void FrameBuffer::set_color_format(GLenum color_format)
{
  this->color_format = color_format;
  return;
}
