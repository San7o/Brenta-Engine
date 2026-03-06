// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/phong.hpp>
#include <brenta/renderer/pipeline.hpp>
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
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug))
    .with(Window::Builder()
          .title("load model test")
          .capture_mouse()
          .width(1280)
          .height(720))
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .with(Input::Builder())
    .build();
  auto engine = Engine::managed();
  
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

  // Model
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
  auto model_builder =
    Model::Builder()
    .path("examples/assets/models/backpack/backpack.obj")
    .transform(Transform()
               .translate(glm::vec3(25.0f, 0.0f, 0.0f))
               .rotate_y(-90.0f)
               .scale(glm::vec3(1.0)))
    .texture_props(Texture::Properties()
                   .set_flipped(true))
    .material(tenno::move(material));
  auto model_component =
    tenno::make_shared<ModelNodeComponent>(model_builder);
  Scene::add_component(root_node, model_component);

  // Skybox
  tenno::vector<std::filesystem::path> skybox_faces = {
    "examples/assets/textures/skybox/right.jpg",
    "examples/assets/textures/skybox/left.jpg",
    "examples/assets/textures/skybox/top.jpg",
    "examples/assets/textures/skybox/bottom.jpg",
    "examples/assets/textures/skybox/front.jpg",
    "examples/assets/textures/skybox/back.jpg",
  };
  scene.set_skybox(skybox_faces);
  
  // Camera movement
  
  auto camera            = scene.get_camera();  
  glm::vec3 acceleration = glm::vec3(0.0);
  glm::vec3 speed        = glm::vec3(0.0);

  Mouse mouse        = {};
  mouse.sensitivity  = 0.05f;
  bool capture_mouse = true;

  Input::add_mouse_callback("rotate_camera",
                            [&camera, &mouse, &capture_mouse](double x, double y)
  {
    if (!capture_mouse) return;
    
    if (mouse.first)
    {
      mouse.x = x;
      mouse.y = y;
      mouse.first = false;
      return;
    }

    float delta_x = x - mouse.x;
    float delta_y = y - mouse.y;
    mouse.x = x;
    mouse.y = y;

    delta_x *= mouse.sensitivity;
    delta_y *= mouse.sensitivity;

    auto pos  = camera->get_pos();
    auto acam = std::get<Camera::Aircraft>(pos);

    acam.yaw   += delta_x;
    acam.pitch -= delta_y;
    camera->set_pos(acam);
    return;
  });

  Input::add_keyboard_callback(Key::Space, [&capture_mouse, &mouse]()
  {
    capture_mouse = !capture_mouse;
    Window::set_mouse_capture(capture_mouse);

    if (!capture_mouse)
      mouse.first = true;
  });

  auto pipeline = RenderPipeline::create_default();
  
  
  INFO("Move with W / A / S / D / Q / E / Mouse");
  
  while (!Window::should_close())
  {
    float delta_time = Window::get_time().delta;
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
    scene.draw(pipeline);

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
