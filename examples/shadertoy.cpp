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

#ifdef BRENTA_USE_IMGUI

#include <brenta/engine.hpp>

#include <iostream>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

using namespace brenta;

int main(int argc, char** argv)
{
  //
  // Setup
  //
  
  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug)
               .file("/tmp/brenta-logs"))
    .subsystem(window::builder()
               .title("shadertoy")
               .width(800)
               .height(600)
               .vsync())
    .subsystem(gl::builder()
               .blending()
               .cull_face()
               .multisample()
               .depth_test())
    .subsystem(gui::builder())
    .build();


  // TODO:
  // - audio
  // - input
  // - text
  // - ecs
  
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }

  //
  // Variables
  //
  
  types::framebuffer fb(window::get_width(), window::get_height());
  
  //
  // Game loop
  //

  while(!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    gui::new_frame(&fb);

    fb.bind();
    gl::clear();

    // Draw here....

    fb.unbind();
    gui::render();
    
    window::poll_events();
    window::swap_buffers();
  }

  //
  // Cleanup
  //
  
  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}

#endif // BRENTA_USE_IMGUI
