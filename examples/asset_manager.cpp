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
#include <brenta/renderer/asset_manager.hpp>

#include <memory>
#include <iostream>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

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
  
  auto camera =
    std::make_shared<Camera>(Camera::Builder()
                             .projection_type(Camera::ProjectionType::Perspective)
                             .position(Camera::Aircraft::Builder()
                                       .pos({0.0f, 0.0f, 0.0f})
                                       .build())
                             .fov(45.0f)
                             .build());

  auto shader = AssetManager::new_shader("default_shader", {
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs } });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  
  auto material = AssetManager::new_material("backpack_material", shader);

  auto model =
    AssetManager::new_model("backpack",
                            std::move(Model::Builder()
                            .path("examples/assets/models/backpack/backpack.obj")
                            .transform(Transform()
                                       .translate(glm::vec3(5.0f, 0.0f, 0.0f))
                                       .rotate(glm::angleAxis(glm::radians(-90.0f),
                                                              glm::vec3(0.0f, 1.0f, 0.0f)))
                                       .scale(glm::vec3(1.0)))
                                      .material(material)));

  auto scene = AssetManager::new_scene("main_scene", camera);
  auto root_node = scene->get_root();
  auto model_node = root_node->new_node();
  model_node->add_model(model);
  model_node->set_local(glm::vec3(10.0f, 0.0f, 0.0f));

  while (!Window::should_close())
  {
    auto delta_time = Window::get_time().get_delta();
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    scene->update(delta_time);
    scene->draw();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
