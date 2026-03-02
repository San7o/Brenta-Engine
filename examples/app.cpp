// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/asset_manager.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/opengl/gl.hpp>

#define BRENTA_MAIN
#include <brenta/app.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>
#include <iostream>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

bool App::setup()
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
  auto engine = Engine::instance();
  engine.initialize();
  
  auto shader =
    AssetManager::new_asset<Shader>("default_shader",
                                    Shader::Builder()
                                    .objects({
                                        { Shader::Type::Vertex,   phong_vs },
                                        { Shader::Type::Fragment, phong_fs } }));
  if (!shader)
  {
    ERROR("Error creating shader");
    return false;
  }
  
  auto material_builder =
    Material::Builder()
    .shader(shader);
  auto material =
    AssetManager::new_asset<Material>("backpack_material",
                                      material_builder);

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
    .material(material);
  auto model =
    AssetManager::new_asset<Model>("backpack",
                                   model_builder);

  auto model_component =
    tenno::make_shared<ModelNodeComponent>(model);

  
  auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(45.0f);
  auto scene_builder =
    Scene::Builder()
    .camera(camera_builder);
  auto scene =
    AssetManager::new_asset<Scene>("main_scene",
                                   scene_builder);
  auto root_node  = scene->get_root();
  auto model_node = Scene::create_child(root_node);
  model_node->set_local(glm::vec3(10.0f, 0.0f, 0.0f));
  Scene::add_component(model_node, model_component);

  return true;
}

bool App::update(float delta_time)
{
  if (Window::is_key_pressed(Key::Escape))
    Window::close();

  Gl::set_color(Color::grey());
  Gl::clear();

  auto scene = AssetManager::get<Scene>("main_scene");
  if (!scene) return true;
  
  scene->update(delta_time);
  scene->draw();

  return true;
}

void App::cleanup()
{
  AssetManager::clear();
  
  auto engine = Engine::instance();
  engine.terminate();
}
