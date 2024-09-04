/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "engine.hpp"
#ifdef USE_ECS
#include "ecs.hpp"
#include "game_ecs.hpp"
#endif

#include <bitset>
#include <filesystem>

using namespace brenta;
#ifdef USE_ECS
using namespace brenta::ecs;
#endif

// Default resolution
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

#ifdef USE_ECS
REGISTER_SYSTEMS(RendererSystem, PointLightsSystem, // DebugTextSystem,
                 DirectionalLightSystem, PhysicsSystem, CollisionsSystem);
#endif

/* default camera */
namespace brenta
{
camera default_camera = camera();
}

int main()
{
    engine eng = engine::builder()
                        .use_screen(true)
                        .use_audio(true)
                        .use_input(true)
                        .use_logger(true)
                        .use_text(true)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(false)
                        .set_screen_msaa(true)
                        .set_screen_vsync(true)
                        .set_screen_title("Game")
                        .set_log_level(brenta::types::log_level::INFO)
                        .set_log_file("./logs/log.txt")
                        .set_text_font("arial.ttf")
                        .set_text_size(24)
                        .set_gl_blending(true)
                        .set_gl_cull_face(true)
                        .set_gl_multisample(true)
                        .set_gl_depth_test(true)
                        .build();

    default_camera = camera::builder()
                 .set_camera_type(enums::camera_type::SPHERICAL)
                 .set_projection_type(enums::projection_type::PERSPECTIVE)
                 .set_spherical_coordinates({1.25f, 1.25f, 30.0f})
                 .set_center(glm::vec3(0.0f, 2.0f, 0.0f))
                 .set_movement_speed(2.5f)
                 .set_mouse_sensitivity(0.05f)
                 .set_zoom(45.0f)
                 .build();

#ifdef USE_ECS
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

    world::add_resource<WireframeResource>(WireframeResource(false));
#endif

    audio::load_audio("guitar",
                     std::filesystem::absolute("assets/audio/guitar.wav"));

    particle_emitter emitter =
        particle_emitter::builder()
            .set_camera(&default_camera)
            .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
            .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
            .set_starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
            .set_starting_time_to_live(0.5f)
            .set_num_particles(1000)
            .set_spawn_rate(0.01f)
            .set_scale(1.0f)
            .set_atlas_path(
                std::filesystem::absolute("assets/textures/particle_atlas.png")
                    .string())
            .set_atlas_width(8)
            .set_atlas_height(8)
            .set_atlas_index(5)
            .build();

#ifdef USE_IMGUI
    brenta::types::framebuffer fb(SCR_WIDTH, SCR_HEIGHT);
#endif

    time::update(screen::get_time());
    while (!screen::is_window_closed())
    {
        screen::poll_events();

#ifdef USE_IMGUI
        gui::new_frame(&fb);
        fb.bind();
#endif

        gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
        gl::clear();

        emitter.update_particles(time::get_delta_time());
        emitter.render_particles();

#ifdef USE_ECS
        time::update(screen::get_time());
        world::tick();
#endif

#ifdef USE_IMGUI
        fb.unbind();
        gui::render();
#endif
        screen::swap_buffers();
    }

    return 0;
}
