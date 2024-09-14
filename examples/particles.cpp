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

#include <bitset>
#include <filesystem>

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
    engine eng = engine::builder()
                        .use_screen(true)
                        .use_logger(true)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(false)
                        .set_screen_msaa(true)
                        .set_screen_vsync(true)
                        .set_screen_title("Game")
                        .set_log_level(types::log_level::INFO)
                        .set_log_file("./logs/log.txt")
                        .set_gl_blending(true)
                        .set_gl_cull_face(true)
                        .set_gl_multisample(true)
                        .set_gl_depth_test(true)
                        .build();

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
            .set_camera(&default_camera)
            .set_starting_position(glm::vec3(0.0f, 0.0f, 0.0f))
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
            .set_atlas_index(3)
            .build();

    time::update(screen::get_time());
    while (!screen::is_window_closed())
    {
        if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
            screen::set_close();

        gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
        gl::clear();

        emitter.update_particles(time::get_delta_time());
        emitter.render_particles();

        time::update(screen::get_time());

        screen::poll_events();
        screen::swap_buffers();
    }

    return 0;
}
