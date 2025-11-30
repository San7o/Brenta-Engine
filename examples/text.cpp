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
  engine eng =
    engine::builder()
      .use_screen(true)
      .use_text(true)                                   // Enable text rendering
      .set_text_font("examples/assets/fonts/arial.ttf") // Set the font
      .set_text_size(48)                                // Set the font size
      .set_gl_blending(true)
      .set_gl_cull_face(true)
      .set_gl_multisample(true)
      .set_gl_depth_test(true)
      .set_screen_width(SCR_WIDTH)
      .set_screen_height(SCR_HEIGHT)
      .set_screen_is_mouse_captured(false)
      .build();

  while (!screen::is_window_closed())
  {
    if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
      screen::set_close();

    gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear();

    text::render_text("Hello OpenGL!", 25.0f, 25.0f, 1.0f,
                      glm::vec3(0.5f, 0.8f, 0.2));

    screen::poll_events();
    screen::swap_buffers();
  }

  return 0;
}
