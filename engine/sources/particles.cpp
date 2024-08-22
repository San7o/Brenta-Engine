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
 * The above copyright notice and this permission notice shall be included in all
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

#include <iostream>
#include <filesystem>
#include <time.h>
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "translation.hpp"

using namespace ECS;

ParticleEmitter::ParticleEmitter()
{
    starting_position = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_spread   = glm::vec3(0.0f, 0.0f, 0.0f);
    starting_timeToLive = 1.0f;
    num_particles = MAX_PARTICLES;
    spawn_rate = 0.01f;
    scale = 1.0f;
    atlas = 0;
    atlas_width = 8;
    atlas_height = 8;
    atlas_index = 0;
    current = 0;
    camera = nullptr;
}

ParticleEmitter::ParticleEmitter(glm::vec3 starting_position,
                                 glm::vec3 starting_velocity,
                                 glm::vec3 starting_spread,
                                 float starting_timeToLive,
                                 int num_particles,
                                 float spawn_rate,
                                 float scale,
                                 std::string atlas_path,
                                 int atlas_width,
                                 int atlas_height,
                                 int atlas_index,
                                 Camera* camera)
{
    this->starting_position = starting_position;
    this->starting_velocity = starting_velocity;
    this->starting_spread = starting_spread;
    this->starting_timeToLive = starting_timeToLive;
    this->num_particles = num_particles;
    this->spawn_rate = spawn_rate;
    this->scale = scale;
    this->atlas = atlas;
    this->atlas_width = atlas_width;
    this->atlas_height = atlas_height;
    this->atlas_index = atlas_index;
    this->current = 0;
    this->camera = camera;

    // Load Texture Atlas
    this->atlas = ECS::Texture::LoadTexture(atlas_path,
                    GL_REPEAT,
                    GL_NEAREST,
                    GL_NEAREST,
                    GL_TRUE,
                    GL_NEAREST_MIPMAP_NEAREST,
                    GL_NEAREST,
                    false);

    // Create shaders
    const GLchar* varyings[] = { "outPosition", "outVelocity", "outTTL" };
    Shader::New(
       varyings,
       3,
       "particle_update",
       GL_VERTEX_SHADER,
       std::filesystem::absolute("game/shaders/particle_update.vs")
    );
    Shader::New(
        "particle_render",
        GL_VERTEX_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.vs").string(),
        GL_GEOMETRY_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.gs").string(),
        GL_FRAGMENT_SHADER,
        std::filesystem::absolute("game/shaders/particle_render.fs").string()
    );
    
    // This is needed to render points
    glEnable(GL_PROGRAM_POINT_SIZE);

    this->vao.Init();
    this->vao.Bind();
    checkOpenGLError("vao bind");

    // Create fbos
    this->fbo[0] = Types::Buffer(GL_TRANSFORM_FEEDBACK_BUFFER);
    this->fbo[1] = Types::Buffer(GL_TRANSFORM_FEEDBACK_BUFFER);

    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[0].id);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
                    this->num_particles * 2 * sizeof(glm::vec3) + this->num_particles * sizeof(float),
                    NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->fbo[1].id);
    glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER,
                    this->num_particles * 2 * sizeof(glm::vec3) + this->num_particles * sizeof(float),
                    NULL, GL_DYNAMIC_COPY);
    checkOpenGLError("glBindBufferBase A");

    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->vao.Unbind();
}

ParticleEmitter::~ParticleEmitter()
{
    fbo[0].Delete();
    fbo[1].Delete();
    Logger::Log(Types::LogLevel::INFO, "Deleted ParticleEmitter");
}

// Update particles using Transform Feedback
// directly from the wiki
void ParticleEmitter::updateParticles(float deltaTime)
{

    Shader::Use("particle_update");
    Shader::SetFloat("particle_update", "deltaTime", deltaTime);
    Shader::SetVec3("particle_update", "emitterPos", this->starting_position);
    Shader::SetVec3("particle_update", "emitterSpread", this->starting_spread);
    Shader::SetFloat("particle_update", "spawnProbability", this->spawn_rate);
    Shader::SetVec3("particle_update", "emitterVel", this->starting_velocity);
    Shader::SetFloat("particle_update", "emitterTTL", this->starting_timeToLive);
    checkOpenGLError("settin update shader");

    this->vao.Bind();

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current].id);
    checkOpenGLError("glBindBufferBase B");

    glBindBuffer(GL_ARRAY_BUFFER, fbo[!current].id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                    2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                    2 * sizeof(glm::vec3) + sizeof(float), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
                    2 * sizeof(glm::vec3) + sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(2);

    // Start transform feedback
    glEnable(GL_RASTERIZER_DISCARD);  // Disable rasterization
    glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
    checkOpenGLError("glBeginTransformFeedback");

    glDrawArrays(GL_POINTS, 0, num_particles);
    checkOpenGLError("glDrawTransformFeedback");

    glEndTransformFeedback(); // Exit transform feedback mode
    glDisable(GL_RASTERIZER_DISCARD);  // Enable rasterization
    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
    this->vao.Unbind();
    current = !current; // Swap buffers
}

// Render particles
void ParticleEmitter::renderParticles()
{
    if (camera == nullptr)
    {
        Logger::Log(Types::LogLevel::ERROR, "Camera not set or null for ParticleEmitter");
        return;
    }

    Shader::Use("particle_render");

    this->vao.Bind();

    glBindBuffer(GL_ARRAY_BUFFER, fbo[current].id);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3) + sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(float), (void*)(2 * sizeof(glm::vec3)));
    checkOpenGLError("glVertexAttribPointer");
    glEnableVertexAttribArray(1);

    // Set uniforms
    Types::Translation t = Types::Translation();
    t.setView(camera->GetViewMatrix());
    t.setProjection(camera->GetProjectionMatrix());
    t.setModel(glm::mat4(1.0f));
    t.setShader("particle_render");
    Shader::SetInt("particle_render", "atlas_width", this->atlas_width);
    Shader::SetInt("particle_render", "atlas_height", this->atlas_height);
    Shader::SetInt("particle_render", "atlas_index", this->atlas_index);
    Shader::SetFloat("particle_render", "scale", this->scale);

    // Set Textures
    ECS::Texture::ActiveTexture(GL_TEXTURE0);
    ECS::Texture::BindTexture(GL_TEXTURE_2D, this->atlas,
                    GL_REPEAT,
                    GL_NEAREST,
                    GL_NEAREST,
                    GL_TRUE,
                    GL_NEAREST_MIPMAP_NEAREST,
                    GL_NEAREST);

    glDrawArrays(GL_POINTS, 0, num_particles);
    checkOpenGLError("glDrawArrays");
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    vao.Unbind();
}

void ParticleEmitter::checkOpenGLError(const std::string& functionName)
{
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after " << functionName << ": " << error << std::endl;
    }
}


ParticleEmitter::Builder& ParticleEmitter::Builder::set_starting_position(glm::vec3 starting_position)
{
    this->starting_position = starting_position;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_starting_velocity(glm::vec3 starting_velocity)
{
    this->starting_velocity = starting_velocity;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_starting_spread(glm::vec3 starting_spread)
{
    this->starting_spread = starting_spread;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_starting_timeToLive(float starting_timeToLive)
{
    this->starting_timeToLive = starting_timeToLive;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_num_particles(int num_particles)
{
    this->num_particles = num_particles;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_spawn_rate(float spawn_rate)
{
    this->spawn_rate = spawn_rate;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_scale(float scale)
{
    this->scale = scale;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_atlas_path(std::string atlas_path)
{
    this->atlas_path = atlas_path;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_atlas_width(int atlas_width)
{
    this->atlas_width = atlas_width;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_atlas_height(int atlas_height)
{
    this->atlas_height = atlas_height;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_atlas_index(int atlas_index)
{
    this->atlas_index = atlas_index;
    return *this;
}

ParticleEmitter::Builder& ParticleEmitter::Builder::set_camera(Camera* camera)
{
    this->camera = camera;
    return *this;
}

ParticleEmitter ParticleEmitter::Builder::build()
{
    /* C++17 has RVO (Return Value Optimization) so move is implicit */
    return ParticleEmitter(this->starting_position,
                           this->starting_velocity,
                           this->starting_spread,
                           this->starting_timeToLive,
                           this->num_particles,
                           this->spawn_rate,
                           this->scale,
                           this->atlas_path,
                           this->atlas_width,
                           this->atlas_height,
                           this->atlas_index,
                           this->camera);
}
