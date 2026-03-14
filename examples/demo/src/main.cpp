// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/brenta.hpp>
#include <demo/demo.hpp>

using namespace brenta;
using namespace viotecs;

int main()
{
  Engine::Builder()
    .with(Logger::Builder()
          .level(Logger::Level::Debug)
          //.event(Logger::Event::Lifetime)
          .file("brenta-logs"))
    .with(Window::Builder()
          .title("brenta demo")
          .width(1280)
          .height(720)
          .vsync()
          .msaa()
          .debug())
    .with(Gl::Builder()
          .blending()
          .backface_culling()
          .multisample()
          .depth_test())
    .with(SoundManager::Builder())
    .with(Input::Builder())
    .with(Ecs::Builder())
    .with(Gui::Builder())
    .build();

  auto engine = Engine::instance();
  engine.initialize();
  ///auto engine = Engine::managed();

  {  // Local scope

    auto camera_builder =
      Camera::Builder()
      .projection_type(Camera::ProjectionType::Perspective)
      .position(Camera::Spherical::Builder()
                .center({0.0f, 2.0f, 0.0f})
                .phi(1.25f)
                .theta(1.25f)
                .radius(30.0f)
                .build())
      .fov(45.0f);
    auto camera =
      tenno::shared_ptr<Camera>(camera_builder);

    tenno::vector<std::filesystem::path> skybox_faces = {
      "examples/assets/textures/skybox/right.jpg",
      "examples/assets/textures/skybox/left.jpg",
      "examples/assets/textures/skybox/top.jpg",
      "examples/assets/textures/skybox/bottom.jpg",
      "examples/assets/textures/skybox/front.jpg",
      "examples/assets/textures/skybox/back.jpg",
    };
    auto skybox =
      tenno::make_shared<Skybox>(skybox_faces);
  
    Mouse mouse = {};
    mouse.sensitivity = 0.05f;
  
    // Entities
    init_player_entity();
    init_cube_entity();
    init_floor_entity();
    init_directional_light_entity();
    init_point_light_entity();
    init_sphere_entity();
    init_robot_entity();
    init_camera_entity(camera);
    init_particle_emitter_entity(camera);

    // Callbacks
    init_toggle_wireframe_callback();
    init_close_window_callback();
    init_camera_mouse_callback(camera, &mouse);
    init_play_guitar_callback();

    // Resources
    World::add_resource<WireframeResource>(false);
    World::add_resource<CameraResource>(camera);
    World::add_resource<SkyboxResource>(skybox);

    // Systems
    World::register_systems<CameraRenderSystem,
                            SkyboxSystem,
                            ModelRenderSystem,
                            PointLightRenderSystem,
                            DirLightRenderSystem,
                            ParticleEmitterSystem,
                            SpriteAnimationSystem,
                            DebugTextSystem,
                            PhysicsSystem,
                            InfoTextSystem,
                            CollisionsSystem>();

    auto font_builder =
      Font::Builder()
      .path("examples/assets/fonts/arial.ttf")
      .size(40);
    auto font =
      AssetManager::new_asset<Font>("TextFont", font_builder);

    auto game_fb = tenno::make_shared<FrameBuffer>(Window::get_width(),
                                                   Window::get_height());
    
    auto pipeline = tenno::make_shared<RenderPipeline>();
    pipeline->add_pass<OpaquePass>(game_fb);
    pipeline->add_pass<TransparentPass>(game_fb);
    pipeline->add_pass<SkyboxPass>(game_fb);
    pipeline->add_pass<UiPass>(game_fb);
    
    while (!Window::should_close())
    {
      Window::poll_events();

      Gui::new_frame(game_fb.get(), "demo");
    
      Gl::set_color(Color::grey());
      Gl::clear();

      Renderer::begin_frame();
      World::tick();
      Renderer::end_frame(pipeline);

      Window::framebuffer->bind();
      Gui::render();
      Window::framebuffer->unbind();

      Window::swap_buffers();
    }
  }

  AssetManager::clear();
  engine.terminate();
  return 0;
}
