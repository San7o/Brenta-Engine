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

/**
 * Just a simple window
 */

#include "ecs.hpp"
#include "engine.hpp"

#include <iostream>
#include <filesystem>

using namespace brenta;
using namespace brenta::types;
using namespace brenta::ecs;

const int SCR_WIDTH = 1280;
const int SCR_HEIGHT = 720;

struct transform_component : component
{
    glm::vec3 position;
    glm::vec3 rotation;
    float scale;

    transform_component()
        : position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), scale(1.0f)
    {
    }
    transform_component(glm::vec3 position, glm::vec3 rotation, float scale)
        : position(position), rotation(rotation), scale(scale)
    {
    }
};

struct model_component : component
{
    model mod;
    float shininess;
    brenta::types::shader_name_t shader;
    bool hasAtlas;
    int atlasSize;
    int atlasIndex;
    int elapsedFrames = 0;

    model_component()
        : mod(model()), shininess(0.0f), shader("default_shader"),
          hasAtlas(false), atlasSize(0), atlasIndex(0)
    {
    }
    model_component(model mod, float shininess, brenta::types::shader_name_t shader)
        : mod(mod), shininess(shininess), shader(shader), hasAtlas(false),
          atlasSize(0), atlasIndex(0)
    {
    }
};

struct renderer_system : system<model_component, transform_component>
{
    void run(std::vector<entity_t> matches) const override
    {
        if (matches.empty())
            return;

        for (auto match : matches)
        {
            /* Get the model component */
            auto model_c =
                world::entity_to_component<model_component>(match);

            auto transform_c =
                world::entity_to_component<transform_component>(match);

            auto my_model = model_c->mod;
            auto default_shader = model_c->shader;

            brenta::types::translation t = brenta::types::translation();
            t.set_view(default_camera.get_view_matrix());
            t.set_projection(default_camera.get_projection_matrix());

            t.set_model(glm::mat4(1.0f));
            t.translate(transform_c->position);
            t.rotate(transform_c->rotation);
            t.scale(transform_c->scale);

            t.set_shader(default_shader);

            shader::set_vec3(default_shader, "viewPos", default_camera.get_position());
            shader::set_float(default_shader, "material.shininess",
                             model_c->shininess);

            shader::set_int(default_shader, "atlasIndex", 0);
            my_model.draw(default_shader);
        }
    }
};

REGISTER_SYSTEMS(renderer_system);

namespace brenta
{
    camera default_camera;
}

int main()
{
    engine eng = engine::builder()
                        .use_screen(true)
                        .use_logger(true)
                        .set_log_level(brenta::types::log_level::DEBUG)
                        .set_screen_width(SCR_WIDTH)
                        .set_screen_height(SCR_HEIGHT)
                        .set_screen_is_mouse_captured(false)
                        .build();

    camera default_camera = camera::builder()
                        .set_camera_type(enums::camera_type::AIRCRAFT)
                        .set_projection_type(enums::projection_type::PERSPECTIVE)
                        .set_position(glm::vec3(0.0f, 5.0f, 20.0f))
                        .set_up(glm::vec3(0.0f, 1.0f, 0.0f))
                        .set_euler_angles(euler_angles(-90.0f, 0.0f, 0.0f))
                        .build();

    // A square
    float vertices[] = {
        // First Triangle
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,

        // Second Triangle
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };
    vao v;
    v.init();
    buffer vbo = buffer(GL_ARRAY_BUFFER);
    vbo.copy_data(sizeof(vertices), vertices, GL_STATIC_DRAW);
    v.set_vertex_data(vbo, 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    vbo.unbind();
    v.unbind();

    shader::create("hdr_shader", 
                 GL_VERTEX_SHADER,
                 "examples/hdr.vs",
                 GL_FRAGMENT_SHADER,
                 "examples/hdr.fs");

#ifdef USE_IMGUI
    brenta::types::framebuffer fb(SCR_WIDTH, SCR_HEIGHT, GL_RGBA16F);
#endif

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
            .set_atlas_index(5)
            .build();

    // Room ------------------------------------
    auto room_entity = world::new_entity();
    world::add_component<transform_component>(room_entity, transform_component());
    if (shader::get_id("default_shader") == 0)
    {
        shader::create("default_shader", GL_VERTEX_SHADER,
                    std::filesystem::absolute("examples/default_shader.vs"),
                    GL_FRAGMENT_SHADER,
                    std::filesystem::absolute("examples/default_shader.fs"));
    }
    model mod(
        std::filesystem::absolute("assets/models/sphere/sphere.obj"));
    auto model_c = model_component(mod, 32.0f, "default_shader");
    world::add_component<model_component>(room_entity,
                                        std::move(model_c));
    INFO("Room entity created");
    // -----------------------------------------

    time::update(screen::get_time());
    while (!screen::is_window_closed())
    {
        screen::poll_events();
        if (screen::is_key_pressed(GLFW_KEY_ESCAPE))
            screen::set_close();

#ifdef USE_IMGUI
        gui::new_frame(&fb);
        ImGui::Begin("HDR");
        ImGui::End();
#endif
        fb.bind();

        gl::set_color(0.2f, 0.2f, 0.207f, 1.0f);
        gl::clear();

        // Our scene here

        shader::use("hdr_shader");

        emitter.update_particles(time::get_delta_time());
        emitter.render_particles();

        time::update(screen::get_time());
        world::tick();

        //vao.Bind();
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glCheckError();
        //vao.Unbind();

        fb.unbind();
#ifdef USE_IMGUI
        gui::render();
#endif

        screen::swap_buffers();
    }

    return 0;
}
