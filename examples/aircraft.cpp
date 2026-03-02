// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/node_components/dir_light_node_component.hpp>
#include <brenta/node_components/point_light_node_component.hpp>
#include <brenta/node_components/model_node_component.hpp>
#include <brenta/engine.hpp>
#include <brenta/logger.hpp>
#include <brenta/window.hpp>
#include <brenta/scene.hpp>
#include <brenta/input.hpp>
#include <brenta/mouse.hpp>

#include <tenno/memory.hpp>
#include <tenno/utility.hpp>

#include "../src/renderer/shaders/c/phong_vs.c"
#include "../src/renderer/shaders/c/phong_fs.c"

using namespace brenta;

#define ACCELERATION 10.0f
#define MAX_ACCELERATION 1.5f
#define FRICTION 15.0f
void update_camera(tenno::shared_ptr<Camera> camera,
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
          .backface_culling()
          .multisample()
          .depth_test())
    .with(Input::Builder())
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
  auto shader = tenno::move(maybe_shader.value()); 
  auto material = Material(tenno::move(shader));
  material.set_float("material.shininess", 32.0f);
  
  auto phong_dir =
    PhongDirLight()
    .set_strength(0.5f);
  auto phong_dir_ptr = 
    tenno::make_shared<PhongDirLight>(tenno::move(phong_dir));
  auto phong_point =
    PhongPointLight()
    .set_strength(1.8f);
  auto phong_point_ptr =
    tenno::make_shared<PhongPointLight>(tenno::move(phong_point));

  // Setup scene
  
  auto camera_builder =
    Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Aircraft::Builder()
              .pos({0.0f, 0.0f, 0.0f})
              .build())
    .fov(30.0f);
  auto scene      = Scene(camera_builder);
  auto root_node  = scene.get_root();

  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(25.0f, 0.0f, 0.0f))
               .rotate_y(-90.0f)
               .scale(glm::vec3(1.0)))
    .texture_props(Texture::Properties()
                   .flipped(true))
    .material(tenno::move(material));
  auto model_component =
    tenno::make_shared<ModelNodeComponent>(model_builder);
  auto dir_light_component = 
    tenno::make_shared<DirLightNodeComponent>(phong_dir_ptr);
  auto point_light_component = 
    tenno::make_shared<PointLightNodeComponent>(phong_point_ptr);
  
  Scene::add_component(root_node, model_component);
  Scene::add_component(root_node, dir_light_component);
  Scene::add_component(root_node, point_light_component);

  // Camera movement
  
  auto camera        = scene.get_camera();  
  glm::vec3 acceleration = glm::vec3(0.0);
  glm::vec3 speed        = glm::vec3(0.0);

  Mouse mouse = {};
  mouse.set_sensitivity(0.05f);
  bool capture_mouse = true;

  Input::add_mouse_callback("rotate_camera",
                            [&camera, &mouse, &capture_mouse](double x, double y)
  {
    if (!capture_mouse) return;
    
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

  Input::add_keyboard_callback(Key::Space, [&capture_mouse, &mouse]()
  {
    capture_mouse = !capture_mouse;
    Window::set_mouse_capture(capture_mouse);

    if (!capture_mouse)
      mouse.set_first(true);
  });

  INFO("Move with W / A / S / D / Q / E / Mouse");
  
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

void update_camera(tenno::shared_ptr<Camera> camera,
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
  
  speed        += (acceleration - FRICTION * speed) * delta_time;
  acceleration += -acceleration * FRICTION * delta_time;

  auto pos  = camera->get_pos();
  auto apos = std::get<Camera::Aircraft>(pos);

  apos.pos += camera->get_front() * speed.x;
  apos.pos += camera->get_up()    * speed.y;
  apos.pos += camera->get_right() * speed.z;
  
  camera->set_pos(apos);
  return;
}
