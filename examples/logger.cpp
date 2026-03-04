// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/opengl/gl.hpp>

using namespace brenta;

int main()
{  
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("logger test")
          .width(800)
          .height(600))
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();
  
  INFO("Hello, World!");
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
