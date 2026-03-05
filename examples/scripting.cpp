// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/scene.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

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
          .backface_culling()
          .multisample()
          .depth_test())
    .build();
  auto engine = Engine::managed();
  
  auto maybe_shader = Shader::create({
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs } });
  if (!maybe_shader)
  {
    ERROR("Error creating shader");
    return 1;
  }
  auto shader        = tenno::move(maybe_shader.value());
  auto material      = Material(tenno::move(shader));
  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(5.0f, 0.0f, 0.0f))
               .rotate(glm::angleAxis(glm::radians(-90.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f)))
               .scale(glm::vec3(1.0)))
    .texture_props(Texture::Properties()
                   .flipped(true))
    .material(tenno::move(material));
  auto model_component =
    tenno::make_shared<ModelNodeComponent>(model_builder);

  
  auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(45.0f);
  auto scene      = Scene(camera_builder);
  auto root_node  = scene.get_root();
  auto model_node = Scene::create_child(root_node);
  model_node->transform = glm::vec3(10.0f, 0.0f, 0.0f);

  Scene::add_component(model_node, model_component);

  Scene::set_script(model_node,
                    std::filesystem::path("./examples/assets/scripts/circle.lua"));

  auto pipeline = RenderPipeline::create_default();
  
  while (!Window::should_close())
  {
    auto delta_time = Window::get_time().delta;
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    scene.update(delta_time);
    scene.draw(pipeline);
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
