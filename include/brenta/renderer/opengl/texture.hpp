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
  
  enum Wrapping
  {
    // Repeats the texture image. This is the default begaviour
    Repeat = GL_REPEAT,
    // Same as Repeat byt mirrirs the image with each repeat
    MirroredRepeat = GL_MIRRORED_REPEAT,
    // Clamps the coordinates between 0 and 1. The result is that
    // higher coordinates become clamped to the edge, resulting in a
    // stretched edge pattern
    ClampToEdge = GL_CLAMP_TO_EDGE,
    // Coordinates outsize the range are now given a use-specified
    // border color
    ClampToBorder = GL_CLAMP_TO_BORDER,
  };
  
  // Type of texture filtering used when minifying or maxifing
  // (scaling down or up) a texture.
  enum Filtering
  {
    // Select the closest color to the texture coordinate
    Nearest              = GL_NEAREST,
    // Interpolates the neighbouring pixels to get an approximate
    // color
    Linear               = GL_NEAREST,
    // Selects the mipmap that most closely matches the size of the
    // pixel being textured and uses the GL_NEAREST criterion (the
    // texture element nearest to the center of the pixel) to produce
    // a texture value.
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
    // Selects the mipmap that most closely matches the size of the
    // pixel being textured and uses the GL_LINEAR criterion (a
    // weighted average of the four texture elements that are closest
    // to the center of the pixel) to produce a texture value.
    NearestMipmapLinear  = GL_NEAREST_MIPMAP_LINEAR,
    // Selects the two mipmaps that most closely match the size of the
    // pixel being textured and uses the GL_NEAREST criterion (the
    // texture element nearest to the center of the pixel) to produce
    // a texture value from each mipmap. The final texture value is a
    // weighted average of those two values.
    LinearMipmapNearest  = GL_LINEAR_MIPMAP_NEAREST,
    // Selects the two mipmaps that most closely match the size of the
    // pixel being textured and uses the GL_LINEAR criterion (a
    // weighted average of the texture elements that are closest to
    // the center of the pixel) to produce a texture value from each
    // mipmap. The final texture value is a weighted average of those
    // two values.
    LinearMipmapLinear   = GL_LINEAR_MIPMAP_LINEAR,
  };
  
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

  Texture::Id    id;
  Texture::Type  type;
  std::string    path;
  
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
