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

void gl::load_opengl(bool gl_blending, bool gl_cull_face, bool gl_multisample,
                     bool gl_depth_test)
{
  GLADloadproc loadproc = (GLADloadproc) window::instance().get_proc_address();
  if (!gladLoadGLLoader(loadproc))
  {
    ERROR("Failed to initialize GLAD");
    exit(-1);
  }

  int width = window::instance().get_width();
  int height = window::instance().get_height();

  glViewport(0, 0, width, height);

  if (gl_depth_test)
  {
    glEnable(GL_DEPTH_TEST);
    INFO("Enabled GL_DEPTH_TEST");
  }

  // Enable blending for transparency
  if (gl_blending)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    INFO("Enabled GL_BLEND (transparency)");
  }

  // Enable face culling, draw only visible triangles
  // based on their orientation (defined clockwise or counterclockwise)
  if (gl_cull_face)
  {
    glEnable(GL_CULL_FACE);
    INFO("Enabled GL_CULL_FACE (draw only visible triangles)");
  }

  // Enable multisampling
  // Only works for a multisample buffer. */
  if (gl_multisample)
  {
    glEnable(GL_MULTISAMPLE);
    INFO("Enabled GL_MULTISAMPLE");
  }

  GLenum errcode = gl::check_error();
  if (!errcode)
    INFO("OpenGl loaded");
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

void gl::clear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gl::bind_vertex_array(unsigned int n)
{
  glBindVertexArray(n);
}

GLenum gl::check_error_(const char *file, int line)
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
