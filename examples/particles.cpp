// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/particles.hpp>
#include <brenta/engine.hpp>
#include <brenta/window.hpp>
#include <brenta/input.hpp>
#include <brenta/time.hpp>
#include <brenta/logger.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/gui/gui.hpp>

#include <bitset>

using namespace brenta;

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
          .cull_face()
          .multisample()
          .depth_test())
    .with(Gui::Builder())
    .with(Input::Builder())
    .build(); 
  auto engine = Engine::managed();

  auto camera = Camera::Builder()
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .theta(1.25f)
              .phi(1.25f)
              .radius(10.0f)
              .build())
    .projection_type(Camera::ProjectionType::Perspective)
    .fov(45.0f)
    .build();

  auto emitter = ParticleEmitter::Builder()
      .with_camera(&camera)
      .starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
      .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
      .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
      .starting_time_to_live(0.5f)
      .num_particles(1000)
      .spawn_rate(0.01f)
      .scale(1.0f)
      .atlas_path("examples/assets/textures/particle_atlas.png")
      .atlas_width(8)
      .atlas_height(8)
      .atlas_index(3)
      .build();

  //
  // Render loop
  //
  
  while (!Window::should_close())
  {
    if (Window::is_key_pressed(Key::Escape))
      Window::close();

    Gl::set_color(Color::grey());
    Gl::clear();

    emitter.update(Window::get_time().get_delta());
    emitter.render();

    Window::poll_events();
    Window::swap_buffers();
  }
  return 0;
}
