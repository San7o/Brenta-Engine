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

texture::texture(const std::string &path,
                 bool flip,
                 const std::string &type)
{
  this->type = type;
  this->path = path;
  this->id = this->load(path, flip);
  DEBUG("texture: created");
}

texture::~texture()
{
  if (this->id == 0) return;

  glDeleteTextures(1, &this->id);
  this->id = 0;
  DEBUG("texture: deleted");
}

unsigned int texture::get_id() const
{
  return this->id;
}

void texture::active_texture(GLenum texture)
{
  glActiveTexture(texture);
}

unsigned int texture::load(const std::string &path, bool flip)
{
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  read_image(path.c_str(), flip);
  return texture;
}

void texture::bind_id(GLenum target, unsigned int id, GLint wrapping,
                      GLint filtering_min, GLint filtering_mag,
                      GLboolean has_mipmap, GLint mipmap_min,
                      GLint mipmap_mag)
{
  glBindTexture(target, id);
  set_texture_wrapping(wrapping);
  set_texture_filtering(filtering_min, filtering_mag);
  set_mipmap(has_mipmap, mipmap_min, mipmap_mag);
}

void texture::bind(GLenum target, GLint wrapping,
                   GLint filtering_min, GLint filtering_mag,
                   GLboolean has_mipmap, GLint mipmap_min,
                   GLint mipmap_mag)
{
  texture::bind_id(target, this->id, wrapping, filtering_min,
                   filtering_mag, has_mipmap, mipmap_min, mipmap_mag);
}

void texture::set_texture_wrapping(GLint wrapping)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
}

void texture::set_texture_filtering(GLint filtering_min, GLint filtering_mag)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering_mag);
}

void texture::set_mipmap(GLboolean hasMipmap, GLint mipmap_min,
                         GLint mipmap_mag)
{
  if (!hasMipmap) return;

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmap_mag);
}

void texture::read_image(const char *path, bool flip)
{
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(flip);
  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
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
    ERROR("texture::read_image: failed to load texture at location: {}", path);
  }
  stbi_image_free(data);
}
