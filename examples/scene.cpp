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
#include <brenta/renderer/scene.hpp>

#include <viotecs/viotecs.hpp>
REGISTER_SYSTEMS()

#include <memory>
#include <iostream>

#include "assets/shaders/c/default_shader_vs.c"
#include "assets/shaders/c/default_shader_fs.c"

using namespace brenta;

int main()
{
  const int screen_width = 800;
  const int screen_height = 600;

  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
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

  auto shader = Shader::create("default_shader", {
      { Shader::Type::Vertex,   default_shader_vs },
      { Shader::Type::Fragment, default_shader_fs } });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }

  auto material = Material(*shader);

  auto model = Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(5.0f, 0.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(1.0)))
    .material(std::move(material))
    .build();

  auto camera_ptr = std::make_shared<Camera>(std::move(cam));
  auto model_ptr  = std::make_shared<Model>(std::move(model));
  
  auto scene = Scene()
    .add_model(model_ptr)
    .set_active_camera(camera_ptr);

  while (!Window::should_close())
  { 
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    scene.update(Window::get_time().get_delta());
    scene.draw();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
