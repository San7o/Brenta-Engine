// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <demo/callbacks/close_window_callback.hpp>

#include <brenta/engine.hpp>

using namespace brenta;

void init_close_window_callback()
{
  auto close_window_callback = []() { screen::set_close(); };
  input::add_keyboard_callback(GLFW_KEY_ESCAPE, close_window_callback);
}
