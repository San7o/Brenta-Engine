// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <bitset>
#include <brenta/engine.hpp>
#include <filesystem>
#include <viotecs/viotecs.hpp>

REGISTER_SYSTEMS()

using namespace brenta;

// Default resolution
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

/* default camera */
namespace brenta
{
camera default_camera = camera();
}

int main()
{
  //
  // Setup
  //
  
  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug)
               .file("/tmp/brenta-logs"))
    .subsystem(window::builder()
               .title("particles")
               .width(SCR_WIDTH)
               .height(SCR_HEIGHT)
               .msaa()
               .vsync())
    .subsystem(gl::builder()
               .blending()
               .cull_face()
               .multisample()
               .depth_test())
    .subsystem(gui::builder())
    .subsystem(input::builder())
    .build(); 
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    oak::error("Failed to initialize subsystem {}", ret.error());
    return 1;
  }

  default_camera = camera::builder()
                     .set_camera_type(enums::camera_type::SPHERICAL)
                     .set_projection_type(enums::projection_type::PERSPECTIVE)
                     .set_spherical_coordinates({1.25f, 1.25f, 10.0f})
                     .set_center(glm::vec3(0.0f, 2.0f, 0.0f))
                     .set_movement_speed(2.5f)
                     .set_mouse_sensitivity(0.05f)
                     .set_zoom(45.0f)
                     .build();

  particle_emitter emitter =
    particle_emitter::builder()
      .with_camera(&default_camera)
      .starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
      .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
      .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
      .starting_time_to_live(0.5f)
      .num_particles(1000)
      .spawn_rate(0.01f)
      .scale(1.0f)
      .atlas_path(
        std::filesystem::absolute("examples/assets/textures/particle_atlas.png")
          .string())
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

  //
  // Cleanup
  //
  
  ret = engine.terminate();
  if (!ret.has_value())
  {
    oak::error("Failed to terminate subsystem {}", ret.error());
    return 1;
  }
  return 0;
}
