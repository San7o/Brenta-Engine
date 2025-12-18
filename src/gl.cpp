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

//
// Static variables
//

bool gl::enable_blending;
bool gl::enable_cull_face;
bool gl::enable_multisample;
bool gl::enable_depth_test;
const std::string gl::subsystem_name = "gl";

// Forward declaration
void APIENTRY glDebugOutput([[maybe_unused]] GLenum source,
                            [[maybe_unused]] GLenum type,
                            [[maybe_unused]] GLuint id,
                            [[maybe_unused]] GLenum severity,
                            [[maybe_unused]] GLsizei length,
                            [[maybe_unused]] const GLchar *message,
                            [[maybe_unused]] const void *userParam);

//
// Subsystem interface
//

std::expected<void, subsystem::error> gl::initialize()
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

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    // Enable all messages from all sources, of all types, of all severities.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
                          0, nullptr, GL_TRUE);
    INFO("GL_DEBUG_OUTPUT configured");
  }
  
  GLenum errcode = gl::check_error();
  if (!errcode)
    INFO("gl initialized");

  return {};
}

std::expected<void, subsystem::error> gl::terminate()
{
  INFO("gl terminated")
  return {};
}

std::string gl::name()
{
  return gl::subsystem_name;
}

//
// Member functions
//

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
// Utilities
//

void APIENTRY glDebugOutput([[maybe_unused]] GLenum source,
                            [[maybe_unused]] GLenum type,
                            [[maybe_unused]] GLuint id,
                            [[maybe_unused]] GLenum severity,
                            [[maybe_unused]] GLsizei length,
                            [[maybe_unused]] const GLchar *message,
                            [[maybe_unused]] const void *userParam)
{
  // ignore non-significant error/warning codes
  if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

  std::cout << "---------------" << std::endl;
  std::cout << "Debug message (" << id << "): " <<  message << std::endl;

  switch (source)
  {
  case GL_DEBUG_SOURCE_API:
    std::cout << "Source: API"; break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    std::cout << "Source: Window System"; break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    std::cout << "Source: Shader Compiler"; break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    std::cout << "Source: Third Party"; break;
  case GL_DEBUG_SOURCE_APPLICATION:
    std::cout << "Source: Application"; break;
  case GL_DEBUG_SOURCE_OTHER:
    std::cout << "Source: Other"; break;
  } std::cout << std::endl;

  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    std::cout << "Type: Error"; break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    std::cout << "Type: Deprecated Behaviour"; break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    std::cout << "Type: Undefined Behaviour"; break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    std::cout << "Type: Portability"; break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    std::cout << "Type: Performance"; break;
  case GL_DEBUG_TYPE_MARKER:
    std::cout << "Type: Marker"; break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    std::cout << "Type: Push Group"; break;
  case GL_DEBUG_TYPE_POP_GROUP:
    std::cout << "Type: Pop Group"; break;
  case GL_DEBUG_TYPE_OTHER:
    std::cout << "Type: Other"; break;
  } std::cout << std::endl;
    
  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    std::cout << "Severity: high"; break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    std::cout << "Severity: medium"; break;
  case GL_DEBUG_SEVERITY_LOW:
    std::cout << "Severity: low"; break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    std::cout << "Severity: notification"; break;
  } std::cout << std::endl;
  std::cout << std::endl;
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
