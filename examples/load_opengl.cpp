// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Create a screen with OpenGL options.
 */

#include <brenta/engine.hpp>
#include <iostream>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;
  
const int screen_width = 800;
const int screen_height = 600;

int main()
{
  //
  // Setup
  //

  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug))
    .with(window::builder()
          .title("load opengl test")
          .width(screen_width)
          .height(screen_height))
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = engine::managed();
  
  //
  // Render loop
  //
  
  while (!window::should_close())
  {
    // Input
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    // Draw
    // If you see a black window, it means that the OpenGL options are not
    // set correctly.
    gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear();

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
