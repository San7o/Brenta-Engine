// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/gl.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/text.hpp>
#include <iostream>

using namespace brenta;

bool gl::enable_blending;
bool gl::enable_cull_face;
bool gl::enable_multisample;
bool gl::enable_depth_test;

std::expected<void, std::string> gl::initialize()
{
  GLADloadproc loadproc = (GLADloadproc) window::get_proc_address();
  if (!gladLoadGLLoader(loadproc))
  {
    ERROR("Failed to initialize GLAD");
    return std::unexpected(this->subsystem_name);
  }

  int width = window::get_width();
  int height = window::get_height();

  glViewport(0, 0, width, height);

  if (enable_depth_test)
  {
    glEnable(GL_DEPTH_TEST);
    INFO("GL_DEPTH_TEST enabled");
  }

  if (enable_blending)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    INFO("GL_BLEND enabled (transparency)");
  }

  if (enable_cull_face)
  {
    glEnable(GL_CULL_FACE);
    INFO("GL_CULL_FACE enabled (draw only visible triangles)");
  }

  if (enable_multisample)
  {
    glEnable(GL_MULTISAMPLE);
    INFO("GL_MULTISAMPLE enabled");
  }

  GLenum errcode = gl::check_error();
  if (!errcode)
    INFO("gl initialized");

  return {};
}

std::expected<void, std::string> gl::terminate()
{
  INFO("gl terminated")
  return {};
}

void gl::set_poligon_mode(GLboolean enable)
{
  if (enable)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    INFO("Enabled GL_POLYGON_MODE (wireframe)");
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    INFO("Disabled GL_POLYGON_MODE (fill)");
  }
}

gl &gl::instance()
{
  static gl _gl;
  return _gl;
}

void gl::set_viewport(int x, int y, int width, int height)
{
  glViewport(x, y, width, height);
}

void gl::set_color(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
}

void gl::draw_arrays(GLenum mode, int first, int count)
{
  glDrawArrays(mode, first, count);
}

void gl::draw_elements(GLenum mode, int count, GLenum type, const void *indices)
{
  glDrawElements(mode, count, type, indices);
}

void gl::bind_vertex_array(unsigned int n)
{
  glBindVertexArray(n);
}

void gl::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLenum gl::_check_error(const char *file, int line)
{
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    std::string error;
    switch (errorCode)
    {
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      error = "STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      error = "STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    default:
      error = "UNKNOWN";
      break;
    }

    error += " | " + std::string(file) + " (" + std::to_string(line) + ")";
    ERROR(error);
  }
  return errorCode;
}

//
// Builder
//

gl::builder &gl::builder::blending()
{
  this->enable_blending = true;
  return *this;
}

gl::builder &gl::builder::cull_face()
{
  this->enable_cull_face = true;
  return *this;
}

gl::builder &gl::builder::multisample()
{
  this->enable_multisample = true;
  return *this;
}

gl::builder &gl::builder::depth_test()
{
  this->enable_depth_test = true;
  return *this;
}

brenta::subsystem &gl::builder::build()
{
  gl::enable_blending = this->enable_blending;
  gl::enable_cull_face = this->enable_cull_face;
  gl::enable_multisample = this->enable_multisample;
  gl::enable_depth_test = this->enable_depth_test;
  return gl::instance();
}
