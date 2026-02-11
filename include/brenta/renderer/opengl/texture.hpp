// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h>

#include <string>
#include <filesystem>

namespace brenta
{

class Texture
{
public:

  using Id = unsigned int;

  enum class Type {
    None,
    Diffuse,
    Specular,
  };
  
  Texture::Id   id;
  Texture::Type type;
  std::string   path;
  
  // This method activates a texture unit. Arg is GL_TEXTURE0 + x
  static void active_texture(GLenum texture);
  static Texture::Id load(const std::filesystem::path &path, bool flip = true);
  static void bind_id(GLenum    target, Texture::Id id,
                      GLint     wrapping      = GL_REPEAT,
                      GLint     filtering_min = GL_NEAREST,
                      GLint     filtering_mag = GL_NEAREST,
                      GLboolean has_mipmap    = GL_TRUE,
                      GLint     mipmap_min    = GL_LINEAR_MIPMAP_LINEAR,
                      GLint     mipmap_mag    = GL_LINEAR);


  Texture() {}
  Texture(const std::filesystem::path &path,
          bool flip = true,
          Texture::Type type = Texture::Type::None);
  constexpr Texture(Texture&& other) noexcept
  {
    this->id = other.id;
    this->path = other.path;
    this->type = other.type;
    other.id = 0;
  }

  constexpr Texture& operator=(Texture&& other) noexcept
  {
    this->id = other.id;
    this->type = other.type;
    this->path = other.path;
    other.id = 0;
    return *this;
  }

  ~Texture();

  Texture::Id get_id() const;
  
  void bind(GLenum    target,
            GLint     wrapping      = GL_REPEAT,
            GLint     filtering_min = GL_NEAREST,
            GLint     filtering_mag = GL_NEAREST,
            GLboolean has_mpmap     = GL_TRUE,
            GLint     mipmap_min    = GL_LINEAR_MIPMAP_LINEAR,
            GLint     mipmap_mag    = GL_LINEAR);

private:
  
  static void set_texture_wrapping(GLint wrapping);
  static void set_texture_filtering(GLint filtering_min, GLint filtering_mag);
  static void set_mipmap(GLboolean has_mipmap, GLint mipmap_min,
                         GLint mipmap_mag);
  static void read_image(const char *path, bool flip);
  
};

} // namespace brenta
