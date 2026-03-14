// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/color.hpp>

#include <tenno/vector.hpp>

#include <string>
#include <filesystem>

namespace brenta
{

//
// Texture
// -------
//
// This class provides an interface to interact with opengl
// textures. There are several types of textures, as well as target
// and with different properties.
//
// To bind a texture, use the bind() method. Before this, you need
// to make sure that you set the coorect active_texture(), which
// is the index that the shader will use to access the bound texture.
//
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
  struct Properties;
  class  Builder;
  
  struct Properties
  {
    
    Texture::Wrapping  wrapping      = Texture::Wrapping::Repeat;
    Texture::Filtering filtering_min = Texture::Filtering::Nearest;
    Texture::Filtering filtering_mag = Texture::Filtering::Nearest;
    GLboolean          has_mipmap    = Gl::True;
    Texture::Filtering mipmap_min    = Texture::Filtering::LinearMipmapLinear;
    Texture::Filtering mipmap_mag    = Texture::Filtering::Linear;
    GLboolean          flipped       = false;

    // Setters

    Properties &set_wrapping(Texture::Wrapping wrapping);
    Properties &set_filtering_min(Texture::Filtering filtering);
    Properties &set_filtering_mag(Texture::Filtering filtering);
    Properties &set_has_mipmap(GLboolean mipmap);
    Properties &set_mipmap_min(Texture::Filtering filtering);
    Properties &set_mipmap_mag(Texture::Filtering filtering);
    Properties &set_flipped(GLboolean flipped);
  
  };

  // This tells the shader where to find the texture
  static void active_texture(int texture);
  static Texture::Id load(const std::filesystem::path &path, bool flip = true);
  static Texture::Id load_solid_color(Color color);
  static void bind_id(Texture::Target target, Texture::Id id);
  static void bind_id(Texture::Target target, Texture::Id id,
                      const Texture::Properties &properties);

  // Non static

  Texture() {}
  Texture(const Config &conf);
  
  Texture(const Texture&)            = delete;
  Texture &operator=(const Texture&) = delete;
  
  Texture(Texture&& other) noexcept
  {
    this->id         = other.id;
    this->path       = other.path;
    this->type       = other.type;
    this->target     = other.target;
    this->properties = other.properties;
    other.id = 0;
  }

  Texture& operator=(Texture&& other) noexcept
  {
    this->id         = other.id;
    this->type       = other.type;
    this->path       = other.path;
    this->target     = other.target;
    this->properties = other.properties;
    other.id = 0;
    return *this;
  }

  ~Texture();

  Texture::Id              get_id() const;
  std::filesystem::path    get_path() const;
  Texture::Type            get_type() const;
  Texture::Target          get_target() const;
  Texture::Properties     &get_properties();
  
  void bind();

protected:
  
  Texture::Id             id = 0;
  Texture::Type           type;
  Texture::Target         target;
  std::filesystem::path   path;
  Texture::Properties     properties;
  
  static void read_image(const std::filesystem::path &path, bool flip);
  
};
  
struct Texture::Config
{
  Texture::Type           type       = Texture::Type::None;
  Texture::Target         target     = Texture::Target::Texture2D;
  std::filesystem::path   path       = "";
  Texture::Properties     properties = {};
  std::optional<Color>    color      = {};
};

class Texture::Builder
{
public:

  Builder& type(Texture::Type type);
  Builder& target(Texture::Target target);
  Builder& path(const std::filesystem::path& path);
  Builder& flipped(bool flipped);
  Builder& properties(const Texture::Properties& prop);
  Builder& color(Color color);

  // Add path to be watched for hot-reloading
  Builder &watch(const std::filesystem::path &path);
  
  Texture build();
  tenno::vector<std::filesystem::path> get_watch_paths() const;  
  
private:

  Texture::Config conf = {};
  tenno::vector<std::filesystem::path> watch_paths = {};  

};
  
} // namespace brenta
