// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/buffer.hpp>
#include <brenta/gl.hpp>
#include <brenta/shader.hpp>
#include <brenta/texture.hpp>
#include <brenta/vao.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <memory>

namespace brenta
{

namespace types
{

/**
 * @brief The Vertex struct represents a vertex of a 3D model
 *
 * A vertex is a point in 3D space that has a position, a normal and
 * texture coordinates.
 */
struct vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

} // namespace types

/**
 * @brief The Mesh class represents a 3D model
 *
 * A mesh is a collection of vertices, indices and textures that
 * represent a 3D model. The mesh can be drawn using a shader and
 * calling the draw() method.
 */
class mesh
{
public:
  
  std::vector<types::vertex> vertices;
  /**
   * @brief indices of the mesh
   *
   * Indicies are used to reduce the memory footprint required to
   * represent a 3D model. They are indices in the vertex array and
   * there can be multiple indices for a vertex.
   */
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<texture>> textures;
  /**
   * @brief Type of texture wrapping
   *
   * Options are:
   * - GL_REPEAT: The default behavior for textures.
   *              Repeats the texture image.
   * - GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors
   *              the image with each repeat.
   * - GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1.
   *              The result is that higher coordinates become
   *              clamped to the edge, resulting in a stretched
   *              edge pattern.
   * - GL_CLAMP_TO_BORDER: Coordinates outside the range are
   *              now given a user-specified border color.
   */
  GLint wrapping;
  /**
   * @brief Minifying texture filtering
   *
   * Type of texture filtering used when minifying (scaling
   * down) a texture. Options are:
   * - GL_NEAREST: select the closest color to the texture coordinate
   * - GL_LINEAR: interpolates the neighbouring pixels to get an
   *              approximate color.
   */
  GLint filtering_min;
  /**
   * @brief Magnifying texture filtering
   *
   * Type of texture filtering used when magnifying (scaling
   * up) a texture. Options are:
   * - GL_NEAREST: select the closest color to the texture coordinate
   * - GL_LINEAR: interpolates the neighbouring pixels to get an
   *              approximate color.
   */
  GLint filtering_mag;
  /**
   * @brief Should the texture have a mipmap?
   */
  GLboolean has_mipmap;
  /**
   * @brief Type of mipmap minifying texture filtering
   *
   * Type of texture filtering used when minifying (scaling
   * down) a texture with mipmaps. Options are:
   * - GL_NEAREST: select the closest color to the texture coordinate
   * - GL_LINEAR: interpolates the neighbouring pixels to get an
   *              approximate color.
   * - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
   *              closely matches the size of the pixel being textured
   *              and uses the GL_NEAREST criterion (the texture element
   *              nearest to the center of the pixel) to produce a texture
   *              value.
   * - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
   *              matches the size of the pixel being textured and uses
   *              the GL_LINEAR criterion (a weighted average of the four
   *              texture elements that are closest to the center of the
   *              pixel) to produce a texture value.
   * - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
   *              match the size of the pixel being textured and uses the
   *              GL_NEAREST criterion (the texture element nearest to the
   *              center of the pixel) to produce a texture value from each
   *              mipmap. The final texture value is a weighted average of
   *              those two values.
   * - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
   *              match the size of the pixel being textured and uses the
   *              GL_LINEAR criterion (a weighted average of the texture
   *              elements that are closest to the center of the pixel) to
   *              produce a texture value from each mipmap. The final texture
   *              value is a weighted average of those two values.
   */
  GLint mipmap_min;
  /**
   * @brief Type of mipmap magnifying texture filtering
   *
   * Type of texture filtering used when magnifying (scaling
   * up) a texture with mipmaps. Options are:
   * - GL_NEAREST: select the closest color to the texture coordinate
   * - GL_LINEAR: interpolates the neighbouring pixels to get an
   *              approximate color.
   * - GL_NEAREST_MIPMAP_NEAREST: selects the mipmap that most
   *              closely matches the size of the pixel being textured
   *              and uses the GL_NEAREST criterion (the texture element
   *              nearest to the center of the pixel) to produce a texture
   *              value.
   * - GL_LINEAR_MIPMAP_NEAREST: selects the mipmap that most closely
   *              matches the size of the pixel being textured and uses
   *              the GL_LINEAR criterion (a weighted average of the four
   *              texture elements that are closest to the center of the
   *              pixel) to produce a texture value.
   * - GL_NEAREST_MIPMAP_LINEAR: selects the two mipmaps that most closely
   *              match the size of the pixel being textured and uses the
   *              GL_NEAREST criterion (the texture element nearest to the
   *              center of the pixel) to produce a texture value from each
   *              mipmap. The final texture value is a weighted average of
   *              those two values.
   * - GL_LINEAR_MIPMAP_LINEAR: selects the two mipmaps that most closely
   *              match the size of the pixel being textured and uses the
   *              GL_LINEAR criterion (a weighted average of the texture
   *              elements that are closest to the center of the pixel) to
   *              produce a texture value from each mipmap. The final texture
   *              value is a weighted average of those two values.
   */
  GLint mipmap_mag;

  struct config;
  class builder;
  static const config default_config;
  
  mesh(config&& conf);
  
  constexpr mesh(const mesh&) = delete;
  constexpr mesh& operator=(const mesh&) = delete;

  constexpr mesh(mesh&&) noexcept = default;
  constexpr mesh& operator=(mesh&&) noexcept = default;
  ~mesh();

  void draw(types::shader_name_t shader_name);

private:

  types::vao vao;
  types::buffer vbo;
  types::buffer ebo;

  void init();
};

struct mesh::config
{
  std::vector<types::vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<texture>> textures;
  GLint wrapping;
  GLint filtering_min;
  GLint filtering_mag;
  GLboolean has_mipmap;
  GLint mipmap_min;
  GLint mipmap_max;
};

/**
 * @brief The Builder class is used to build a Mesh object
 */
class mesh::builder
{
private:

  mesh::config conf = mesh::default_config;
  
public:
  builder &vertices(std::vector<types::vertex> vertices);
  builder &indices(std::vector<unsigned int> indices);
  builder &textures(std::vector<std::shared_ptr<texture>> textures);
  builder &wrapping(GLint wrapping);
  builder &filtering_min(GLint filtering_min);
  builder &filtering_mag(GLint filtering_mag);
  builder &has_mipmap(GLboolean has_mipmap);
  builder &mipmap_min(GLint mipmap_min);
  builder &mipmap_mag(GLint mipmap_mag);

  mesh build();
};

} // namespace brenta
