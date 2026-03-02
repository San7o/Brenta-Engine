// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/text.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <tenno/memory.hpp>

using namespace brenta;

int main()
{
  const int SCR_WIDTH = 800;
  const int SCR_HEIGHT = 600;

  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug)
          .event(Logger::Event::Lifetime)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("text demo")
          .width(800)
          .height(600)
          .vsync())
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();

  //
  // Render loop
  //

  auto font = Font("examples/assets/fonts/arial.ttf", 100);
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    Text::render("Hello OpenGL!",
                 25.0f, 25.0f, 1.0f,
                 Color::yellow(), font);

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
