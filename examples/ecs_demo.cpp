// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/window.hpp>
#include <brenta/transform.hpp>
#include <brenta/logger.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/ecs/ecs.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>

#include <brenta/ecs/components/transform_ecs_component.hpp>
#include <brenta/ecs/components/camera_ecs_component.hpp>
#include <brenta/ecs/systems/camera_render_system.hpp>
#include <brenta/ecs/systems/model_render_system.hpp>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;
using namespace viotecs;

void init_ecs_scene(void)
{
  //
  // Entities
  //

  // Model entity
  auto maybe_shader =
    Shader::Builder()
    .objects({
        { Shader::Type::Vertex,   phong_vs },
        { Shader::Type::Fragment, phong_fs } })
    .build();
  if (!maybe_shader)
  {
    ERROR("error compiling shader");
    return;
  }
  auto shader =
    tenno::make_shared<Shader>(tenno::move(*maybe_shader));
  auto material =
    tenno::make_shared<Material>(Material::Builder()
                                 .shader(shader)
                                 .floating("material.shininess", 32.0f)
                                 .build());
  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/simple_cube/simple_cube.obj")
    .transform(Transform()
               .translate(glm::vec3(0.0f, 0.0f, 0.0f))
               .scale(glm::vec3(1.0)))
    .material(material);
  auto model_e = World::new_entity();
  model_e.add_component<ModelEcsComponent>(model_builder);
  model_e.add_component<TransformEcsComponent>();

  // Camera entity
  auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .phi(1.25f)
              .theta(1.25f)
              .radius(30.0f)
              .build())
    .fov(45.0f);
  auto camera_e = World::new_entity();
  camera_e.add_component<CameraEcsComponent>(camera_builder);

  //
  // Resources
  //
  
  //
  // Systems
  //

  World::register_systems<CameraRenderSystem,
                          ModelRenderSystem>();
  
}

int main(void)
{
  Engine::Builder()
    .with(Window::Builder())
    .with(Gl::Builder()
          .backface_culling())
    .with(Logger::Builder()
          .event(Logger::Event::Lifetime)
          .level(Logger::Level::Debug))
    .with(Ecs::Builder())
    .build();
  auto engine = Engine::managed();
  
  init_ecs_scene();

  while(!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    Renderer::begin_frame();
    World::tick();
    Renderer::end_frame();
    
    Window::poll_events();
    Window::swap_buffers();
  }

  World::destroy();
  return 0;
}
