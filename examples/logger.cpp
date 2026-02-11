// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
          .title("logger test")
          .width(screen_width)
          .height(screen_height))
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();
  
  INFO("Hello, World!");
  
  //
  // Render loop
  // 

  while (!Window::should_close())
  {
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
