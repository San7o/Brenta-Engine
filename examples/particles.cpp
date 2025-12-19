// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <bitset>
#include <brenta/engine.hpp>
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
          .vsync())
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
    .camera_type(camera::camera_type::spherical)
    .projection_type(camera::projection_type::perspective)
    .spherical_coordinates({1.25f, 1.25f, 10.0f})
    .center(glm::vec3(0.0f, 2.0f, 0.0f))
    .movement_speed(2.5f)
    .mouse_sensitivity(0.05f)
    .zoom(45.0f)
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
