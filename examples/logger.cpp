// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/opengl/gl.hpp>

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
          .title("logger test")
          .width(screen_width)
          .height(screen_height))
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = engine::managed();
  
  INFO("Hello, World!");
  
  //
  // Render loop
  // 

  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
