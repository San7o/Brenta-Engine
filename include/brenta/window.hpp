// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>
#include <brenta/time.hpp>
#include <brenta/key.hpp>

#include <string>
#include <memory>

namespace brenta
{

class WindowDriver;
  
/**
 * @brief Window subsystem
 *
 * This class is used to create a window and handle all the events
 * related to the window. This class provides methods to initialize
 * the window, get the window size, check if a key is pressed, get the
 * time since the start of the program, and more.
 */
class Window : public Subsystem
{
public:

  struct Config;
  class  Builder;
  
  using WindowHandle = void*;
  using ProcHandle   = void*;
  
  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Constructors destructors
  Window() = default;
  ~Window() = default;

  // Member functions
  
  // Get a static instance of the window
  static brenta::Window &instance();
  
  //
  // Getters
  //
  
  static int          get_width();
  static int          get_height();
  static bool         should_close();
  static bool         is_key_pressed(Key key);
  static Time         get_time();
  static ProcHandle   get_proc_address();
  static WindowHandle get_window();
  static std::shared_ptr<WindowDriver> get_driver();

  //
  // Setters
  //

  static void set_mouse_capture(bool is_captured);
  static void set_width_height(int width, int height);
  
  static void set_mouse_callback(void* callback);
  static void set_size_callback(void* callback);
  static void set_mouse_pos_callback(void* callback);
  static void set_key_callback(void* callback);

  //
  // Utils
  //

  // Swap the front and back buffers
  // Having two buffers avoids flickering.
  static void swap_buffers();
  // Poll all pending events
  static void poll_events();
  static void update_dimensions();
  static void update_dimensions(int width, int height);
  static void make_context_current();
  static void close();

protected:
  
  static Window::Config       init_config;

  static bool                 initialized;
  static std::shared_ptr<WindowDriver> backend;
};

class WindowDriver
{
public:

  WindowDriver() = default;
  virtual ~WindowDriver() = default;

  virtual std::expected<void, std::string>
  initialize(const Window::Config &conf) = 0;
  virtual std::expected<void, std::string> terminate() = 0;
  
  //
  // Getters
  //
  
  virtual int          get_width() = 0;
  virtual int          get_height() = 0;
  virtual bool         should_close() = 0;
  virtual bool         is_key_pressed(Key key) = 0;
  virtual Time         get_time() = 0;
  virtual Window::ProcHandle   get_proc_address() = 0;
  virtual Window::WindowHandle get_window() = 0;  

  //
  // Setters
  //

  virtual void set_mouse_capture(bool is_captured) = 0;
  virtual void set_width_height(int width, int height) = 0;
  
  virtual void set_mouse_callback(void* callback) = 0;
  virtual void set_size_callback(void* callback) = 0;
  virtual void set_mouse_pos_callback(void* callback) = 0;
  virtual void set_key_callback(void* callback) = 0;


  //
  // Utils
  //

  // Swap the front and back buffers
  // Having two buffers avoids flickering.
  virtual void swap_buffers() = 0;
  virtual void poll_events() = 0;
  virtual void update_dimensions() = 0;
  virtual void update_dimensions(int width, int height) = 0;
  virtual void set_context_version(int major, int minor) = 0;
  virtual void use_core_profile() = 0;
  virtual void set_hints_apple() = 0;
  virtual void create_window(int width, int height,
                             const std::string& title) = 0;
  virtual void make_context_current() = 0;
  virtual void close() = 0;

};
  
struct Window::Config
{
public:
  int         width         = 800;
  int         height        = 600;
  std::string title         = "Brenta Engine";
  bool        capture_mouse = false;
  bool        msaa          = false;
  bool        vsync         = false;
  bool        debug         = false;
};
  
class Window::Builder : public Subsystem::Builder
{
private:

  Window::Config conf = {};
  
public:

  Builder() = default;
  ~Builder() = default;
  
  Builder &width(int width);
  Builder &height(int height);
  Builder &title(const std::string &title);
  Builder &capture_mouse();
  Builder &msaa();
  Builder &vsync();
  Builder &debug(); // opengl debug errors, set this during development
  
  Subsystem &build() override;
};
  
} // namespace brenta
