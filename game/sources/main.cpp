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

#include "ecs.hpp"
#include "engine.hpp"
#include "game_ecs.hpp"

#include <bitset>
#include <filesystem>

using namespace Brenta;
using namespace Brenta::ECS;

// Default resolution
const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

REGISTER_SYSTEMS(RendererSystem, DebugTextSystem, PointLightsSystem,
                 DirectionalLightSystem, PhysicsSystem, CollisionsSystem);

/* default camera */
namespace Brenta
{
Camera camera = Camera();
}

int main()
{
    Engine engine = Engine::Builder()
                        .use_screen(true)
                        .use_audio(true)
                        .use_input(true)
                        .use_logger(true)
                        .use_text(true)
                        .use_ecs(true)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(false)
                        .set_screen_msaa(true)
                        .set_screen_vsync(true)
                        .set_screen_title("Game")
                        .set_log_level(Brenta::Types::LogLevel::INFO)
                        .set_log_file("./logs/log.txt")
                        .set_text_font("arial.ttf")
                        .set_text_size(24)
                        .set_gl_blending(true)
                        .set_gl_cull_face(true)
                        .set_gl_multisample(true)
                        .set_gl_depth_test(true)
                        .build();

    camera = Camera::Builder()
                 .set_camera_type(Enums::CameraType::SPHERICAL)
                 .set_projection_type(Enums::ProjectionType::PERSPECTIVE)
                 .set_spherical_coordinates({1.25f, 1.25f, 30.0f})
                 .set_center(glm::vec3(0.0f, 2.0f, 0.0f))
                 .set_movement_speed(2.5f)
                 .set_mouse_sensitivity(0.05f)
                 .set_zoom(45.0f)
                 .build();

    InitPlayerEntity();
    // InitCubeEntity();
    InitFloorEntity();
    InitDirectionalLightEntity();
    InitPointLightEntity();
    InitSphereEntity();
    InitRobotEntity();

    InitToggleWireframeCallback();
    InitCloseWindowCallback();
    InitCameraMouseCallback();
    InitPlayGuitarCallback();

    World::AddResource<WireframeResource>(WireframeResource(false));

    Audio::LoadAudio("guitar",
                     std::filesystem::absolute("assets/audio/guitar.wav"));

    ParticleEmitter emitter =
        ParticleEmitter::Builder()
            .set_camera(&camera)
            .set_starting_position(glm::vec3(0.0f, 0.0f, 5.0f))
            .set_starting_velocity(glm::vec3(0.0f, 5.0f, 0.0f))
            .set_starting_spread(glm::vec3(3.0f, 10.0f, 3.0f))
            .set_starting_timeToLive(0.5f)
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

    Time::Update(Screen::GetTime());
    while (!Screen::isWindowClosed())
    {
        GL::SetColor(0.2f, 0.2f, 0.207f, 1.0f);
        GL::Clear();

        emitter.updateParticles(Time::GetDeltaTime());
        emitter.renderParticles();

        Time::Update(Screen::GetTime());
        World::Tick();

        Screen::PollEvents();
        Screen::SwapBuffers();
    }

    return 0;
}
