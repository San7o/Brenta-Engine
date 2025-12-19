// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

/**
 * Just a simple window
 */

#include <brenta/engine.hpp>
#include <iostream>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(window::builder()
          .title("text demo")
          .width(800)
          .height(600)
          .vsync())
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(text::builder()
          .font("examples/assets/fonts/arial.ttf")
          .size(100))
    .build();
  auto engine = engine::managed();

  //
  // Render loop
  //
  
  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear();

    text::render_text("Hello OpenGL!", 25.0f, 25.0f, 1.0f,
                      glm::vec3(0.5f, 0.8f, 0.2));

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
