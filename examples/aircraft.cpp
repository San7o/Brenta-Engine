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
#include <brenta/input.hpp>
#include <brenta/mouse.hpp>

#include <memory>
#include <iostream>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

#define ACCELERATION 10.0f
#define MAX_ACCELERATION 1.5f
#define FRICTION 15.0f
void update_camera(std::shared_ptr<Camera> camera,
                   glm::vec3 &acceleration,
                   glm::vec3 &speed,
                   float delta_time);

int main()
{
  const int screen_width = 1280;
  const int screen_height = 720;

  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("load model test")
          .capture_mouse()
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
  
  auto camera =
    std::make_shared<Camera>(Camera::Builder()
                             .projection_type(Camera::ProjectionType::Perspective)
                             .position(Camera::Aircraft::Builder()
                                       .pos({0.0f, 0.0f, 0.0f})
                                       .build())
                             .fov(30.0f)
                             .build());

  auto shader = Shader::create("default_shader", {
      { Shader::Type::Vertex,   phong_vs },
      { Shader::Type::Fragment, phong_fs } });
  if (!shader)
  {
    ERROR("Error creating shader");
    return 1;
  }

  auto material = std::move(Material(shader.value())
                            .set_float("material.shininess", 32.0f));

  auto model =
    std::make_shared<Model>(Model::Builder()
                            .path("examples/assets/models/backpack/backpack.obj")
                            .transform(Transform()
                                       .translate(glm::vec3(15.0f, 0.0f, 0.0f))
                                       .rotate_y(-90.0f)
                                       .scale(glm::vec3(1.0)))
                            .material(std::move(material))
                            .build());

  auto phong_dir =
    std::make_shared<PhongDirLight>(PhongDirLight()
                                    .set_strength(0.5f));
  auto phong_point =
    std::make_shared<PhongPointLight>(PhongPointLight()
                                      .set_strength(1.8f));
  
  auto scene = Scene(camera);
  auto root_node = scene.get_root();
  
  root_node->add_model(model);
  root_node->add_point_light(phong_point);
  root_node->set_dir_light(phong_dir);

  // Camera movement
  glm::vec3 acceleration = glm::vec3(0.0);
  glm::vec3 speed = glm::vec3(0.0);

  Mouse mouse = {};
  mouse.set_sensitivity(0.05f);
  
  Input::add_mouse_callback("rotate_camera", [camera, &mouse](double x, double y)
  {
    if (mouse.get_first())
    {
      mouse.set_x(x);
      mouse.set_y(y);
      mouse.set_first(false);
      return;
    }

    float delta_x = x - mouse.get_x();
    float delta_y = y - mouse.get_y();
    mouse.set_x(x);
    mouse.set_y(y);

    auto sensitivity = mouse.get_sensitivity();
    delta_x *= sensitivity;
    delta_y *= sensitivity;

    auto pos = camera->get_pos();
    auto acam = std::get<Camera::Aircraft>(pos);

    acam.yaw += delta_x;
    acam.pitch -= delta_y;
    camera->set_pos(acam);
    return;
  });

  bool capture_mouse = true;
  Input::add_keyboard_callback(Key::Space, [&capture_mouse]()
  {
    capture_mouse = !capture_mouse;
    Window::set_mouse_capture(capture_mouse);
  });

  
  while (!Window::should_close())
  {
    float delta_time = Window::get_time().get_delta();
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    if (Window::is_key_pressed(Key::W))
      acceleration.x = ACCELERATION;
    if (Window::is_key_pressed(Key::S))
      acceleration.x = -ACCELERATION;
    if (Window::is_key_pressed(Key::A))
      acceleration.z = -ACCELERATION;
    if (Window::is_key_pressed(Key::D))
      acceleration.z = ACCELERATION;
    if (Window::is_key_pressed(Key::E))
      acceleration.y = ACCELERATION;
    if (Window::is_key_pressed(Key::Q))
      acceleration.y = -ACCELERATION;

    update_camera(camera, acceleration, speed, delta_time);
        
    Gl::set_color(Color::grey());
    Gl::clear();
    
    scene.update(delta_time);
    scene.draw();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}

void update_camera(std::shared_ptr<Camera> camera,
                   glm::vec3 &acceleration,
                   glm::vec3 &speed,
                   float delta_time)
{
  if (acceleration.x > MAX_ACCELERATION)
    acceleration.x = MAX_ACCELERATION;
  if (acceleration.y > MAX_ACCELERATION)
    acceleration.y = MAX_ACCELERATION;
  if (acceleration.z > MAX_ACCELERATION)
    acceleration.z = MAX_ACCELERATION;

  if (acceleration.x < -MAX_ACCELERATION)
    acceleration.x = -MAX_ACCELERATION;
  if (acceleration.y < -MAX_ACCELERATION)
    acceleration.y = -MAX_ACCELERATION;
  if (acceleration.z < -MAX_ACCELERATION)
    acceleration.z = -MAX_ACCELERATION;
  
  speed += (acceleration - FRICTION * speed) * delta_time;
  acceleration += -acceleration * FRICTION * delta_time;

  auto pos = camera->get_pos();
  auto apos = std::get<Camera::Aircraft>(pos);

  apos.pos += camera->get_front() * speed.x;
  apos.pos += camera->get_up()    * speed.y;
  apos.pos += camera->get_right() * speed.z;
  
  camera->set_pos(apos);
  return;
}
