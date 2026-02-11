// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Create a screen with OpenGL options.
 */

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

using namespace brenta;

int main()
{
  const int screen_width = 800;
  const int screen_height = 600;

  //
  // Setup
  //

  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("load opengl test")
          .width(screen_width)
          .height(screen_height))
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();
  
  //
  // Render loop
  //
  
  while (!Window::should_close())
  {
    // Input
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();

    // Draw
    // If you see a black window, it means that the OpenGL options are not
    // set correctly.
    Gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    Gl::clear();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
