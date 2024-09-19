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

#include "particles.hpp"

#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "translation.hpp"

#include <filesystem>
#include <iostream>
#include <time.h>

using namespace brenta;

particle_emitter::particle_emitter()
{
    starting_position = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_spread = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_time_to_live = 1.0f;
    num_particles = MAX_PARTICLES;
    spawn_rate = 0.01f;
    scale = 1.0f;
    atlas = 0;
    atlas_width = 8;
    atlas_height = 8;
    atlas_index = 0;
    current = 0;
    cam = nullptr;
}

particle_emitter::particle_emitter(
    glm::vec3 starting_position, glm::vec3 starting_velocity,
    glm::vec3 starting_spread, float starting_time_to_live, int num_particles,
    float spawn_rate, float scale, std::string atlas_path, int atlas_width,
    int atlas_height, int atlas_index, camera *cam)
{
    this->starting_position = starting_position;
    this->starting_velocity = starting_velocity;
    this->starting_spread = starting_spread;
    this->starting_time_to_live = starting_time_to_live;
    this->num_particles = num_particles;
    this->spawn_rate = spawn_rate;
    this->scale = scale;
    this->atlas = atlas;
    this->atlas_width = atlas_width;
    this->atlas_height = atlas_height;
    this->atlas_index = atlas_index;
    this->current = 0;
    this->cam = cam;

    // Load Texture Atlas
    this->atlas = texture::load_texture(
        atlas_path, GL_REPEAT, GL_NEAREST, GL_NEAREST, GL_TRUE,
        GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST, false);

    // Create shaders
    const GLchar *varyings[] = {"outPosition", "outVelocity", "outTTL"};
    shader::create(
        varyings, 3, "particle_update", GL_VERTEX_SHADER,
        std::filesystem::absolute("engine/shaders/particle_update.vs"));
    shader::create(
        "particle_render", GL_VERTEX_SHADER,
        std::filesystem::absolute("engine/shaders/particle_render.vs").string(),
        GL_GEOMETRY_SHADER,
        std::filesystem::absolute("engine/shaders/particle_render.gs").string(),
        GL_FRAGMENT_SHADER,
        std::filesystem::absolute("engine/shaders/particle_render.fs")
            .string());

    // This is needed to render points
    glEnable(GL_PROGRAM_POINT_SIZE);

    this->vao.init();
    this->vao.bind();
    check_opengl_error("vao bind");

    // Create fbos
    this->fbo[0] = types::buffer(GL_TRANSFORM_FEEDBACK_BUFFER);
    this->fbo[1] = types::buffer(GL_TRANSFORM_FEEDBACK_BUFFER);

    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[0].id);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
                 this->num_particles * 2 * sizeof(glm::vec3)
                     + this->num_particles * sizeof(float),
                 NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[1].id);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
                 this->num_particles * 2 * sizeof(glm::vec3)
                     + this->num_particles * sizeof(float),
                 NULL, GL_DYNAMIC_COPY);
    check_opengl_error("glBindBufferBase A");

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->vao.unbind();
}

particle_emitter::~particle_emitter()
{
    fbo[0].destroy();
    fbo[1].destroy();
    INFO("Deleted ParticleEmitter");
}

// Update particles using Transform Feedback
// directly from the wiki
void particle_emitter::update_particles(float delta_time)
{
    shader::use("particle_update");
    shader::set_float("particle_update", "deltaTime", delta_time);
    shader::set_vec3("particle_update", "emitterPos", this->starting_position);
    shader::set_vec3("particle_update", "emitterSpread", this->starting_spread);
    shader::set_float("particle_update", "spawnProbability", this->spawn_rate);
    shader::set_vec3("particle_update", "emitterVel", this->starting_velocity);
    shader::set_float("particle_update", "emitterTTL",
                      this->starting_time_to_live);
    check_opengl_error("settin update shader");

    this->vao.bind();

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current].id);
    check_opengl_error("glBindBufferBase B");

    glBindBuffer(GL_ARRAY_BUFFER, fbo[!current].id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3) + sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3) + sizeof(float),
                          (void *) sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3) + sizeof(float),
                          (void *) (2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    // Start transform feedback
    glEnable(GL_RASTERIZER_DISCARD);     // Disable rasterization
    glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
    check_opengl_error("glBeginTransformFeedback");

    glDrawArrays(GL_POINTS, 0, num_particles);
    check_opengl_error("glDrawTransformFeedback");

    glEndTransformFeedback();         // Exit transform feedback mode
    glDisable(GL_RASTERIZER_DISCARD); // Enable rasterization
    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    this->vao.unbind();
    current = !current; // Swap buffers
}

// Render particles
void particle_emitter::render_particles()
{
    if (this->cam == nullptr)
    {
        ERROR("Camera not set or null for ParticleEmitter");
        return;
    }

    shader::use("particle_render");

    this->vao.bind();

    glBindBuffer(GL_ARRAY_BUFFER, fbo[current].id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3) + sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE,
                          2 * sizeof(glm::vec3) + sizeof(float),
                          (void *) (2 * sizeof(glm::vec3)));
    check_opengl_error("glVertexAttribPointer");
    glEnableVertexAttribArray(1);

    // Set uniforms
    types::translation t = types::translation();
    t.set_view(this->cam->get_view_matrix());
    t.set_projection(this->cam->get_projection_matrix());
    t.set_model(glm::mat4(1.0f));
    t.set_shader("particle_render");
    shader::set_int("particle_render", "atlas_width", this->atlas_width);
    shader::set_int("particle_render", "atlas_height", this->atlas_height);
    shader::set_int("particle_render", "atlas_index", this->atlas_index);
    shader::set_float("particle_render", "scale", this->scale);
    shader::set_float("particle_render", "aspect_ratio",
                      (float) screen::get_width()
                          / (float) screen::get_height());

    // Set Textures
    texture::active_texture(GL_TEXTURE0);
    texture::bind_texture(GL_TEXTURE_2D, this->atlas, GL_REPEAT, GL_NEAREST,
                          GL_NEAREST, GL_TRUE, GL_NEAREST_MIPMAP_NEAREST,
                          GL_NEAREST);

    glDrawArrays(GL_POINTS, 0, num_particles);
    check_opengl_error("glDrawArrays");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vao.unbind();
}

void particle_emitter::check_opengl_error(const std::string &function_name)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after " << function_name << ": " << error
                  << std::endl;
    }
}

particle_emitter::builder &
particle_emitter::builder::set_starting_position(glm::vec3 starting_position)
{
    this->starting_position = starting_position;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_starting_velocity(glm::vec3 starting_velocity)
{
    this->starting_velocity = starting_velocity;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_starting_spread(glm::vec3 starting_spread)
{
    this->starting_spread = starting_spread;
    return *this;
}

particle_emitter::builder &particle_emitter::builder::set_starting_time_to_live(
    float starting_time_to_live)
{
    this->starting_time_to_live = starting_time_to_live;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_num_particles(int num_particles)
{
    this->num_particles = num_particles;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_spawn_rate(float spawn_rate)
{
    this->spawn_rate = spawn_rate;
    return *this;
}

particle_emitter::builder &particle_emitter::builder::set_scale(float scale)
{
    this->scale = scale;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_atlas_path(std::string atlas_path)
{
    this->atlas_path = atlas_path;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_atlas_width(int atlas_width)
{
    this->atlas_width = atlas_width;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_atlas_height(int atlas_height)
{
    this->atlas_height = atlas_height;
    return *this;
}

particle_emitter::builder &
particle_emitter::builder::set_atlas_index(int atlas_index)
{
    this->atlas_index = atlas_index;
    return *this;
}

particle_emitter::builder &particle_emitter::builder::set_camera(camera *cam)
{
    this->cam = cam;
    return *this;
}

particle_emitter particle_emitter::builder::build()
{
    /* C++17 has RVO (Return Value Optimization) so move is implicit */
    return particle_emitter(this->starting_position, this->starting_velocity,
                            this->starting_spread, this->starting_time_to_live,
                            this->num_particles, this->spawn_rate, this->scale,
                            this->atlas_path, this->atlas_width,
                            this->atlas_height, this->atlas_index, this->cam);
}
