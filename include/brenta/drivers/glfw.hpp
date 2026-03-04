// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/window.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <GLFW/glfw3.h>

namespace brenta
{

//
// Glfw Driver for Window
// ----------------------
//
class GlfwDriver : public Window::Driver
{
public:

  GlfwDriver()  = default;
  ~GlfwDriver() = default;

  std::expected<void, std::string> initialize(const Window::Config &conf) override;
  std::expected<void, std::string> terminate() override;
  
  //
  // Getters
  //
  
  int                  get_width()                override;
  int                  get_height()               override;
  bool                 should_close()             override;
  bool                 is_key_pressed(Key key)    override;
  Time                 get_time()                 override;
  Window::ProcHandle   get_proc_address()         override;
  Window::WindowHandle get_window()               override;  

  //
  // Setters
  //

  void set_mouse_capture(bool is_captured)        override;
  void set_width_height(int width, int height)    override;
  void set_mouse_callback(void* callback)         override;
  void set_size_callback(void* callback)          override;
  void set_mouse_pos_callback(void* callback)     override;
  void set_key_callback(void* callback)           override;
  void set_dimensions(int width, int height)      override;

  //
  // Utils
  //

  void swap_buffers()                             override;
  void poll_events()                              override;
  void update_dimensions()                        override;
  void set_context_version(int major, int minor)  override;
  void use_core_profile()                         override;
  void set_hints_apple()                          override;
  void make_context_current()                     override;
  void close()                                    override;
  void create_window(int width, int height,
                     const std::string &title)    override;
  
private:
  
  GLFWwindow  *window;
  int          width  = 800;
  int          height = 600;
  Time         time;
  
  static void glfw_key_callback([[maybe_unused]] GLFWwindow *window,
                                int glfw_key,
                                [[maybe_unused]] int scancode,
                                int glfw_action,
                                int glfw_mods);

  static void framebuffer_size_callback(GLFWwindow *window_backend,
                                        int width, int height);
  static void mouse_callback([[maybe_unused]] GLFWwindow *window,
                             double xpos,
                             double ypos);
  static int key_to_glfw(Key key);
  static Key glfw_to_key(int glfw_key);

};

  
} // namespace brenta
