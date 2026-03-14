// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/window.hpp>

#include <brenta/drivers/glfw.hpp>
#include <glad/glad.h>

#include <cstdio>

using namespace brenta;

//
// Static variables
//

tenno::shared_ptr<Window::Driver> Window::backend     = nullptr;
tenno::shared_ptr<FrameBuffer>    Window::framebuffer = nullptr;
const std::string             Window::subsystem_name  = "window";
Window::Config                Window::init_config     = {};
bool                          Window::initialized     = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Window::initialize()
{
  if (this->is_initialized()) return {};

  Window::backend = tenno::make_shared<GlfwDriver>();
  auto ret = Window::backend->initialize(Window::init_config);
  if (!ret)
  {
    ERROR("{}: Failed to initialize window", Window::subsystem_name);
    return ret;
  }

  Window::framebuffer = tenno::make_shared<FrameBuffer>();
  
  INFO("{}: initialized", Window::subsystem_name);
  Window::initialized = true;
  return {};
}

std::expected<void, Subsystem::Error> Window::terminate()
{
  if (!this->is_initialized()) return {};

  if (Window::backend)
    Window::backend->terminate();

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
  if (Window::backend)
    return Window::backend->should_close();
  return true;
}

bool Window::is_key_pressed(Key key)
{
  if (Window::backend)
    return Window::backend->is_key_pressed(key);
  return false;
}

Time Window::get_time()
{
  if (Window::backend)
    return Window::backend->get_time();
  return {};
}

Window::WindowHandle Window::get_window()
{
  if (Window::backend)
    return Window::backend->get_window();
  return nullptr;
}

Window::ProcHandle Window::get_proc_address()
{
  if (Window::backend)
    return Window::backend->get_proc_address();
  return nullptr;
}

int Window::get_width()
{
  if (Window::backend)
    return Window::backend->get_width();
  return 0;
}

int Window::get_height()
{
  if (Window::backend)
    return Window::backend->get_height();
  return 0;
}

tenno::shared_ptr<Window::Driver> Window::get_driver()
{
  return Window::backend;
}

void Window::update_dimensions()
{
  if (Window::backend)
    Window::backend->update_dimensions();
}

void Window::set_dimensions(int width, int height)
{
  if (Window::backend)
    Window::backend->set_dimensions(width, height);
  return;
}

void Window::set_mouse_callback(Window::Callback callback)
{
  if (Window::backend)
    Window::backend->set_mouse_callback(callback);
  return;
}

void Window::set_size_callback(Window::Callback callback)
{
  if (Window::backend)
    Window::backend->set_size_callback(callback);
  return;
}

void Window::set_mouse_capture(bool is_captured)
{
  if (Window::backend)
    Window::backend->set_mouse_capture(is_captured);
  return;
}

void Window::close()
{
  if (Window::backend)
    Window::backend->close();
  return;
}

void Window::swap_buffers()
{
  if (Window::backend)
    Window::backend->swap_buffers();
  return;
}

void Window::poll_events()
{
  if (Window::backend)
    Window::backend->poll_events();
  return;
}

void Window::set_key_callback(Window::Callback callback)
{
  if (Window::backend)
    Window::backend->set_key_callback(callback);
}

void Window::set_mouse_pos_callback(Window::Callback callback)
{
  if (Window::backend)
    Window::backend->set_mouse_pos_callback(callback);
}

void Window::make_context_current()
{
  if (Window::backend)
    Window::backend->make_context_current();
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
