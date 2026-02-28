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
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/renderer/asset_manager.hpp>
#include <brenta/input.hpp>
#include <brenta/mouse.hpp>
#include <brenta/text.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

int main()
{
  //
  // Init
  //
  
  const int screen_width = 1280;
  const int screen_height = 720;

  Engine::Builder()
    .with(Logger::Builder()
          .event(Logger::Event::Lifetime)
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
    .with(Input::Builder())
    .build();
  auto engine = Engine::managed();

  //
  // Load assets
  //
  
  auto camera =
    tenno::make_shared<Camera>(Camera::Builder()
                               .projection_type(Camera::ProjectionType::Perspective)
                               .position(Camera::Spherical::Builder()
                                         .center({0.0f, 0.0f, 0.0f})
                                         .phi(1.25f)
                                         .theta(1.25f)
                                         .radius(30.0f)
                                         .build())
                               .fov(20.0f)
                               .build());

  auto font = AssetManager::new_asset<Font>("default_font",
                                            Font::Builder()
                                            .path("examples/assets/fonts/arial.ttf")
                                            .size(32.0f));
  auto shader =
    AssetManager::new_asset<Shader>("default_shader",
                                    Shader::Builder()
                                    .objects({
                                        { Shader::Type::Vertex,   phong_vs },
                                        { Shader::Type::Fragment, phong_fs } }));
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }

  auto material =
    AssetManager::new_asset<Material>("shiny_material",
                                      Material::Builder()
                                      .shader(shader)
                                      .floating("material.shininess", 32.0f));

  auto model =
    AssetManager::new_asset<Model>("simple_cube",
                                   Model::Builder()
                                   .path("examples/assets/models/simple_cube/simple_cube.obj")
                                   .transform(Transform()
                                              .translate(glm::vec3(0.0f, 0.0f, 0.0f))
                                              .scale(glm::vec3(1.0)))
                                   .material(material));

  auto phong_dir =
    tenno::make_shared<PhongDirLight>(PhongDirLight()
                                      .set_strength(0.5f));
  auto phong_point =
    tenno::make_shared<PhongPointLight>(PhongPointLight()
                                        .set_strength(1.8f));

  //
  // Setup scene
  //
  
  auto scene = AssetManager::new_asset<Scene>("main_scene",
                                              Scene::Builder()
                                              .camera(camera));
  auto root_node = scene->get_root();
  
  auto model_node = Scene::create_child(root_node);
  model_node->add_model(model);
    
  auto point_node = Scene::create_child(root_node);
  point_node->add_point_light(phong_point);
  point_node->get_local().translate({-2.0, 2.0, 0.0});

  auto dir_node = Scene::create_child(root_node);
  dir_node->set_dir_light(phong_dir);
  
  Mouse mouse = {};
  mouse.set_sensitivity(0.05f);
  
  Input::add_mouse_callback("rotate_camera",
                            [camera, &mouse](double xpos, double ypos)
  {
    // Rotate the camera
    if (Window::is_key_pressed(Key::LeftShift))
    {
      if (mouse.get_first())
      {
        mouse.set_x(xpos);
        mouse.set_y(ypos);
        mouse.set_first(false);
      }

      float xoffset = xpos - mouse.get_x();
      float yoffset = mouse.get_y() - ypos;
      mouse.set_x(xpos);
      mouse.set_y(ypos);

      auto sensitivity = mouse.get_sensitivity();
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);
        scam.theta += yoffset * sensitivity;
        scam.phi   += xoffset * sensitivity;

        if (scam.theta <= 0.01f) scam.theta = 0.01f;
        if (scam.theta >= 3.13f) scam.theta = 3.13f;

        camera->set_pos(scam);
        
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    // translate the cam center
    else if (Window::is_key_pressed(Key::LeftControl))
    {
      if (mouse.get_first())
      {
        mouse.set_x(xpos);
        mouse.set_y(ypos);
        mouse.set_first(false);
      }

      float xoffset = xpos - mouse.get_x();
      float yoffset = mouse.get_y() - ypos;
      mouse.set_x(xpos);
      mouse.set_y(ypos);

      auto sensitivity = mouse.get_sensitivity();
      xoffset *= sensitivity * 0.3f;
      yoffset *= sensitivity * 0.3f;

      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);
        glm::vec3 world_pos = camera->get_transform().get_pos();
        // Local coordinate system
        glm::vec3 fixed_center =
          glm::vec3(scam.center.x, world_pos.y, scam.center.z);
        // Unit vectors
        glm::vec3 front =
          glm::normalize(world_pos - fixed_center);
        glm::vec3 right =
          glm::normalize(glm::cross(front, camera->get_world_up()));

        scam.center += right * glm::vec3(xoffset);
        scam.center -= camera->get_world_up() * glm::vec3(yoffset);

        camera->set_pos(scam);
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    // zoom the camera
    else if (Window::is_key_pressed(Key::LeftAlt))
    {
      if (mouse.get_first())
      {
        mouse.set_x(xpos);
        mouse.set_y(ypos);
        mouse.set_first(false);
      }

      float xoffset = xpos - mouse.get_x();
      float yoffset = mouse.get_y() - ypos;
      mouse.set_x(xpos);
      mouse.set_y(ypos);

      xoffset *= mouse.get_sensitivity();
      yoffset *= mouse.get_sensitivity();
      
      auto new_cam = camera->get_pos();
      try {
        Camera::Spherical scam = std::get<Camera::Spherical>(new_cam);

        scam.radius -= yoffset;
        if (scam.radius <= 0.1f) scam.radius = 0.1f;

        camera->set_pos(scam);
      } catch ([[maybe_unused]] const std::bad_variant_access& ex) {
        return;
      }
    }
    else
    {
      mouse.set_first(true);
    }
  });

  Input::add_keyboard_callback(Key::R, [](){

    // Make sure we are not using the old model
    Renderer::flush();
    
    DEBUG("Reloading model");
    if (!AssetManager::reload<Model>("simple_cube"))
    {
      ERROR("Error in reloading the model");
      return;
    }
    DEBUG("Done reloading model");
  });

  //
  // Main loop
  //

  // Setup hotreloading
  AssetManager::hotreload_activate();
  
  while (!Window::should_close())
  {
    float delta_time = Window::get_time().get_delta();
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    AssetManager::hotreload_update();
    
    Gl::set_color(Color::grey());
    Gl::clear();

    Text::render("Move with Shift / Ctrl / Alt + Mouse",
                 25.0f, 25.0f, 1.0f,
                 Color::yellow(), font);
    Text::render("Press R to reload the model",
                 25.0f, 55.0f, 1.0f,
                 Color::yellow(), font);
    
    scene->update(delta_time);
    scene->draw();

    Window::poll_events();
    Window::swap_buffers();
  }
  
  AssetManager::hotreload_deactivate();
  AssetManager::clear();
  return 0;
}
