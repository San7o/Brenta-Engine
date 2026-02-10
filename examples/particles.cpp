// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#include <brenta/window.hpp>
#include <brenta/input.hpp>
#include <brenta/time.hpp>
#include <brenta/logger.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/gui/gui.hpp>

#include <bitset>

#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

int main()
{
  //
  // Setup
  //
  
  engine::builder()
    .with(logger::builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(window::builder()
          .title("particles")
          .width(SCR_WIDTH)
          .height(SCR_HEIGHT)
          .msaa()
          .vsync()
          .debug())
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(gui::builder())
    .with(input::builder())
    .build(); 
  auto engine = engine::manager();

  auto camera = camera::builder()
    .position(camera::spherical::builder()
              .center({0.0f, 2.0f, 0.0f})
              .theta(1.25f)
              .phi(1.25f)
              .radius(10.0f)
              .build())
    .projection_type(camera::projection_type::perspective)
    .fov(45.0f)
    .build();

  auto emitter =
    particle_emitter::builder()
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
  
  time::update(window::get_time());
  while (!window::should_close())
  {
    if (window::is_key_pressed(GLFW_KEY_ESCAPE))
      window::close();

    gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    gl::clear();

    emitter.update_particles(time::get_delta_time());
    emitter.render_particles();

    time::update(window::get_time());

    window::poll_events();
    window::swap_buffers();
  }
  return 0;
}
