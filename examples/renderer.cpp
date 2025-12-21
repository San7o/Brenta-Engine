// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <iostream>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;
  
const int screen_width = 800;
const int screen_height = 600;

int main()
{
  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug))
    .with(window::builder()
          .title("load model test")
          .width(screen_width)
          .height(screen_height))
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = engine::managed();
  
  auto cam = camera::builder()
    .camera_type(camera::camera_type::aircraft)
    .projection_type(camera::projection_type::perspective)
    .position({0.0f, 0.0f, 0.0f})
    .zoom(45.0f)
    .build();
  
  model m = model::builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .build();

  shader::name_t s = "default_shader";
  shader::create(s,
                 shader::type::vertex, "examples/default_shader.vs",
                 shader::type::fragment, "examples/default_shader.fs");

  while (!window::should_close())
  { 
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    gl::clear();
    
    renderer::begin_frame(cam);
    renderer::submit(renderer::item(&m, s)
                     .translate(glm::vec3(5.0f, 0.0f, 0.0f))
                     .rotate(glm::vec3(0.0, -90.0, 0.0))
                     .scale(1.0));
    renderer::end_frame();
    
    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
