// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/translation.hpp>
#include <brenta/window.hpp>

#include <iostream>
#include <time.h>

using namespace brenta;

const ParticleEmitter::Config ParticleEmitter::default_config = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 0.0f, 0.0f),
  1.0f,
  MAX_PARTICLES,
  0.01f,
  1.0f,
  "",
  8,
  8,
  0,
  nullptr,
};

ParticleEmitter::ParticleEmitter(Config conf)
{
  this->starting_position = conf.starting_position;
  this->starting_velocity = conf.starting_velocity;
  this->starting_spread = conf.starting_spread;
  this->starting_time_to_live = conf.starting_time_to_live;
  this->num_particles = conf.num_particles;
  this->spawn_rate = conf.spawn_rate;
  this->scale = conf.scale;
  this->atlas_width = conf.atlas_width;
  this->atlas_height = conf.atlas_height;
  this->atlas_index = conf.atlas_index;
  this->current = 0;
  this->cam = conf.cam;

  // Load Texture Atlas
  this->atlas = std::move(Texture(conf.atlas_path, false));

  // Create shaders
  const GLchar *varyings[] = {"outPosition", "outVelocity", "outTTL"};
  Shader::create(varyings, 3, "particle_update",
                 Shader::Type::Vertex, "src/renderer/shaders/particle_update.vs");
  Shader::create("particle_render",
                 Shader::Type::Vertex,   "src/renderer/shaders/particle_render.vs",
                 Shader::Type::Geometry, "src/renderer/shaders/particle_render.gs",
                 Shader::Type::Fragment, "src/renderer/shaders/particle_render.fs");

  // This is needed to render points
  glEnable(GL_PROGRAM_POINT_SIZE);

  this->vao.init();
  this->vao.bind();
  Gl::check_error();

  // Create fbos
  this->fbo[0].init(GL_TRANSFORM_FEEDBACK_BUFFER);
  this->fbo[1].init(GL_TRANSFORM_FEEDBACK_BUFFER);

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
  Gl::check_error();

  // Unbind buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  this->vao.unbind();
}

// Update particles using Transform Feedback
void ParticleEmitter::update(float delta_time)
{
  Shader::use("particle_update");
  Shader::set_float("particle_update", "deltaTime", delta_time);
  Shader::set_vec3("particle_update", "emitterPos", this->starting_position);
  Shader::set_vec3("particle_update", "emitterSpread", this->starting_spread);
  Shader::set_float("particle_update", "spawnProbability", this->spawn_rate);
  Shader::set_vec3("particle_update", "emitterVel", this->starting_velocity);
  Shader::set_float("particle_update", "emitterTTL",
                    this->starting_time_to_live);
  Gl::check_error();

  this->vao.bind();

  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current].id);
  Gl::check_error();

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
  Gl::check_error();

  glDrawArrays(GL_POINTS, 0, num_particles);
  Gl::check_error();

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
void ParticleEmitter::render()
{
  if (!this->cam)
  {
    ERROR("ParticleEmitter::render_particles: Camera not set or null for emitter");
    return;
  }

  Shader::use("particle_render");

  this->vao.bind();

  glBindBuffer(GL_ARRAY_BUFFER, fbo[current].id);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float),
                        (void *) (2 * sizeof(glm::vec3)));
  Gl::check_error();
  glEnableVertexAttribArray(1);

  // Set uniforms
  int window_width = Window::get_width();
  int window_height = Window::get_height();
  Translation t = Translation();
  t.set_view(this->cam->get_view_matrix());
  t.set_projection(this->cam->get_projection_matrix(window_width, window_height));
  t.set_model(glm::mat4(1.0f));
  t.set_shader("particle_render");
  Shader::set_int("particle_render", "atlas_width", this->atlas_width);
  Shader::set_int("particle_render", "atlas_height", this->atlas_height);
  Shader::set_int("particle_render", "atlas_index", this->atlas_index);
  Shader::set_float("particle_render", "scale", this->scale);
  Shader::set_float("particle_render", "aspect_ratio",
                    (float) window_width
                    / (float) window_height);

  // Set Textures
  Texture::active_texture(GL_TEXTURE0);
  this->atlas.bind(GL_TEXTURE_2D, GL_REPEAT, GL_NEAREST,
                   GL_NEAREST, GL_TRUE, GL_NEAREST_MIPMAP_NEAREST,
                   GL_NEAREST);

  glDrawArrays(GL_POINTS, 0, num_particles);
  Gl::check_error();

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  vao.unbind();
  return;
}

//
// Builder functions
//

ParticleEmitter::Builder &
ParticleEmitter::Builder::starting_position(glm::vec3 starting_position)
{
  this->conf.starting_position = starting_position;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::starting_velocity(glm::vec3 starting_velocity)
{
  this->conf.starting_velocity = starting_velocity;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::starting_spread(glm::vec3 starting_spread)
{
  this->conf.starting_spread = starting_spread;
  return *this;
}

ParticleEmitter::Builder &ParticleEmitter::Builder::starting_time_to_live(
  float starting_time_to_live)
{
  this->conf.starting_time_to_live = starting_time_to_live;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::num_particles(int num_particles)
{
  this->conf.num_particles = num_particles;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::spawn_rate(float spawn_rate)
{
  this->conf.spawn_rate = spawn_rate;
  return *this;
}

ParticleEmitter::Builder &ParticleEmitter::Builder::scale(float scale)
{
  this->conf.scale = scale;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::atlas_path(const std::filesystem::path &atlas_path)
{
  this->conf.atlas_path = atlas_path;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::atlas_width(int atlas_width)
{
  this->conf.atlas_width = atlas_width;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::atlas_height(int atlas_height)
{
  this->conf.atlas_height = atlas_height;
  return *this;
}

ParticleEmitter::Builder &
ParticleEmitter::Builder::atlas_index(int atlas_index)
{
  this->conf.atlas_index = atlas_index;
  return *this;
}

ParticleEmitter::Builder &ParticleEmitter::Builder::with_camera(Camera *cam)
{
  this->conf.cam = cam;
  return *this;
}

ParticleEmitter ParticleEmitter::Builder::build()
{
  return ParticleEmitter(this->conf);
}
