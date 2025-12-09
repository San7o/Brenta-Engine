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
protected:

  int width = 1280;
  int height = 720;
  GLFWwindow *window_backend = NULL;
  std::string title = "Brenta Engine";
  bool capture_mouse = false;
  bool msaa = false;
  bool vsync = false;
  
public:

  std::string subsystem_name = "window";
  
  window() = default;
  ~window() = default;

  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate() override;

  /**
   * @brief Get a static instance of the window
   */
  static brenta::window &instance();
  
  class builder;
  
  //
  // Getters
  //
  
  int get_width();
  int get_height();
  bool is_window_closed();
  bool is_key_pressed(int key);
  float get_time();
  GLFWwindow *get_window();  
  /**
   * @brief Get the OpenGL function pointer
   * @return OpenGL function pointer
   */
  GLFWglproc get_proc_address();

  //
  // Setters
  //

  void set_mouse_callback(GLFWcursorposfun callback);
  void set_size_callback(GLFWframebuffersizefun callback);
  void set_mouse_pos_callback(GLFWcursorposfun callback);
  void set_key_callback(GLFWkeyfun callback);
  void set_mouse_capture(bool is_captured);
  void set_close();

  //
  // Utils
  //

  /**
   * @brief Swap the front and back buffers
   *
   * Having two buffers is done to avoid flickering.
   */
  void swap_buffers();
  /**
   * @brief Poll all pending events
   */
  void poll_events();

private:
  void set_context_version(int major, int minor);
  void use_core_profile();
  void set_hints_apple();
  void create_window(int width, int height, std::string title);
  void make_context_current();
  static void framebuffer_size_callback(GLFWwindow *window_backend, int width,
                                        int height);
};

class window::builder : public subsystem::builder
{
private:
  
  window &_window;
  
public:

  builder() : _window(window::instance()) {}
  ~builder() = default;
  
  builder &width(int width);
  builder &height(int height);
  builder &title(std::string title);
  builder &capture_mouse();
  builder &msaa();
  builder &vsync();
  
  subsystem &build();
};
  
} // namespace brenta
