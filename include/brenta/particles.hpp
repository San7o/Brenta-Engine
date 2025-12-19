// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#define MAX_PARTICLES 1000

#include <brenta/camera.hpp>
#include <brenta/vao.hpp>
#include <glad/glad.h>
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
 * This class is used to define a particle emitter, update and render
 * its particles. This class provides a builder pattern to create a
 * ParticleEmitter object.  The particles ban be updated and rendered
 * using the updateParticles and renderParticles methods.
 */
class particle_emitter
{
public:
  
  glm::vec3 starting_position;
  glm::vec3 starting_velocity;
  glm::vec3 starting_spread;
  float starting_time_to_live;
  int num_particles;
  float spawn_rate;
  float scale;
  /**
   * @brief Feddback buffer objects
   *
   * These are used to save the new state of updated particles in the
   * updateParticles method.
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
  int atlas_width;
  int atlas_height;
  int atlas_index;
  /**
   * @brief Vertex array object
   */
  types::vao vao;

  class config;
  class builder;

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
  particle_emitter(particle_emitter::config conf); // TODO
  ~particle_emitter();

  /**
   * @brief Update the particles
   *
   * @param deltaTime Time passed since last frame
   */
  void update_particles(float deltaTime);
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
  
  glm::vec3 starting_position_val = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 starting_velocity_val = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 starting_spread_val = glm::vec3(0.0f, 0.0f, 0.0f);
  float starting_time_to_live_val = 1.0f;
  int num_particles_val = MAX_PARTICLES;
  float spawn_rate_val = 0.01f;
  float scale_val = 1.0f;
  std::string atlas_path_val = "";
  int atlas_width_val = 8;
  int atlas_height_val = 8;
  int atlas_index_val = 0;
  camera *cam_val = nullptr;

public:
  builder &starting_position(glm::vec3 starting_position);
  builder &starting_velocity(glm::vec3 starting_velocity);
  builder &starting_spread(glm::vec3 starting_spread);
  builder &starting_time_to_live(float starting_time_to_live);
  builder &num_particles(int num_particles);
  builder &spawn_rate(float spawn_rate);
  builder &scale(float scale);
  builder &atlas_path(std::string atlas_path);
  builder &atlas_width(int atlas_width);
  builder &atlas_height(int atlas_height);
  builder &atlas_index(int atlas_index);
  builder &with_camera(camera *cam);

  particle_emitter build();
};

} // namespace brenta
