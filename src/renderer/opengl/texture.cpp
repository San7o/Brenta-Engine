// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/logger.hpp>

#include <glad/glad.h>

#include <iostream>
#include <stb_image.h>
#include <string>
#include <cmath>

using namespace brenta;

int Texture::tot_memory = 0;

Texture::Texture(const Config &conf)
{
  this->type       = conf.type;
  this->target     = conf.target;
  this->properties = conf.properties;

  if (conf.color)
  {
    this->id = this->load_solid_color(conf.color.value());

    // Update memory for profiling
    this->memory         = 4;
    Texture::tot_memory += this->memory;
  
    EVENT(Logger::Event::Lifetime,
          "texture: created {} from color r={},g={},b={},a={}",
          this->id, conf.color->r, conf.color->g,
          conf.color->b, conf.color->a);
  }
  else
  {
    this->path = conf.path;
    if (this->path == "")
      return;

    int loaded_memory = 0;
    this->id = this->load(this->path, loaded_memory, conf.properties.flipped);

    // Update memory for profiling
    this->memory         = loaded_memory;
    Texture::tot_memory += this->memory;
  
    
    EVENT(Logger::Event::Lifetime, "texture: created {} from path {}",
          this->id, this->path.string());
  }
  
  return;
}

Texture::~Texture()
{
  if (this->id == 0) return;

  glDeleteTextures(1, &this->id);

  // Update memory for profiling
  Texture::tot_memory -= this->memory;
  this->memory = 0;
  
  EVENT(Logger::Event::Lifetime, "texture: deleted {}", this->id);
  this->id = 0;
  return;
}

Texture::Id Texture::get_id() const
{
  return this->id;
}

std::filesystem::path Texture::get_path() const
{
  return this->path;
}

Texture::Type Texture::get_type() const
{
  return this->type;
}

Texture::Target Texture::get_target() const
{
  return this->target;
}


Texture::Properties &Texture::get_properties()
{
  return this->properties;
}

void Texture::active_texture(int texture)
{
  glActiveTexture(GL_TEXTURE0 + texture);
  check_error();
  return;
}

unsigned int Texture::load_solid_color(Color color)
{
  // save state
  GLint old_active_texture, old_texture_2d;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active_texture);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_texture_2d);
 
  Texture::Id texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  unsigned char color_rgba[4] = {
    (unsigned char)(color.r * 255.0f),
    (unsigned char)(color.g * 255.0f),
    (unsigned char)(color.b * 255.0f),
    (unsigned char)(color.a * 255.0f)
  };

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, &color_rgba[0]);
  check_error();

  // restore state
  glBindTexture(GL_TEXTURE_2D, old_texture_2d);
  glActiveTexture(old_active_texture);
  check_error();

  return texture;
}

unsigned int Texture::load(const std::filesystem::path &path,
                           int &loaded_bytes, bool flipped)
{
  // save state
  GLint old_active_texture, old_texture_2d;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active_texture);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_texture_2d);
 
  Texture::Id texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  loaded_bytes = read_image(path, flipped);

  // restore state
  glBindTexture(GL_TEXTURE_2D, old_texture_2d);
  glActiveTexture(old_active_texture);
  check_error();
  return texture;
}

void Texture::bind_id(Texture::Target target, Texture::Id id)
{
  Texture::bind_id(target, id, Texture::Properties{});
}

void Texture::bind_id(Texture::Target target, Texture::Id id,
                      const Texture::Properties &prop)
{
  glBindTexture(target, id);
  check_error();

  // Wrapping
  glTexParameteri(target, GL_TEXTURE_WRAP_S, prop.wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, prop.wrapping);
  glTexParameteri(target, GL_TEXTURE_WRAP_R, prop.wrapping);

  // Filtering
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, prop.filtering_min);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, prop.filtering_mag);
  
  // Mipmap
  if (!prop.has_mipmap) return;

  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, prop.mipmap_min);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, prop.mipmap_mag);

  check_error();
  return;
}

void Texture::bind()
{
  Texture::bind_id(this->target, this->id, this->properties);
  return;
}

int Texture::read_image(const std::filesystem::path &path, bool flip)
{
  int width, height, nrChannels;
  int tot_memory = 0;
  stbi_set_flip_vertically_on_load(flip);
  unsigned char *data = stbi_load(path.string().c_str(), &width,
                                  &height, &nrChannels, 0);
  if (data)
  {
    GLenum format = GL_RGB;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    check_error();
  
    // Update memory for profiling
    tot_memory += nrChannels * width * height;
  }
  else
  {
    ERROR("Texture::read_image: failed to load texture at location: {}",
          path.string());
  }
  stbi_image_free(data);

  return tot_memory;
}

//
// Properties
//


Texture::Properties &Texture::Properties::set_wrapping(Texture::Wrapping wrapping)
{
  this->wrapping = wrapping;
  return *this;
}

Texture::Properties &Texture::Properties::set_filtering_min(Texture::Filtering filtering)
{
  this->filtering_min = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::set_filtering_mag(Texture::Filtering filtering)
{
  this->filtering_mag = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::set_has_mipmap(GLboolean mipmap)
{
  this->has_mipmap = mipmap;
  return *this;
}

Texture::Properties &Texture::Properties::set_mipmap_min(Texture::Filtering filtering)
{
  this->mipmap_min = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::set_mipmap_mag(Texture::Filtering filtering)
{
  this->mipmap_mag = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::set_flipped(GLboolean flipped)
{
  this->flipped = flipped;
  return *this;
}

//
// Builder
//

// Texture

Texture::Builder& Texture::Builder::type(Texture::Type type)
{
  this->conf.type = type;
  return *this;
}

Texture::Builder& Texture::Builder::target(Texture::Target target)
{
  this->conf.target = target;
  return *this;
}

Texture::Builder& Texture::Builder::path(const std::filesystem::path& path)
{
  this->conf.path = path;
  return *this;
}

Texture::Builder& Texture::Builder::properties(const Texture::Properties& prop)
{
  this->conf.properties = prop;
  return *this;
}

Texture::Builder& Texture::Builder::color(Color color)
{
  this->conf.color = color;
  return *this;
}

Texture::Builder& Texture::Builder::watch(const std::filesystem::path& path)
{
  this->watch_paths.push_back(path);
  return *this;
}

Texture Texture::Builder::build()
{
  return Texture(this->conf);
}

tenno::vector<std::filesystem::path> Texture::Builder::get_watch_paths() const
{
  return this->watch_paths;
}
