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

int window::width;
int window::height;
GLFWwindow *window::window_backend;
std::string window::title;
Time window::time;
const std::string window::subsystem_name = "window";
const window::config window::default_config = {
  800,
  600,
  "Brenta Engine",
  false,
  false,
  false,
  false,
};
window::config window::init_config = window::default_config;
bool window::initialized = false;

//
// Subsystem interface
//

std::expected<void, subsystem::error> window::initialize()
{
  if (this->is_initialized()) return {};
  
  if (glfwInit() == GLFW_FALSE)
  {
    return std::unexpected(window::subsystem_name +
                           ": failed to initialize GLFW");
  }

  set_context_version(3, 3);
  use_core_profile();

  if (window::init_config.msaa)
  {
    glfwWindowHint(GLFW_SAMPLES, 4);
    INFO("{}: enabled MSAA", window::subsystem_name);
  } else {
    INFO("{}: disabled MSAA", window::subsystem_name);
  }

  if (!window::init_config.vsync)
  {
    glfwSwapInterval(0);
    INFO("{}: disabled VSync", window::subsystem_name);
  } else {
    INFO("{}: enabled VSync", window::subsystem_name);
  }

  if (window::init_config.debug)
  {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    INFO("{}: enabled OPENGL_DEBUG_CONTEXT", window::subsystem_name);
  }

#ifdef __APPLE__
  set_hints_apple();
#endif

  create_window(window::init_config.width,
                window::init_config.height, title);
  make_context_current();
  set_mouse_capture(window::init_config.capture_mouse);

  set_size_callback(framebuffer_size_callback);

  window::initialized = true;
  INFO("{}: initialized", window::subsystem_name)
  return {};
}

std::expected<void, subsystem::error> window::terminate()
{
  if (!this->is_initialized()) return {};
  
  glfwDestroyWindow(window::window_backend);
  glfwTerminate();

  window::initialized = false;
  INFO("{}: terminated", window::subsystem_name);
  return {};
}

std::string window::name()
{
  return window::subsystem_name;
}

bool window::is_initialized()
{
  return window::initialized;
}

//
// Member functions
//

window &window::instance()
{
  static window _instance;
  return _instance;
}

bool window::should_close()
{
  return glfwWindowShouldClose(window::window_backend);
}

void window::set_width_height(int width, int height)
{
  window::width = width;
  window::height = height;
  return;
}

bool window::is_key_pressed(int key)
{
  return glfwGetKey(window::window_backend, key) == GLFW_PRESS;
}

Time window::get_time()
{
  return window::time;
}

GLFWwindow *window::get_window()
{
  return window::window_backend;
}

GLFWglproc window::get_proc_address()
{
  return reinterpret_cast<void (*)()>(glfwGetProcAddress);
}

int window::get_width()
{
  return window::width;
}

int window::get_height()
{
  return window::height;
}

void window::set_mouse_callback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(window::window_backend, callback);
}

void window::set_size_callback(GLFWframebuffersizefun callback)
{
  glfwSetFramebufferSizeCallback(window::window_backend, callback);

  DEBUG("{}: set framebuffer size callback", window::subsystem_name);
}

void window::set_mouse_capture(bool is_captured)
{
  if (is_captured)
  {
    glfwSetInputMode(window::window_backend, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("{}: mouse capture enabled", window::subsystem_name);
  }
  else
  {
    glfwSetInputMode(window::window_backend, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    INFO("{}: mouse capture disabled", window::subsystem_name);
  }
}

void window::close()
{
  glfwSetWindowShouldClose(window::window_backend, GLFW_TRUE);
}

void window::swap_buffers()
{
  window::time.update(glfwGetTime());
  glfwSwapBuffers(window::window_backend);
}

void window::poll_events()
{
  glfwPollEvents();
}

void window::set_context_version(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

  INFO("{}: set context to OpenGL version: {}.{}",
       window::subsystem_name, major, minor);
}

void window::set_key_callback(GLFWkeyfun callback)
{
  glfwSetKeyCallback(window::window_backend, callback);
}

void window::set_mouse_pos_callback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(window::get_window(), callback);
}

void window::use_core_profile()
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  INFO("{}: set OpenGL profile to core", window::subsystem_name);
}

void window::set_hints_apple()
{
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void window::create_window(int width, int height, std::string title)
{
  window::window_backend = glfwCreateWindow(width, height, title.c_str(),
                                            NULL, NULL);
  if (window::window_backend == NULL)
  {
    ERROR("{}: failed to create GLFW window", window::subsystem_name);
    window::instance().terminate();
  }
  window::width = width;
  window::height = height;
  window::title = title;
}

void window::make_context_current()
{
  glfwMakeContextCurrent(window::window_backend);
}

void window::framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                       [[maybe_unused]] int width,
                                       [[maybe_unused]] int height)
{
#ifndef BRENTA_USE_IMGUI
  glViewport(0, 0, width, height);
  window::width = width;
  window::height = height;
#endif
}

//
// Builder
//

window::builder &window::builder::width(int width)
{
  this->conf.width = width;
  return *this;
}

window::builder &window::builder::height(int height)
{
  this->conf.height = height;
  return *this;
}

window::builder &window::builder::title(const std::string &title)
{
  this->conf.title = title;
  return *this;
}

window::builder &window::builder::capture_mouse()
{
  this->conf.capture_mouse = true;
  return *this;
}

window::builder &window::builder::msaa()
{
  this->conf.msaa = true;
  return *this;
}

window::builder &window::builder::debug()
{
  this->conf.debug = true;
  return *this;
}

window::builder &window::builder::vsync()
{
  this->conf.vsync = true;
  return *this;
}

subsystem &window::builder::build()
{
  window::init_config = this->conf;
  return window::instance();
}
