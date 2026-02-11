// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/brenta.hpp>
#ifdef BRENTA_USE_ECS
#include <demo/game_ecs.hpp>
#include <viotecs/viotecs.hpp>
#endif
#include <bitset>

using namespace brenta;

// Default resolution
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

#ifdef BRENTA_USE_ECS
using namespace viotecs;

REGISTER_SYSTEMS(RendererSystem, PointLightsSystem, DebugTextSystem,
                 DirectionalLightSystem, PhysicsSystem, CollisionsSystem);
#endif

int main()
{
  engine::builder()
    .with(logger::builder()
          .level(oak::level::info)
          .file("/tmp/brenta-logs"))
    .with(window::builder()
          .title("brenta demo")
          .width(800)
          .height(600)
          .vsync()
          .msaa()
          .debug())
    .with(gl::builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(audio::builder()
          .sound("guitar", "examples/assets/audio/guitar.wav"))
    .with(input::builder())
    .with(ecs::builder())
    .with(gui::builder())
    .with(text::builder()
          .font("examples/assets/fonts/arial.ttf")
          .size(40))
    .build();
  
  auto engine = engine::managed();
  
  auto camera = camera::builder()
    .projection_type(camera::projection_type::perspective)
    .position(camera::spherical::builder()
              .center({0.0f, 2.0f, 0.0f})
              .phi(1.25f)
              .theta(1.25f)
              .radius(30.0f)
              .build())
    .fov(45.0f)
    .build();

  mouse mouse = {};
  mouse.set_sensitivity(0.05f);
  
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
  init_camera_mouse_callback(&camera, &mouse);
  init_play_guitar_callback();

  world::add_resource<WireframeResource>(false);
  world::add_resource<CameraResource>(&camera);
#endif

  auto emitter = particle_emitter::builder()
    .with_camera(&camera)
    .starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
    .starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
    .starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
    .starting_time_to_live(0.5f)
    .num_particles(1000)
    .spawn_rate(0.01f)
    .scale(1.0f)
    .atlas_path("examples/assets/textures/particle_atlas.png")
    .atlas_width(8)
    .atlas_height(8)
    .atlas_index(5)
    .build();

#ifdef BRENTA_USE_IMGUI
  brenta::types::framebuffer fb(SCR_WIDTH, SCR_HEIGHT);
#endif

  while (!window::should_close())
  {
    window::poll_events();

#ifdef BRENTA_USE_IMGUI
    gui::new_frame(&fb, "demo");
    fb.bind();
#endif
    
    gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    gl::clear();

    emitter.update_particles(window::get_time().get_delta());
    emitter.render_particles();

#ifdef BRENTA_USE_ECS
    world::tick();
#endif

#ifdef BRENTA_USE_IMGUI
    fb.unbind();
    gui::render();
#endif
    window::swap_buffers();
  }

  return 0;
}
