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

#pragma once

#define MAX_PARTICLES 1000

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>       /* OpenGL driver */
#include <string>
#include <vector>

#include "vao.h"

namespace ECS
{

class ParticleEmitter
{
public:
    /* Starting position, velocity and time to live
     * of a new particle */
    glm::vec3 starting_position;
    glm::vec3 starting_velocity;
    /* Determines how spread apart are the particles */
    glm::vec3 starting_spread;
    /* Time to live of a new particle mesured in seconds */
    float starting_timeToLive;
    /* This number must be less then MAX_PARTICLES */
    int num_particles;
    /* The rate at which particles are spawned */
    float spawn_rate;
    /* The scale in size of the particles */
    float scale;
    /* Framebuffer objects, used to save the state
     * after the update phase */
    GLuint fbo[2];
    /* Used to mark the current fbo for reading */
    int current;
    int atlas;
    int atlas_width;
    int atlas_height;
    /* Index of the texture atlas to draw */
    int atlas_index;
    Types::VAO vao;

    ParticleEmitter();
    ParticleEmitter(glm::vec3 starting_position,
                    glm::vec3 starting_velocity,
                    glm::vec3 starting_spread,
                    float starting_timeToLive,
                    int num_particles,
                    float spawn_rate,
                    float scale,
                    std::string atlas_path,
                    int atlas_width,
                    int atlas_height,
                    int atlas_index);
    ~ParticleEmitter();
    
    class Builder;

    /* Builder pattern */

    /**
     * Update particles using Transform Feedback
     */
    void updateParticles(float deltaTime);

    void renderParticles();

private:
    void checkOpenGLError(const std::string& functionName);
};

/**
 * Builder pattern for ParticleEmitter
 */
class ParticleEmitter::Builder
{
private:
    glm::vec3 starting_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 starting_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 starting_spread = glm::vec3(0.0f, 0.0f, 0.0f);
    float starting_timeToLive = 1.0f;
    int num_particles = MAX_PARTICLES;
    float spawn_rate = 0.01f;
    float scale = 1.0f;
    std::string atlas_path = "";
    int atlas_width = 8;
    int atlas_height = 8;
    int atlas_index = 0;

public:
    Builder& set_starting_position(glm::vec3 starting_position);
    Builder& set_starting_velocity(glm::vec3 starting_velocity);
    Builder& set_starting_spread(glm::vec3 starting_spread);
    Builder& set_starting_timeToLive(float starting_timeToLive);
    Builder& set_num_particles(int num_particles);
    Builder& set_spawn_rate(float spawn_rate);
    Builder& set_scale(float scale);
    Builder& set_atlas_path(std::string atlas_path);
    Builder& set_atlas_width(int atlas_width);
    Builder& set_atlas_height(int atlas_height);
    Builder& set_atlas_index(int atlas_index);

    ParticleEmitter build();
};

} // namespace ECS
