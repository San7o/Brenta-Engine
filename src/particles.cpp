// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/camera.hpp>
#include <brenta/particles.hpp>
#include <brenta/window.hpp>
#include <brenta/shader.hpp>
#include <brenta/texture.hpp>
#include <brenta/translation.hpp>

#include <iostream>
#include <time.h>

using namespace brenta;

const particle_emitter::config particle_emitter::default_config = {
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

particle_emitter::particle_emitter(config conf)
{
  this->starting_position = conf.starting_position;
  this->starting_velocity = conf.starting_velocity;
  this->starting_spread = conf.starting_spread;
  this->starting_time_to_live = conf.starting_time_to_live;
  this->num_particles = conf.num_particles;
  this->spawn_rate = conf.spawn_rate;
  this->scale = conf.scale;
  this->atlas = 0;
  this->atlas_width = conf.atlas_width;
  this->atlas_height = conf.atlas_height;
  this->atlas_index = conf.atlas_index;
  this->current = 0;
  this->cam = conf.cam;

  // Load Texture Atlas
  this->atlas = texture::load_texture(conf.atlas_path, false);

  // Create shaders
  const GLchar *varyings[] = {"outPosition", "outVelocity", "outTTL"};
  shader::create(varyings, 3, "particle_update",
                 GL_VERTEX_SHADER, "src/shaders/particle_update.vs");
  shader::create("particle_render",
                 GL_VERTEX_SHADER, "src/shaders/particle_render.vs",
                 GL_GEOMETRY_SHADER, "src/shaders/particle_render.gs",
                 GL_FRAGMENT_SHADER, "src/shaders/particle_render.fs");

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
    ERROR("particle_emitter::render_particles: Camera not set or null for emitter");
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
  int window_width = window::get_width();
  int window_height = window::get_height();
  types::translation t = types::translation();
  t.set_view(this->cam->get_view_matrix());
  t.set_projection(this->cam->get_projection_matrix(window_width, window_height));
  t.set_model(glm::mat4(1.0f));
  t.set_shader("particle_render");
  shader::set_int("particle_render", "atlas_width", this->atlas_width);
  shader::set_int("particle_render", "atlas_height", this->atlas_height);
  shader::set_int("particle_render", "atlas_index", this->atlas_index);
  shader::set_float("particle_render", "scale", this->scale);
  shader::set_float("particle_render", "aspect_ratio",
                    (float) window_width
                    / (float) window_height);

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

//
// Builder functions
//

particle_emitter::builder &
particle_emitter::builder::starting_position(glm::vec3 starting_position)
{
  this->conf.starting_position = starting_position;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::starting_velocity(glm::vec3 starting_velocity)
{
  this->conf.starting_velocity = starting_velocity;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::starting_spread(glm::vec3 starting_spread)
{
  this->conf.starting_spread = starting_spread;
  return *this;
}

particle_emitter::builder &particle_emitter::builder::starting_time_to_live(
  float starting_time_to_live)
{
  this->conf.starting_time_to_live = starting_time_to_live;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::num_particles(int num_particles)
{
  this->conf.num_particles = num_particles;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::spawn_rate(float spawn_rate)
{
  this->conf.spawn_rate = spawn_rate;
  return *this;
}

particle_emitter::builder &particle_emitter::builder::scale(float scale)
{
  this->conf.scale = scale;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::atlas_path(std::string atlas_path)
{
  this->conf.atlas_path = atlas_path;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::atlas_width(int atlas_width)
{
  this->conf.atlas_width = atlas_width;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::atlas_height(int atlas_height)
{
  this->conf.atlas_height = atlas_height;
  return *this;
}

particle_emitter::builder &
particle_emitter::builder::atlas_index(int atlas_index)
{
  this->conf.atlas_index = atlas_index;
  return *this;
}

particle_emitter::builder &particle_emitter::builder::with_camera(camera *cam)
{
  this->conf.cam = cam;
  return *this;
}

particle_emitter particle_emitter::builder::build()
{
  return particle_emitter(this->conf);
}
