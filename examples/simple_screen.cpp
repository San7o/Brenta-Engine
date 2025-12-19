// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Just a simple window
//

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
    .with(window::builder()
          .title("simple screen")
          .width(SCR_WIDTH)
          .height(SCR_HEIGHT))
    .build();
  
  auto engine = engine::managed();
  
  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    window::poll_events();
    window::swap_buffers();
  }

  return 0;
}
