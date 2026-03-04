// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/engine.hpp>
#include <brenta/input.hpp>
#include <brenta/time.hpp>
#include <brenta/logger.hpp>
#include <brenta/gui.hpp>
#include <brenta/window.hpp>
#include <brenta/mouse.hpp>

#include <bitset>

using namespace brenta;

void setup_gui(FrameBuffer &fb,
               ParticleEmitter *emitter);

int main()
{
  const int SCR_WIDTH = 1280;
  const int SCR_HEIGHT = 720;

  //
  // Setup
  //
  
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("particles")
          .width(SCR_WIDTH)
          .height(SCR_HEIGHT)
          .msaa()
          .vsync()
          .debug())
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .with(Gui::Builder())
    .with(Input::Builder())
    .build(); 
  auto engine = Engine::managed();

  auto camera_builder =
    Camera::Builder()
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .theta(1.25f)
              .phi(1.25f)
              .radius(10.0f)
              .build())
    .projection_type(Camera::ProjectionType::Perspective)
    .fov(45.0f);
  auto camera =
    tenno::make_shared<Camera>(camera_builder);

  auto emitter = ParticleEmitter::Builder()
      .with_camera(camera)
      .starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
      .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
      .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
      .starting_time_to_live(0.5f)
      .num_particles(1000)
      .spawn_rate(0.99f)
      .scale(1.0f)
      .atlas_path("examples/assets/textures/particle_atlas.png")
      .atlas_width(8)
      .atlas_height(8)
      .atlas_index(3)
      .build();

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
  
  Gui::load_font();
  FrameBuffer fb(Window::get_width(), Window::get_height());  
  
  //
  // Render loop
  //
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();
    
    setup_gui(fb, &emitter);
    
    Gl::set_color(Color::grey());
    Gl::clear();

    fb.bind();
    emitter.update(Window::get_time().delta);
    emitter.render();
    fb.unbind();

    Gui::render();
    
    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}

void setup_gui(FrameBuffer &fb,
               ParticleEmitter *emitter)
{
  Gui::new_frame(&fb, "particles");
  Gui::push_font();
  ImGui::Begin("Particle settings");

  // Position and Movement
  ImGui::SliderFloat3("Starting position",
                      &emitter->starting_position.x, -50.0f, 50.0f);
  ImGui::SliderFloat3("Starting velocity",
                      &emitter->starting_velocity.x, -15.0f, 15.0f);
  ImGui::SliderFloat3("Starting spread",
                      &emitter->starting_spread.x, 0.0f, 10.0f);

  // Timing and Life
  ImGui::SliderFloat("Starting time to live",
                     &emitter->starting_time_to_live, 0.0f, 15.0f);
  ImGui::SliderFloat("Spawn rate",
                     &emitter->spawn_rate, 0.0f, 1.0f);

  // Quantity and Appearance
  ImGui::SliderInt("Number of particles",
                   &emitter->num_particles, 1, BRENTA_MAX_PARTICLES);
  ImGui::SliderFloat("Scale",
                     &emitter->scale, 0.01f, 5.0f);
  
  ImGui::End();
  Gui::pop_font();
}
