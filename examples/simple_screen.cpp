// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Just a simple window
//

#include <brenta/engine.hpp>
#include <brenta/window.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

using namespace brenta;

int main()
{
  const int SCR_WIDTH = 800;
  const int SCR_HEIGHT = 600;

  Engine::Builder()
    .with(Window::Builder()
          .title("simple screen")
          .width(SCR_WIDTH)
          .height(SCR_HEIGHT))
    .build();
  auto engine = Engine::managed();
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Window::poll_events();
    Window::swap_buffers();
  }

  return 0;
}
