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

using namespace brenta;

Texture::Texture(const Config &conf)
{
  this->type       = conf.type;
  this->target     = conf.target;
  this->path       = conf.path;
  this->properties = conf.properties;
  this->id         = this->load(this->path, conf.properties.get_flipped());
  
  EVENT(Logger::Event::Lifetime, "texture: created {}", this->id);
  return;
}

Texture::~Texture()
{
  if (this->id == 0) return;

  glDeleteTextures(1, &this->id);
  
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

unsigned int Texture::load(const std::filesystem::path &path, bool flip)
{
  // save state
  GLint old_active_texture, old_texture_2d;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active_texture);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_texture_2d);
 
  Texture::Id texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  read_image(path, flip);

  // restore state
  glBindTexture(GL_TEXTURE_2D, old_texture_2d);
  glActiveTexture(old_active_texture);
  check_error();
  return texture;
}

void Texture::bind_id(Texture::Target target, Texture::Id id,
                      const Texture::Properties &prop)
{
  glBindTexture(target, id);
  check_error();

  // Wrapping
  glTexParameteri(target, GL_TEXTURE_WRAP_S, prop.get_wrapping());
  glTexParameteri(target, GL_TEXTURE_WRAP_T, prop.get_wrapping());

  // Filtering
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, prop.get_filtering_min());
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, prop.get_filtering_mag());
  
  // Mipmap
  if (!prop.get_has_mipmap()) return;

  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, prop.get_mipmap_min());
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, prop.get_mipmap_mag());

  check_error();
  return;
}

void Texture::bind()
{
  Texture::bind_id(this->target, this->id, this->properties);
  return;
}

void Texture::read_image(const std::filesystem::path &path, bool flip)
{
  int width, height, nrChannels;
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
  }
  else
  {
    ERROR("Texture::read_image: failed to load texture at location: {}",
          path.string());
  }
  stbi_image_free(data);

  check_error();
  return;
}

//
// Properties
//


Texture::Wrapping Texture::Properties::get_wrapping() const
{
  return this->prop_wrapping;
}

Texture::Filtering Texture::Properties::get_filtering_min() const
{
  return this->prop_filtering_min;
}

Texture::Filtering Texture::Properties::get_filtering_mag() const
{
  return this->prop_filtering_mag;
}

GLboolean Texture::Properties::get_has_mipmap() const
{
  return this->prop_has_mipmap;
}

Texture::Filtering Texture::Properties::get_mipmap_min() const
{
  return this->prop_mipmap_min;
}

Texture::Filtering Texture::Properties::get_mipmap_mag() const
{
  return this->prop_mipmap_mag;
}

GLboolean Texture::Properties::get_flipped() const
{
  return this->prop_flipped;
}

// Setters

Texture::Properties &Texture::Properties::wrapping(Texture::Wrapping wrapping)
{
  this->prop_wrapping = wrapping;
  return *this;
}

Texture::Properties &Texture::Properties::filtering_min(Texture::Filtering filtering)
{
  this->prop_filtering_min = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::filtering_mag(Texture::Filtering filtering)
{
  this->prop_filtering_mag = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::has_mipmap(GLboolean mipmap)
{
  this->prop_has_mipmap = mipmap;
  return *this;
}

Texture::Properties &Texture::Properties::mipmap_min(Texture::Filtering filtering)
{
  this->prop_mipmap_min = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::mipmap_mag(Texture::Filtering filtering)
{
  this->prop_mipmap_mag = filtering;
  return *this;
}

Texture::Properties &Texture::Properties::flipped(GLboolean flipped)
{
  this->prop_flipped = flipped;
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
