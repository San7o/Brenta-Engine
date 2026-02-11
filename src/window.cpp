// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <cstdio>

#include <glad/glad.h>

using namespace brenta;

//
// Static variables
//

int                Window::width;
int                Window::height;
GLFWwindow        *Window::window_backend;
std::string        Window::title;
Time               Window::time;
const std::string  Window::subsystem_name = "window";
const Window::Config Window::default_config = {
  800,
  600,
  "Brenta Engine",
  false,
  false,
  false,
  false,
};
Window::Config Window::init_config = Window::default_config;
bool Window::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Window::initialize()
{
  if (this->is_initialized()) return {};
  
  if (glfwInit() == GLFW_FALSE)
  {
    return std::unexpected(Window::subsystem_name +
                           ": failed to initialize GLFW");
  }

  set_context_version(3, 3);
  use_core_profile();

  if (Window::init_config.msaa)
  {
    glfwWindowHint(GLFW_SAMPLES, 4);
    INFO("{}: enabled MSAA", Window::subsystem_name);
  } else {
    INFO("{}: disabled MSAA", Window::subsystem_name);
  }

  if (!Window::init_config.vsync)
  {
    glfwSwapInterval(0);
    INFO("{}: disabled VSync", Window::subsystem_name);
  } else {
    INFO("{}: enabled VSync", Window::subsystem_name);
  }

  if (Window::init_config.debug)
  {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    INFO("{}: enabled OPENGL_DEBUG_CONTEXT", Window::subsystem_name);
  }

#ifdef __APPLE__
  set_hints_apple();
#endif

  create_window(Window::init_config.width,
                Window::init_config.height, title);
  make_context_current();
  set_mouse_capture(Window::init_config.capture_mouse);

  set_size_callback(framebuffer_size_callback);

  Window::initialized = true;
  INFO("{}: initialized", Window::subsystem_name)
  return {};
}

std::expected<void, Subsystem::Error> Window::terminate()
{
  if (!this->is_initialized()) return {};
  
  glfwDestroyWindow(Window::window_backend);
  glfwTerminate();

  Window::initialized = false;
  INFO("{}: terminated", Window::subsystem_name);
  return {};
}

std::string Window::name()
{
  return Window::subsystem_name;
}

bool Window::is_initialized()
{
  return Window::initialized;
}

//
// Member functions
//

Window &Window::instance()
{
  static Window _instance;
  return _instance;
}

bool Window::should_close()
{
  return glfwWindowShouldClose(Window::window_backend);
}

void Window::set_width_height(int width, int height)
{
  Window::width = width;
  Window::height = height;
  return;
}

bool Window::is_key_pressed(int key)
{
  return glfwGetKey(Window::window_backend, key) == GLFW_PRESS;
}

Time Window::get_time()
{
  return Window::time;
}

GLFWwindow *Window::get_window()
{
  return Window::window_backend;
}

GLFWglproc Window::get_proc_address()
{
  return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

int Window::get_width()
{
  return Window::width;
}

int Window::get_height()
{
  return Window::height;
}

void Window::set_mouse_callback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(Window::window_backend, callback);
  return;
}

void Window::set_size_callback(GLFWframebuffersizefun callback)
{
  glfwSetFramebufferSizeCallback(Window::window_backend, callback);

  DEBUG("{}: set framebuffer size callback", Window::subsystem_name);
  return;
}

void Window::set_mouse_capture(bool is_captured)
{
  if (is_captured)
  {
    glfwSetInputMode(Window::window_backend, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("{}: mouse capture enabled", Window::subsystem_name);
  }
  else
  {
    glfwSetInputMode(Window::window_backend, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    INFO("{}: mouse capture disabled", Window::subsystem_name);
  }
  return;
}

void Window::close()
{
  glfwSetWindowShouldClose(Window::window_backend, GLFW_TRUE);
  return;
}

void Window::swap_buffers()
{
  Window::time.update(glfwGetTime());
  glfwSwapBuffers(Window::window_backend);
  return;
}

void Window::poll_events()
{
  glfwPollEvents();
  return;
}

void Window::set_context_version(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

  INFO("{}: set context to OpenGL version: {}.{}",
       Window::subsystem_name, major, minor);
}

void Window::set_key_callback(GLFWkeyfun callback)
{
  glfwSetKeyCallback(Window::window_backend, callback);
  return;
}

void Window::set_mouse_pos_callback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(Window::get_window(), callback);
  return;
}

void Window::use_core_profile()
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  INFO("{}: set OpenGL profile to core", Window::subsystem_name);
  return;
}

void Window::set_hints_apple()
{
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  return;
}

void Window::create_window(int width, int height, std::string title)
{
  Window::window_backend = glfwCreateWindow(width, height, title.c_str(),
                                            NULL, NULL);
  if (Window::window_backend == NULL)
  {
    ERROR("{}: failed to create GLFW window", Window::subsystem_name);
    Window::instance().terminate();
  }
  Window::width = width;
  Window::height = height;
  Window::title = title;
  return;
}

void Window::make_context_current()
{
  glfwMakeContextCurrent(Window::window_backend);
  return;
}

void Window::framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                       [[maybe_unused]] int width,
                                       [[maybe_unused]] int height)
{
#ifndef BRENTA_USE_IMGUI
  glViewport(0, 0, width, height);
  Window::width = width;
  Window::height = height;
#endif
  return;
}

//
// Builder
//

Window::Builder &Window::Builder::width(int width)
{
  this->conf.width = width;
  return *this;
}

Window::Builder &Window::Builder::height(int height)
{
  this->conf.height = height;
  return *this;
}

Window::Builder &Window::Builder::title(const std::string &title)
{
  this->conf.title = title;
  return *this;
}

Window::Builder &Window::Builder::capture_mouse()
{
  this->conf.capture_mouse = true;
  return *this;
}

Window::Builder &Window::Builder::msaa()
{
  this->conf.msaa = true;
  return *this;
}

Window::Builder &Window::Builder::debug()
{
  this->conf.debug = true;
  return *this;
}

Window::Builder &Window::Builder::vsync()
{
  this->conf.vsync = true;
  return *this;
}

Subsystem &Window::Builder::build()
{
  Window::init_config = this->conf;
  return Window::instance();
}
