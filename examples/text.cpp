// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/gui/text.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <iostream>

using namespace brenta;

int main()
{
  const int SCR_WIDTH = 800;
  const int SCR_HEIGHT = 600;

  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("text demo")
          .width(800)
          .height(600)
          .vsync())
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(Text::Builder()
          .font("examples/assets/fonts/arial.ttf")
          .size(100))
    .build();
  auto engine = Engine::managed();

  //
  // Render loop
  //
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    Text::render_text("Hello OpenGL!", 25.0f, 25.0f, 1.0f,
                      glm::vec3(0.5f, 0.8f, 0.2));

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
