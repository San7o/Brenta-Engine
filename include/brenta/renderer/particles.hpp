// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#ifndef BRENTA_MAX_PARTICLES
  #define BRENTA_MAX_PARTICLES 1000
#endif

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/opengl/ubo.hpp>
#include <brenta/renderer/opengl/vao.hpp>

#include <glm/vec3.hpp>

#include <filesystem>

namespace brenta
{

class Camera;
  
//
// Particle Emitter class
// ----------------------
//
// This class is used to define a particle emitter, update and render
// its particles. This class provides a builder pattern to create a
// ParticleEmitter object.  The particles ban be updated and rendered
// using the updateParticles and renderParticles methods.
//
class ParticleEmitter
{
public:
  
  struct Config;
  class  Builder;

  // Particle variables
  glm::vec3 starting_position;
  glm::vec3 starting_velocity;
  glm::vec3 starting_spread;
  float     starting_time_to_live;
  int       num_particles;
  float     spawn_rate;
  float     scale;

  Vao     vao;
  // We have two fbo which get swapped when rendered
  // Feedback buffer
  Fbo     fbo[2];
  int     current_fbo_index;
  Ubo     ubo;
  
  Texture atlas;
  int     atlas_width;
  int     atlas_height;
  int     atlas_index;

  ParticleEmitter(Config conf);
  ParticleEmitter(ParticleEmitter&&) = default;

  ParticleEmitter(const ParticleEmitter&)            = delete;
  ParticleEmitter &operator=(const ParticleEmitter&) = delete;
  
  ~ParticleEmitter() = default;

  void update(float delta_time);
  void render(int width, int height);

private:
  
  tenno::weak_ptr<Camera>     cam;
  tenno::shared_ptr<Shader>   shader_update;
  tenno::shared_ptr<Shader>   shader_render;

};

struct ParticleEmitter::Config
{
  glm::vec3               starting_position     = {};
  glm::vec3               starting_velocity     = {};
  glm::vec3               starting_spread       = {};
  float                   starting_time_to_live = 1.0f;
  int                     num_particles         = BRENTA_MAX_PARTICLES;
  float                   spawn_rate            = 0.99f;
  float                   scale                 = 1.0f;
  std::filesystem::path   atlas_path            = "";
  int                     atlas_width           = 8;
  int                     atlas_height          = 8;
  int                     atlas_index           = 0;
  tenno::weak_ptr<Camera> cam                   = {};
};
  
class ParticleEmitter::Builder
{
private:

  ParticleEmitter::Config conf = {};

public:
  
  Builder &starting_position(glm::vec3 starting_position);
  Builder &starting_velocity(glm::vec3 starting_velocity);
  Builder &starting_spread(glm::vec3 starting_spread);
  Builder &starting_time_to_live(float starting_time_to_live);
  Builder &num_particles(int num_particles);
  Builder &spawn_rate(float spawn_rate);
  Builder &scale(float scale);
  Builder &atlas_path(const std::filesystem::path &atlas_path);
  Builder &atlas_width(int atlas_width);
  Builder &atlas_height(int atlas_height);
  Builder &atlas_index(int atlas_index);
  Builder &with_camera(tenno::weak_ptr<Camera> cam);

  ParticleEmitter build();
};

} // namespace brenta
