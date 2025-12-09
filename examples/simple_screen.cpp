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
  auto& engine = engine::builder()
    .subsystem(window::builder()
               .title("simple screen")
               .width(SCR_WIDTH)
               .height(SCR_HEIGHT))
    .build();
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }
    
  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    window::poll_events();
    window::swap_buffers();
  }

  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}
