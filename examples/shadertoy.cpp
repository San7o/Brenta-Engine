// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// shadertoy
// =========
//
// Play with shaders interactively
//

#include <brenta/engine.hpp>

#include <iostream>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

int main(int argc, char** argv)
{
  auto& engine =
    brenta::engine::builder()
    .subsystem(brenta::window::builder()
               .title("shadertoy")
               .width(800)
               .height(600)
               .vsync())
    .build();
  
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }
  
  std::cout << "Hello, World" << std::endl;

  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }
  return 0;
}
