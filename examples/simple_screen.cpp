// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Just a simple window
//

#include <brenta/engine.hpp>
#include <brenta/window.hpp>

using namespace brenta;

int main()
{
  Engine::Builder()
    .with(Window::Builder()
          .title("simple screen")
          .width(800)
          .height(600))
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
