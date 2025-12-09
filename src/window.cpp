// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/audio.hpp>
#include <brenta/camera.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <cstdio>

using namespace brenta;

int window::width;
int window::height;
GLFWwindow *window::window_backend;
std::string window::title;
bool window::capture_mouse;
bool window::msaa;
bool window::vsync;

std::expected<void, std::string> window::initialize()
{
  if (glfwInit() == GLFW_FALSE)
  {
    return std::unexpected("window: Failed to initialize GLFW on init");
  }

  set_context_version(3, 3);
  use_core_profile();

  if (this->msaa)
  {
    glfwWindowHint(GLFW_SAMPLES, 4);
    INFO("MSAA is enabled");
  } else {
    INFO("MSAA is disabled");
  }

  if (!this->vsync)
  {
    glfwSwapInterval(0);
    INFO("VSync is disabled");
  } else {
    INFO("VSync is enabled");
  }

#ifdef __APPLE__
  set_hints_apple();
#endif

  create_window(this->width, this->height, title);
  make_context_current();
  set_mouse_capture(this->capture_mouse);

  set_size_callback(framebuffer_size_callback);

  INFO("window initialized")
  return {};
}

std::expected<void, std::string> window::terminate()
{
  INFO("terminating window");
  glfwDestroyWindow(this->window_backend);
  glfwTerminate();
  INFO("window terminated");

  return {};
}

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

float window::get_time()
{
  return glfwGetTime();
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

  INFO("set framebuffer size callback");
}

void window::set_mouse_capture(bool is_captured)
{
  window::capture_mouse = is_captured;
  if (is_captured)
  {
    glfwSetInputMode(window::window_backend, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("mouse capture enabled");
  }
  else
  {
    glfwSetInputMode(window::window_backend, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    INFO("mouse capture disabled");
  }
}

void window::close()
{
  glfwSetWindowShouldClose(window::window_backend, GLFW_TRUE);
}

void window::swap_buffers()
{
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

  INFO("set context to OpenGL version: {}.{}", major, minor);
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

  INFO("set OpenGL profile to core");
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
    ERROR("Failed to create GLFW window");
    window::instance().terminate();
  }
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
  this->_width = width;
  return *this;
}

window::builder &window::builder::height(int height)
{
  this->_height = height;
  return *this;
}

window::builder &window::builder::title(std::string title)
{
  this->_title = title;
  return *this;
}

window::builder &window::builder::capture_mouse()
{
  this->_capture_mouse = true;
  return *this;
}

window::builder &window::builder::msaa()
{
  this->_msaa = true;
  return *this;
}

window::builder &window::builder::vsync()
{
  this->_vsync = true;
  return *this;
}

subsystem &window::builder::build()
{
  window::width = this->_width;
  window::height = this->_height;
  window::title = this->_title;
  window::capture_mouse = this->_capture_mouse;
  window::msaa = this->_msaa;
  window::vsync = this->_vsync;
  return window::instance();
}
