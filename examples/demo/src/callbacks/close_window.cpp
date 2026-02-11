// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/window.hpp>
#include <brenta/input.hpp>
#include <demo/callbacks/close_window.hpp>

using namespace brenta;

void init_close_window_callback()
{
  auto close_window_callback = []() { Window::close(); };
  Input::add_keyboard_callback(GLFW_KEY_ESCAPE, close_window_callback);
}
