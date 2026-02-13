// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/gl.hpp>
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
 * represent a 3D model. The mesh can be drawn using the draw()
 * method.
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

  Mesh(Config&& builder);
  
  constexpr Mesh(const Mesh&) = delete;
  constexpr Mesh& operator=(const Mesh&) = delete;

  constexpr Mesh(Mesh&&) noexcept = default;
  constexpr Mesh& operator=(Mesh&&) noexcept = default;
  ~Mesh();

  void draw() const;

private:

  Vao    vao;
  Vbo    vbo;
  Ebo    ebo;

  void init();
};

// The Vertex struct represents a vertex of a 3D model
class Mesh::Vertex
{
public:
  
  glm::vec3   position;
  glm::vec3   normal;
  glm::vec2   tex_coords;

  Vertex() = default;
  Vertex(glm::vec3 position)
    : position(position) {}
  Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 tex_coords)
    : position(position), normal(normal), tex_coords(tex_coords) {}
  
};

class Mesh::Config
{
public:
  
  std::vector<Vertex>                   vertices;
  std::vector<unsigned int>             indices;
  std::vector<std::shared_ptr<Texture>> textures;
  
};

class Mesh::Builder
{
private:

  Mesh::Config conf = {};
  
public:
  
  Builder &vertices(std::vector<Vertex> &&vertices);
  Builder &indices(std::vector<unsigned int> &&indices);
  Builder &texture(Texture &&texture);
  Builder &texture(std::shared_ptr<Texture> &&texture);
  Builder &textures(std::vector<std::shared_ptr<Texture>> &&textures);
  
  Mesh build();
};

} // namespace brenta
