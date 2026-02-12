// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/opengl/vao.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <memory>

namespace brenta
{

/**
 * @brief The Mesh class represents a 3D model
 *
 * A mesh is a collection of vertices, indices and textures that
 * represent a 3D model. The mesh can be drawn using a shader and
 * calling the draw() method.
 */
class Mesh
{
public:

  class Vertex;
  class Config;
  class Builder;
  
  std::vector<Vertex> vertices;
  // Indicies are used to reduce the memory footprint required to
  // represent a 3D model. They are indices in the vertex array, there
  // can be multiple indices for the same vertex.
  std::vector<unsigned int>             indices;
  std::vector<std::shared_ptr<Texture>> textures;
  
  // Options are:
  // - GL_REPEAT: The default behavior for textures.
  //              Repeats the texture image.
  // - GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors
  //              the image with each repeat.
  // - GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1.
  //              The result is that higher coordinates become
  //              clamped to the edge, resulting in a stretched
  //              edge pattern.
  // - GL_CLAMP_TO_BORDER: Coordinates outside the range are
  //              now given a user-specified border color.
  GLint wrapping;

  // Type of texture filtering used when minifying (scaling
  // down) a texture. Options are:
  // - GL_NEAREST: select the closest color to the texture coordinate
  // - GL_LINEAR: interpolates the neighbouring pixels to get an
  //              approximate color.
  GLint filtering_min;

  // Type of texture filtering used when magnifying (scaling
  // up) a texture. Options are:
  // - GL_NEAREST: select the closest color to the texture coordinate
  // - GL_LINEAR: interpolates the neighbouring pixels to get an
  //              approximate color.
  GLint filtering_mag;
  GLboolean has_mipmap;

  // Type of texture filtering used when minifying (scaling
  // down) a texture with mipmaps. Options are:
  // - GL_NEAREST: select the closest color to the texture coordinate
  // - GL_LINEAR: interpolates the neighbouring pixels to get an
  //              approximate color.
  // - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
  //              closely matches the size of the pixel being textured
  //              and uses the GL_NEAREST criterion (the texture element
  //              nearest to the center of the pixel) to produce a texture
  //              value.
  // - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
  //              matches the size of the pixel being textured and uses
  //              the GL_LINEAR criterion (a weighted average of the four
  //              texture elements that are closest to the center of the
  //              pixel) to produce a texture value.
  // - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
  //              match the size of the pixel being textured and uses the
  //              GL_NEAREST criterion (the texture element nearest to the
  //              center of the pixel) to produce a texture value from each
  //              mipmap. The final texture value is a weighted average of
  //              those two values.
  // - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
  //              match the size of the pixel being textured and uses the
  //              GL_LINEAR criterion (a weighted average of the texture
  //              elements that are closest to the center of the pixel) to
  //              produce a texture value from each mipmap. The final texture
  //              value is a weighted average of those two values.
  GLint mipmap_min;

  // Type of texture filtering used when magnifying (scaling
  // up) a texture with mipmaps. Options are:
  // - GL_NEAREST: select the closest color to the texture coordinate
  // - GL_LINEAR: interpolates the neighbouring pixels to get an
  //              approximate color.
  // - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
  //              closely matches the size of the pixel being textured
  //              and uses the GL_NEAREST criterion (the texture element
  //              nearest to the center of the pixel) to produce a texture
  //              value.
  // - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
  //              matches the size of the pixel being textured and uses
  //              the GL_LINEAR criterion (a weighted average of the four
  //              texture elements that are closest to the center of the
  //              pixel) to produce a texture value.
  // - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
  //              match the size of the pixel being textured and uses the
  //              GL_NEAREST criterion (the texture element nearest to the
  //              center of the pixel) to produce a texture value from each
  //              mipmap. The final texture value is a weighted average of
  //              those two values.
  // - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
  //              match the size of the pixel being textured and uses the
  //              GL_LINEAR criterion (a weighted average of the texture
  //              elements that are closest to the center of the pixel) to
  //              produce a texture value from each mipmap. The final texture
  //              value is a weighted average of those two values.
  GLint mipmap_mag;

  Mesh(Config&& conf);
  
  constexpr Mesh(const Mesh&) = delete;
  constexpr Mesh& operator=(const Mesh&) = delete;

  constexpr Mesh(Mesh&&) noexcept = default;
  constexpr Mesh& operator=(Mesh&&) noexcept = default;
  ~Mesh();

  void draw(Shader::Name shader_name) const;

private:

  Vao    vao;
  Vbo    vbo;
  Ebo    ebo;

  static const Config default_config;
  
  void init();
};

// The Vertex struct represents a vertex of a 3D model
class Mesh::Vertex
{
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

class Mesh::Config
{
public:
  std::vector<Vertex>                   vertices;
  std::vector<unsigned int>             indices;
  std::vector<std::shared_ptr<Texture>> textures;
  GLint     wrapping;
  GLint     filtering_min;
  GLint     filtering_mag;
  GLboolean has_mipmap;
  GLint     mipmap_min;
  GLint     mipmap_max;
};

class Mesh::Builder
{
private:

  Mesh::Config conf = Mesh::default_config;
  
public:
  Builder &vertices(std::vector<Vertex> vertices);
  Builder &indices(std::vector<unsigned int> indices);
  Builder &textures(std::vector<std::shared_ptr<Texture>> textures);
  Builder &wrapping(GLint wrapping);
  Builder &filtering_min(GLint filtering_min);
  Builder &filtering_mag(GLint filtering_mag);
  Builder &has_mipmap(GLboolean has_mipmap);
  Builder &mipmap_min(GLint mipmap_min);
  Builder &mipmap_mag(GLint mipmap_mag);

  Mesh build();
};

} // namespace brenta
