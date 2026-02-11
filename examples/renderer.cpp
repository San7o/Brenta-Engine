// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <iostream>

using namespace brenta;

int main()
{
  const int screen_width = 800;
  const int screen_height = 600;

  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug))
    .with(Window::Builder()
          .title("load model test")
          .width(screen_width)
          .height(screen_height))
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();
  
  auto cam = Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(45.0f)
    .build();
  
  Model m = Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .build();

  Shader::Name s = "default_shader";
  Shader::create(s,
                 Shader::Type::Vertex,   "examples/default_shader.vs",
                 Shader::Type::Fragment, "examples/default_shader.fs");

  while (!Window::should_close())
  { 
    if (Window::is_key_pressed(GLFW_KEY_ESCAPE))
      Window::close();

    Gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    Gl::clear();
    
    Renderer::begin_frame(cam);
    Renderer::submit(Renderer::Command(&m, s)
                     .translate(glm::vec3(5.0f, 0.0f, 0.0f))
                     .rotate(glm::vec3(0.0, -90.0, 0.0))
                     .scale(1.0));
    Renderer::end_frame();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
