// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/camera.hpp>
#include <brenta/renderer/particles.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/window.hpp>

#include <iostream>
#include <time.h>

#include "./shaders/c/particle_render_fs.c"
#include "./shaders/c/particle_render_vs.c"
#include "./shaders/c/particle_render_gs.c"
#include "./shaders/c/particle_update_vs.c"

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

struct ParticleSettings {
  glm::vec3 gravity;
  float     deltaTime;
  glm::vec3 emitterVel;
  float     emitterTTL;
  glm::vec3 emitterPos;
  float     spawnProbability;
  glm::vec3 emitterSpread;
  float     __padding;
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
  this->current_fbo_index = 0;
  this->cam = conf.cam;

  // Load Texture Atlas
  this->atlas = std::move(Texture(conf.atlas_path, false));

  // Create shaders
  const GLchar *varyings[] = {"outPosition", "outVelocity", "outTTL"};
  auto shader_update =
    Shader::create(varyings,
                   sizeof(varyings) / sizeof(varyings[0]),
                   "particle_update", {
                     { Shader::Type::Vertex, particle_update_vs }});
  if (!shader_update) return;
  
  auto shader_render =
    Shader::create("particle_render", {
        { Shader::Type::Vertex,   particle_render_vs },
        { Shader::Type::Geometry, particle_render_gs },
        { Shader::Type::Fragment, particle_render_fs }});
  if (!shader_render) return;

  // This is needed to render points
  glEnable(GL_PROGRAM_POINT_SIZE);

  this->vao.init();
  this->vao.bind();
  Gl::check_error();

  // Create fbos
  this->fbo[0].init(Buffer::Target::TransformFeedback);
  this->fbo[1].init(Buffer::Target::TransformFeedback);

  this->fbo[0].bind();
  this->fbo[0].copy_data(NULL, this->num_particles * 2 * sizeof(glm::vec3)
                         + this->num_particles * sizeof(float),
                         Buffer::DataUsage::DynamicCopy);
  this->fbo[1].bind();
  this->fbo[1].copy_data(NULL,
                         this->num_particles * 2 * sizeof(glm::vec3)
                         + this->num_particles * sizeof(float),
                         Buffer::DataUsage::DynamicCopy);
  Gl::check_error();

  // Unbind buffers
  this->fbo[0].unbind();
  this->vao.unbind();

  // Setup UBO
  this->ubo.init(*shader_update, "settings", 3, sizeof(ParticleSettings));
  this->ubo.bind();
  this->ubo.copy_data(NULL,
                      sizeof(ParticleSettings),
                      Buffer::DataUsage::DynamicDraw);
  this->ubo.unbind();

  return;
}

// Update particles using Transform Feedback
void ParticleEmitter::update(float delta_time)
{
  auto shader = Shader::get_shader("particle_update");
  if (!shader) return;
  shader->use();

  ParticleSettings settings = {
    .gravity = glm::vec3(0.0f, -9.81f, 0.0f),
    .deltaTime = delta_time,
    .emitterVel = this->starting_velocity,
    .emitterTTL = this->starting_time_to_live,
    .emitterPos = this->starting_position,
    .spawnProbability = this->spawn_rate,
    .emitterSpread = this->starting_spread,
    .__padding = 0.0f,
  };

  // ubo
  
  this->ubo.bind();
  this->ubo.copy_data(&settings, sizeof(ParticleSettings),
                      Buffer::DataUsage::DynamicDraw);
  this->ubo.unbind();
  Gl::check_error();

  // Vao
  
  this->vao.bind();
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, fbo[current_fbo_index].get_id());
  Gl::check_error();
  glBindBuffer(GL_ARRAY_BUFFER, fbo[!current_fbo_index].get_id());
  this->vao.link_buffer(this->fbo[!current_fbo_index],
                        0, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float), (void *) 0);
  this->vao.link_buffer(this->fbo[!current_fbo_index],  
                        1, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float),
                        (void *) sizeof(glm::vec3));
  this->vao.link_buffer(this->fbo[!current_fbo_index],  
                        2, 1, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float),
                        (void *) (2 * sizeof(glm::vec3)));

  // Start transform feedback
  glEnable(GL_RASTERIZER_DISCARD);     // Disable rasterization
  glBeginTransformFeedback(GL_POINTS); // Enter transform feedback mode
  Gl::check_error();

  glDrawArrays(GL_POINTS, 0, num_particles);
  Gl::check_error();

  glEndTransformFeedback();         // Exit transform feedback mode
  glDisable(GL_RASTERIZER_DISCARD); // Enable rasterization
  
  // Unbind buffers
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  this->vao.unbind();
  glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
  return;
}

// Render particles
void ParticleEmitter::render()
{
  if (!this->cam)
  {
    ERROR("ParticleEmitter::render_particles: Camera not set or null for emitter");
    return;
  }

  auto shader = Shader::get_shader("particle_render");
  if (!shader) return;

  shader->use();
  this->vao.bind();

  glBindBuffer(GL_ARRAY_BUFFER, fbo[current_fbo_index].get_id());
  this->vao.link_buffer(this->fbo[current_fbo_index],
                        0, 3, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float), (void *) 0);
  this->vao.link_buffer(this->fbo[current_fbo_index],
                        1, 1, GL_FLOAT, GL_FALSE,
                        2 * sizeof(glm::vec3) + sizeof(float),
                        (void *) (2 * sizeof(glm::vec3)));
  Gl::check_error();

  // Set uniforms
  int window_width = Window::get_width();
  int window_height = Window::get_height();

  shader->set_mat4("view",       this->cam->get_view_matrix());
  shader->set_mat4("projection", this->cam->get_projection_matrix(window_width, window_height));
  shader->set_mat4("model",      glm::mat4(1.0f));
  shader->set_int("atlas_width",  this->atlas_width);
  shader->set_int("atlas_height", this->atlas_height);
  shader->set_int("atlas_index",  this->atlas_index);
  shader->set_float("scale",      this->scale);
  shader->set_float("aspect_ratio",
                           (float) window_width  / (float) window_height);

  // Set Textures
  Texture::active_texture(GL_TEXTURE0);
  this->atlas.bind(GL_TEXTURE_2D,
                   Texture::Wrapping::Repeat,
                   Texture::Filtering::Nearest,
                   Texture::Filtering::Nearest,
                   Gl::True,
                   Texture::Filtering::NearestMipmapNearest,
                   Texture::Filtering::Nearest);

  glDrawArrays(GL_POINTS, 0, num_particles);
  Gl::check_error();

  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  this->vao.unbind();

  // Swap buffers
  current_fbo_index = !current_fbo_index; // Swap buffers
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
