// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glad/glad.h>

#include <brenta/renderer/opengl/gl.hpp>

#include <string>
#include <filesystem>

namespace brenta
{

class Texture
{
public:

  using Id = unsigned int;

  enum Type
  {
    None,
    Diffuse,
    Specular,
  };

  enum Target
  {
    Texture1D                 = GL_TEXTURE_1D,
    Texture2D                 = GL_TEXTURE_2D,
    Texture3D                 = GL_TEXTURE_3D,
    Texture1DArray            = GL_TEXTURE_1D_ARRAY,
    Texture2DArray            = GL_TEXTURE_2D_ARRAY,
    TextureRectangle          = GL_TEXTURE_RECTANGLE,
    TextureCubeMap            = GL_TEXTURE_CUBE_MAP,
    TextureCubeMapArray       = GL_TEXTURE_CUBE_MAP_ARRAY,
    TextureBuffer             = GL_TEXTURE_BUFFER,
    Texture2DMultisample      = GL_TEXTURE_2D_MULTISAMPLE,
    Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
  };
  
  enum Wrapping
  {
    // Repeats the texture image. This is the default begaviour
    Repeat         = GL_REPEAT,
    // Same as Repeat byt mirrirs the image with each repeat
    MirroredRepeat = GL_MIRRORED_REPEAT,
    // Clamps the coordinates between 0 and 1. The result is that
    // higher coordinates become clamped to the edge, resulting in a
    // stretched edge pattern
    ClampToEdge    = GL_CLAMP_TO_EDGE,
    // Coordinates outsize the range are now given a use-specified
    // border color
    ClampToBorder  = GL_CLAMP_TO_BORDER,
  };
  
  enum Filtering
  {
    // Select the closest color to the texture coordinate
    Nearest              = GL_NEAREST,
    // Interpolates the neighbouring pixels to get an approximate
    // color
    Linear               = GL_LINEAR,
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

  struct Config;
  class  Properties;
  class  Builder;
  
  // This method activates a texture unit. Arg is GL_TEXTURE0 + x
  static void active_texture(GLenum texture);
  static Texture::Id load(const std::filesystem::path &path, bool flip = true);
  static void bind_id(Texture::Target target, Texture::Id id,
                      const Texture::Properties &properties = Texture::Properties());

  // Non static
    
  Texture() {}
  Texture(const Config &conf);
  
  constexpr Texture(Texture&& other) noexcept
  {
    this->id         = other.id;
    this->path       = other.path;
    this->type       = other.type;
    this->properties = other.properties;
    other.id = 0;
  }

  constexpr Texture& operator=(Texture&& other) noexcept
  {
    this->id         = other.id;
    this->type       = other.type;
    this->path       = other.path;
    this->properties = other.properties;
    other.id = 0;
    return *this;
  }

  ~Texture();

  Texture::Id get_id() const;
  std::string get_path() const;
  Texture::Type get_type() const;
  Texture::Properties &get_properties();
  
  void bind(Texture::Target target);

  class Properties
  {
  public:

    Properties() {};

    // Getters

    Texture::Wrapping  get_wrapping() const;
    Texture::Filtering get_filtering_min() const;
    Texture::Filtering get_filtering_mag() const;
    GLboolean          get_has_mipmap() const;
    Texture::Filtering get_mipmap_min() const;
    Texture::Filtering get_mipmap_mag() const;
    GLboolean          get_flipped() const;
  
    // Setters

    Properties &wrapping(Texture::Wrapping wrapping);
    Properties &filtering_min(Texture::Filtering filtering);
    Properties &filtering_mag(Texture::Filtering filtering);
    Properties &has_mipmap(GLboolean mipmap);
    Properties &mipmap_min(Texture::Filtering filtering);
    Properties &mipmap_mag(Texture::Filtering filtering);
    Properties &flipped(GLboolean flipped);
  
  private:
  
    Texture::Wrapping  prop_wrapping      = Texture::Wrapping::Repeat;
    Texture::Filtering prop_filtering_min = Texture::Filtering::Nearest;
    Texture::Filtering prop_filtering_mag = Texture::Filtering::Nearest;
    GLboolean          prop_has_mipmap    = Gl::True;
    Texture::Filtering prop_mipmap_min    = Texture::Filtering::LinearMipmapLinear;
    Texture::Filtering prop_mipmap_mag    = Texture::Filtering::Linear;
    GLboolean          prop_flipped       = true;
  };

private:
  
  Texture::Id          id;
  Texture::Type        type;
  std::string          path;
  Texture::Properties  properties;
  
  static void read_image(const char *path, bool flip);
  
};
  
struct Texture::Config
{
  Texture::Type       type       = Texture::Type::None;
  std::string         path       = "";
  Texture::Properties properties = {};
};

class Texture::Builder
{
public:

  Builder& type(Texture::Type type);
  Builder& path(const std::string& path);
  Builder& flipped(bool flipped);
  Builder& properties(const Texture::Properties& prop);

  Texture build();
  
private:

  Texture::Config conf = {};
  
};
  
} // namespace brenta
