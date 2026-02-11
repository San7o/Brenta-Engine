// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

// clang-format off
// The order of these includes is important
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <brenta/subsystem.hpp>
#include <brenta/time.hpp>

#include <string>

namespace brenta
{

/**
 * @brief Window subsystem
 *
 * This class is used to create a window and handle all the events
 * related to the window. This class provides methods to initialize
 * the window, get the window size, check if a key is pressed, get the
 * time since the start of the program, and more.
 */
class window : public subsystem
{
public:

  struct config;
  class builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;

  // Constructors destructors
  window() = default;
  ~window() = default;

  // Member functions
  
  // Get a static instance of the window
  static brenta::window &instance();
  
  //
  // Getters
  //
  
  static int         get_width();
  static int         get_height();
  static bool        should_close();
  static bool        is_key_pressed(int key);
  static Time        get_time();
  static GLFWwindow *get_window();  
  static GLFWglproc  get_proc_address();

  //
  // Setters
  //

  static void set_mouse_callback(GLFWcursorposfun callback);
  static void set_size_callback(GLFWframebuffersizefun callback);
  static void set_mouse_pos_callback(GLFWcursorposfun callback);
  static void set_key_callback(GLFWkeyfun callback);
  static void set_mouse_capture(bool is_captured);
  static void set_width_height(int width, int height);
  static void close();

  //
  // Utils
  //

  // Swap the front and back buffers
  // Having two buffers avoids flickering.
  static void swap_buffers();
  // Poll all pending events
  static void poll_events();

private:
  
  static const window::config default_config;
  static window::config       init_config;

  static int          width;
  static int          height;
  static GLFWwindow  *window_backend;
  static std::string  title;
  static bool         initialized;
  static Time         time;
  
  static void set_context_version(int major, int minor);
  static void use_core_profile();
  static void set_hints_apple();
  static void create_window(int width, int height, std::string title);
  static void make_context_current();
  static void framebuffer_size_callback(GLFWwindow *window_backend,
                                        int width, int height);
};

struct window::config
{
public:
  int width;
  int height;
  std::string title;
  bool capture_mouse;
  bool msaa;
  bool vsync;
  bool debug;
};
  
class window::builder : public subsystem::builder
{
private:

  window::config conf = window::default_config;
  
public:

  builder() = default;
  ~builder() = default;
  
  builder &width(int width);
  builder &height(int height);
  builder &title(const std::string &title);
  builder &capture_mouse();
  builder &msaa();
  builder &vsync();
  builder &debug(); // opengl debug errors, set this during development
  
  subsystem &build() override;
};
  
} // namespace brenta
