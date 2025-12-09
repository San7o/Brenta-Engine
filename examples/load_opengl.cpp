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

int main()
{
  //
  // Setup
  //
  
  const int screen_width = 800;
  const int screen_height = 600;

  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug))
    .subsystem(window::builder()
               .title("load opengl test")
               .width(screen_width)
               .height(screen_height))
    .subsystem(gl::builder()
               .blending()
               .cull_face()
               .multisample()
               .depth_test())
    .build();
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }
  
  //
  // Render loop
  //
  
  while (!window::should_close())
  {
    /* Input */
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    /* Clear */
    // If you see a black window, it means that the OpenGL options are not
    // set correctly.
    gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear();

    window::poll_events();
    window::swap_buffers();
  }
 
  //
  // Cleanup
  //
  
  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}
