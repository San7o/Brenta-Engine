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

  return {};
}

std::expected<void, std::string> window::terminate()
{
  INFO("Terminating window");
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

bool window::is_window_closed()
{
  return glfwWindowShouldClose(this->window_backend);
}

bool window::is_key_pressed(int key)
{
  return glfwGetKey(this->window_backend, key) == GLFW_PRESS;
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
  glfwSetCursorPosCallback(this->window_backend, callback);
}

void window::set_size_callback(GLFWframebuffersizefun callback)
{
  glfwSetFramebufferSizeCallback(this->window_backend, callback);

  INFO("Set framebuffer size callback");
}

void window::set_mouse_capture(bool is_captured)
{
  if (is_captured)
  {
    glfwSetInputMode(this->window_backend, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    INFO("Mouse captured");
  }
  else
  {
    glfwSetInputMode(this->window_backend, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    INFO("Mouse not captured");
  }
}

void window::set_close()
{
  glfwSetWindowShouldClose(this->window_backend, GLFW_TRUE);
}

void window::swap_buffers()
{
  glfwSwapBuffers(this->window_backend);
}

void window::poll_events()
{
  glfwPollEvents();
}

void window::set_context_version(int major, int minor)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);

  INFO("Set context to OpenGL version: {}.{}", major, minor);
}

void window::set_key_callback(GLFWkeyfun callback)
{
  glfwSetKeyCallback(this->window_backend, callback);
}

void window::set_mouse_pos_callback(GLFWcursorposfun callback)
{
  glfwSetCursorPosCallback(window::get_window(), callback);
}

void window::use_core_profile()
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  INFO("Set OpenGL profile to core");
}

void window::set_hints_apple()
{
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void window::create_window(int width, int height, std::string title)
{
  this->window_backend = glfwCreateWindow(width, height, title.c_str(),
                                          NULL, NULL);
  if (this->window_backend == NULL)
  {
    ERROR("Failed to create GLFW window");
    terminate();
  }
}

void window::make_context_current()
{
  glfwMakeContextCurrent(this->window_backend);
}

void window::framebuffer_size_callback([[maybe_unused]] GLFWwindow *window,
                                       [[maybe_unused]] int width,
                                       [[maybe_unused]] int height)
{
#ifndef BRENTA_USE_IMGUI
  glViewport(0, 0, width, height);
  this->width = width;
  this->height = height;
#endif
}

//
// Builder
//

window::builder &window::builder::width(int width)
{
  this->_window.width = width;
  return *this;
}

window::builder &window::builder::height(int height)
{
  this->_window.height = height;
  return *this;
}

window::builder &window::builder::title(std::string title)
{
  this->_window.title = title;
  return *this;
}

window::builder &window::builder::capture_mouse()
{
  this->_window.capture_mouse = true;
  return *this;
}

window::builder &window::builder::msaa()
{
  this->_window.msaa = true;
  return *this;
}

window::builder &window::builder::vsync()
{
  this->_window.vsync = true;
  return *this;
}

subsystem &window::builder::build()
{
  return this->_window;
}
