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

#pragma once

#define MAX_PARTICLES 1000

#include "camera.hpp"
#include "vao.hpp"

#include <glad/glad.h> /* OpenGL driver */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace brenta
{

/**
 * @brief ParticleEmitter class
 *
 * This class is used to define a particle emitter,
 * update and render its particles. This class provides
 * a builder pattern to create a ParticleEmitter object.
 * The particles ban be updated and rendered using the
 * updateParticles and renderParticles methods.
 */
class particle_emitter
{
  public:
    /**
     * @brief Starting position of a new particle
     */
    glm::vec3 starting_position;
    /**
     * @brief Starting velocity of a new particle
     */
    glm::vec3 starting_velocity;
    /**
     * @brief Starting spread of a new particle
     */
    glm::vec3 starting_spread;
    /**
     * @brief Time to live of a new particle
     */
    float starting_time_to_live;
    /**
     * @brief Number of particles
     */
    int num_particles;
    /**
     * @brief Spawn rate of particles
     */
    float spawn_rate;
    /**
     * @brief Scale of particles
     */
    float scale;
    /**
     * @brief Feddback buffer objects
     *
     * These are used to save the new state of updated
     * particles in the updateParticles method.
     */
    types::buffer fbo[2];
    /**
     * @brief Current fbo index
     */
    int current;
    /**
     * @brief Atlas texture
     */
    int atlas;
    /**
     * @brief Atlas width
     */
    int atlas_width;
    /**
     * @brief Atlas height
     */
    int atlas_height;
    /**
     * @brief Atlas index
     */
    int atlas_index;
    /**
     * @brief Vertex array object
     */
    types::vao vao;

    /**
     * @brief Empty constructor
     *
     * Sets default values
     */
    particle_emitter();
    /**
     * @brief Construct a new ParticleEmitter object
     *
     * @param starting_position Starting position of a new particle
     * @param starting_velocity Starting velocity of a new particle
     * @param starting_spread Starting spread of a new particle
     * @param starting_timeToLive Time to live of a new particle
     * @param num_particles Number of particles
     * @param spawn_rate Spawn rate of particles
     * @param scale Scale of particles
     * @param atlas_path Atlas texture path
     * @param atlas_width Atlas width
     * @param atlas_height Atlas height
     * @param atlas_index Atlas index
     * @param camera Camera
     */
    particle_emitter(glm::vec3 starting_position, glm::vec3 starting_velocity,
                     glm::vec3 starting_spread, float starting_timeToLive,
                     int num_particles, float spawn_rate, float scale,
                     std::string atlas_path, int atlas_width, int atlas_height,
                     int atlas_index, camera *cam);
    /**
     * @brief Destroy the ParticleEmitter object
     */
    ~particle_emitter();

    /**
     * @brief The builder pattern for ParticleEmitter
     */
    class builder;

    /**
     * @brief Update the particles
     *
     * @param deltaTime Time passed since last frame
     */
    void update_particles(float deltaTime);
    /**
     * @brief Render the particles
     */
    void render_particles();

  private:
    camera *cam;
    void check_opengl_error(const std::string &functionName);
};

/**
 * @brief Builder pattern for ParticleEmitter
 */
class particle_emitter::builder
{
  private:
    glm::vec3 starting_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 starting_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 starting_spread = glm::vec3(0.0f, 0.0f, 0.0f);
    float starting_time_to_live = 1.0f;
    int num_particles = MAX_PARTICLES;
    float spawn_rate = 0.01f;
    float scale = 1.0f;
    std::string atlas_path = "";
    int atlas_width = 8;
    int atlas_height = 8;
    int atlas_index = 0;
    camera *cam = nullptr;

  public:
    builder &set_starting_position(glm::vec3 starting_position);
    builder &set_starting_velocity(glm::vec3 starting_velocity);
    builder &set_starting_spread(glm::vec3 starting_spread);
    builder &set_starting_time_to_live(float starting_time_to_live);
    builder &set_num_particles(int num_particles);
    builder &set_spawn_rate(float spawn_rate);
    builder &set_scale(float scale);
    builder &set_atlas_path(std::string atlas_path);
    builder &set_atlas_width(int atlas_width);
    builder &set_atlas_height(int atlas_height);
    builder &set_atlas_index(int atlas_index);
    builder &set_camera(camera *cam);

    particle_emitter build();
};

} // namespace brenta
