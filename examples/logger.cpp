// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

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
                 .use_screen(true)
                 .set_screen_width(SCR_WIDTH)
                 .set_screen_height(SCR_HEIGHT)
                 .set_screen_is_mouse_captured(false)
                 .use_logger(true)
                 .build();

  INFO("Hello, World!");

  while (!screen::is_window_closed())
  {
    if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
      screen::set_close();

    screen::poll_events();
    screen::swap_buffers();
  }

  return 0;
}
