// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/gl.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/text.hpp>

#include <sstream>

using namespace brenta;

//
// Static variables
//

const std::string gl::subsystem_name = "gl";
const gl::config gl::default_config = {
  false,
  false,
  false,
  false,
};
gl::config gl::init_config = default_config;
bool gl::initialized = false;

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
  if (this->is_initialized()) return {};
  
  GLADloadproc loadproc = (GLADloadproc) window::get_proc_address();
  if (!gladLoadGLLoader(loadproc))
  {
    ERROR("{}: failed to initialize GLAD", gl::subsystem_name);
    return std::unexpected("Failed to initialize GLAD");
  }

  int width = window::get_width();
  int height = window::get_height();

  glViewport(0, 0, width, height);

  if (gl::init_config.enable_depth_test)
  {
    glEnable(GL_DEPTH_TEST);
    INFO("{}: enabled GL_DEPTH_TEST", gl::subsystem_name);
  }

  if (gl::init_config.enable_blending)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    INFO("{}: enabled GL_BLEND (transparency)", gl::subsystem_name);
  }

  if (gl::init_config.enable_cull_face)
  {
    glEnable(GL_CULL_FACE);
    INFO("{}: enabled GL_CULL_FACE", gl::subsystem_name);
  }

  if (gl::init_config.enable_multisample)
  {
    glEnable(GL_MULTISAMPLE);
    INFO("{}: enabled GL_MULTISAMPLE", gl::subsystem_name);
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
    INFO("{}: configured GL_DEBUG_OUTPUT", gl::subsystem_name);
  }
  
  GLenum errcode = gl::check_error();
  if (errcode != GL_NO_ERROR)
  {
    return std::unexpected("GL error");
  }

  gl::initialized = true;
  INFO("{}: initialized", gl::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> gl::terminate()
{
  if (!this->is_initialized()) return {};

  gl::initialized = false;
  INFO("{}: terminated", gl::subsystem_name);
  return {};
}

std::string gl::name()
{
  return gl::subsystem_name;
}

bool gl::is_initialized()
{
  return gl::initialized;
}

//
// Member functions
//

void gl::set_poligon_mode(GLboolean enable)
{
  if (enable)
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    INFO("{}: enabled GL_POLYGON_MODE (wireframe)", gl::subsystem_name);
  }
  else
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    INFO("{}: disabled GL_POLYGON_MODE (fill)", gl::subsystem_name);
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
    std::string error = "";
    switch (errorCode)
    {
    case GL_INVALID_ENUM: error = "invalid enum"; break;
    case GL_INVALID_VALUE: error = "invalid value"; break;
    case GL_INVALID_OPERATION: error = "invalid operation"; break;
    case GL_STACK_OVERFLOW: error = "stack overflow"; break;
    case GL_STACK_UNDERFLOW: error = "stack underflow"; break;
    case GL_OUT_OF_MEMORY: error = "out of memory"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "invalid framebuffer operation"; break;
    default: error = "unknown error";  break;
    }

    error += " | " + std::string(file) + " (" + std::to_string(line) + ")";
    ERROR("{}: {}", gl::subsystem_name, error);
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

  std::stringstream out;
  out << "Debug message (" << id << "): " <<  message << std::endl;

  switch (source)
  {
  case GL_DEBUG_SOURCE_API:
    out << "Source: API"; break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    out << "Source: Window System"; break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    out << "Source: Shader Compiler"; break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    out << "Source: Third Party"; break;
  case GL_DEBUG_SOURCE_APPLICATION:
    out << "Source: Application"; break;
  case GL_DEBUG_SOURCE_OTHER:
    out << "Source: Other"; break;
  } out << std::endl;

  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    out << "Type: Error"; break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    out << "Type: Deprecated Behaviour"; break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    out << "Type: Undefined Behaviour"; break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    out << "Type: Portability"; break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    out << "Type: Performance"; break;
  case GL_DEBUG_TYPE_MARKER:
    out << "Type: Marker"; break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    out << "Type: Push Group"; break;
  case GL_DEBUG_TYPE_POP_GROUP:
    out << "Type: Pop Group"; break;
  case GL_DEBUG_TYPE_OTHER:
    out << "Type: Other"; break;
  } out << std::endl;
    
  switch (severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    out << "Severity: high"; break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    out << "Severity: medium"; break;
  case GL_DEBUG_SEVERITY_LOW:
    out << "Severity: low"; break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    out << "Severity: notification"; break;
  } out << std::endl;
  out << std::endl;

  ERROR("{}: {}", gl::subsystem_name, out.str());
}

//
// Builder
//

gl::builder &gl::builder::blending()
{
  this->conf.enable_blending = true;
  return *this;
}

gl::builder &gl::builder::cull_face()
{
  this->conf.enable_cull_face = true;
  return *this;
}

gl::builder &gl::builder::multisample()
{
  this->conf.enable_multisample = true;
  return *this;
}

gl::builder &gl::builder::depth_test()
{
  this->conf.enable_depth_test = true;
  return *this;
}

brenta::subsystem &gl::builder::build()
{
  gl::init_config = this->conf;
  return gl::instance();
}
