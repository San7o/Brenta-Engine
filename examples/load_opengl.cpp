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

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

int main()
{
  engine eng = engine::builder()
                 /* Screen options */
                 .use_screen(true)
                 .set_screen_width(SCR_WIDTH)
                 .set_screen_height(SCR_HEIGHT)
                 .set_screen_is_mouse_captured(false)
                 /* OpenGL options */
                 .set_gl_blending(true)
                 .set_gl_cull_face(true)
                 .set_gl_multisample(true)
                 .set_gl_depth_test(true)
                 .build();

  while (!screen::is_window_closed())
  {
    /* Input */
    if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
      screen::set_close();

    /* Clear */
    // If you see a black screen, it means that the OpenGL options are not
    // set correctly.
    gl::set_color(0.2f, 0.3f, 0.3f, 1.0f);
    gl::clear();

    screen::poll_events();
    screen::swap_buffers();
  }

  return 0;
}
