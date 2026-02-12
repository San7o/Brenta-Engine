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
  
  std::vector<Vertex>                   vertices;
  // Indicies are used to reduce the memory footprint required to
  // represent a 3D model. They are indices in the vertex array, there
  // can be multiple indices for the same vertex.
  std::vector<unsigned int>             indices;
  std::vector<std::shared_ptr<Texture>> textures;

  Texture::Wrapping    wrapping;
  Texture::Filtering   filtering_min;
  Texture::Filtering   filtering_mag;
  GLboolean            has_mipmap;
  Texture::Filtering   mipmap_min;
  Texture::Filtering   mipmap_mag;

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
  
  glm::vec3   position;
  glm::vec3   normal;
  glm::vec2   tex_coords;
  
};

class Mesh::Config
{
public:
  
  std::vector<Vertex>                   vertices;
  std::vector<unsigned int>             indices;
  std::vector<std::shared_ptr<Texture>> textures;
  Texture::Wrapping     wrapping;
  Texture::Filtering    filtering_min;
  Texture::Filtering    filtering_mag;
  GLboolean             has_mipmap;
  Texture::Filtering    mipmap_min;
  Texture::Filtering    mipmap_max;
  
};

class Mesh::Builder
{
private:

  Mesh::Config conf = Mesh::default_config;
  
public:
  
  Builder &vertices(std::vector<Vertex> vertices);
  Builder &indices(std::vector<unsigned int> indices);
  Builder &textures(std::vector<std::shared_ptr<Texture>> textures);
  Builder &wrapping(Texture::Wrapping wrapping);
  Builder &filtering_min(Texture::Filtering filtering_min);
  Builder &filtering_mag(Texture::Filtering filtering_mag);
  Builder &has_mipmap(GLboolean has_mipmap);
  Builder &mipmap_min(Texture::Filtering mipmap_min);
  Builder &mipmap_mag(Texture::Filtering mipmap_mag);

  Mesh build();
};

} // namespace brenta
