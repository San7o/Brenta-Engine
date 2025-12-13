// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/engine.hpp>
#ifdef BRENTA_USE_ECS
#include <demo/game_ecs.hpp>
#include <viotecs/viotecs.hpp>
#endif
#include <bitset>
#include <filesystem>

using namespace brenta;

// Default resolution
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

#ifdef BRENTA_USE_ECS
using namespace viotecs;

REGISTER_SYSTEMS(RendererSystem, PointLightsSystem, DebugTextSystem,
                 DirectionalLightSystem, PhysicsSystem, CollisionsSystem);
#endif

/* default camera */
namespace brenta
{
camera default_camera = camera();
}

int main()
{

  auto& engine = engine::builder()
    .subsystem(logger::builder()
               .level(oak::level::debug)
               .file("/tmp/brenta-logs"))
    .subsystem(window::builder()
               .title("brenta demo")
               .width(800)
               .height(600)
               .vsync()
               .msaa())
    .subsystem(gl::builder()
               .blending()
               .cull_face()
               .multisample()
               .depth_test())
    .subsystem(audio::builder()
               .load("guitar", "examples/assets/audio/guitar.wav"))
    .subsystem(input::builder())
    .subsystem(ecs::builder())
    .subsystem(gui::builder())
    .subsystem(text::builder()
               .font("examples/assets/fonts/arial.ttf")
               .size(40))
    .build();
  auto ret = engine.initialize();
  if (!ret.has_value())
  {
    ERROR("Failed to initialize subsystem {}", ret.error());
    return 1;
  }

  default_camera = camera::builder()
                     .camera_type(enums::camera_type::SPHERICAL)
                     .projection_type(enums::projection_type::PERSPECTIVE)
                     .spherical_coordinates({1.25f, 1.25f, 30.0f})
                     .center(glm::vec3(0.0f, 2.0f, 0.0f))
                     .movement_speed(2.5f)
                     .mouse_sensitivity(0.05f)
                     .zoom(45.0f)
                     .build();

#ifdef BRENTA_USE_ECS
  init_player_entity();
  // init_cube_entity();
  init_floor_entity();
  init_directional_light_entity();
  init_point_light_entity();
  init_sphere_entity();
  init_robot_entity();

  init_toggle_wireframe_callback();
  init_close_window_callback();
  init_camera_mouse_callback();
  init_play_guitar_callback();

  world::add_resource<WireframeResource>(false);
#endif

  particle_emitter emitter =
    particle_emitter::builder()
      .with_camera(&default_camera)
      .starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
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
      .atlas_index(5)
      .build();

#ifdef BRENTA_USE_IMGUI
  brenta::types::framebuffer fb(SCR_WIDTH, SCR_HEIGHT);
#endif

  time::update(window::get_time());
  while (!window::should_close())
  {
    window::poll_events();

#ifdef BRENTA_USE_IMGUI
    gui::new_frame(&fb);
    fb.bind();
#endif

    gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    gl::clear();

    emitter.update_particles(time::get_delta_time());
    emitter.render_particles();

#ifdef BRENTA_USE_ECS
    time::update(window::get_time());
    world::tick();
#endif

#ifdef BRENTA_USE_IMGUI
    fb.unbind();
    gui::render();
#endif
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
