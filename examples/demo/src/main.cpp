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

#ifdef BRENTA_USE_ECS
using namespace viotecs;

REGISTER_SYSTEMS(RendererSystem, PointLightsSystem, DebugTextSystem,
                 DirectionalLightSystem, PhysicsSystem, CollisionsSystem);
#endif

int main()
{
  const int SCR_WIDTH = 1280;
  const int SCR_HEIGHT = 720;

  Engine::Builder()
    .with(Logger::Builder()
          .level(oak::level::debug)
          .file("/tmp/brenta-logs"))
    .with(Window::Builder()
          .title("brenta demo")
          .width(800)
          .height(600)
          .vsync()
          .msaa()
          .debug())
    .with(Gl::Builder()
          .blending()
          .cull_face()
          .multisample()
          .depth_test())
    .with(Audio::Builder()
          .sound("guitar", "examples/assets/audio/guitar.wav"))
    .with(Input::Builder())
    .with(Ecs::Builder())
    .with(Gui::Builder())
    .with(Text::Builder()
          .font("examples/assets/fonts/arial.ttf")
          .size(40))
    .build();
  
  auto engine = Engine::managed();
  
  auto camera = Camera::Builder()
    .projection_type(Camera::ProjectionType::Perspective)
    .position(Camera::Spherical::Builder()
              .center({0.0f, 2.0f, 0.0f})
              .phi(1.25f)
              .theta(1.25f)
              .radius(30.0f)
              .build())
    .fov(45.0f)
    .build();

  Mouse mouse = {};
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

  auto emitter = ParticleEmitter::Builder()
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
  FrameBuffer fb(SCR_WIDTH, SCR_HEIGHT);
#endif

  while (!Window::should_close())
  {
    Window::poll_events();

#ifdef BRENTA_USE_IMGUI
    Gui::new_frame(&fb, "demo");
    fb.bind();
#endif
    
    Gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
    Gl::clear();

    emitter.update(Window::get_time().get_delta());
    emitter.render();

#ifdef BRENTA_USE_ECS
    world::tick();
#endif

#ifdef BRENTA_USE_IMGUI
    fb.unbind();
    Gui::render();
#endif
    Window::swap_buffers();
  }

  return 0;
}
