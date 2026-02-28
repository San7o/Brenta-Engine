// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/buffer.hpp>
#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/opengl/vao.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tenno/vector.hpp>
#include <tenno/memory.hpp>

namespace brenta
{

/**
 * @brief The Mesh class represents a collection of 3D vertices
 */
class Mesh
{
public:

  class Vertex;
  class Config;
  class Builder;
  
  tenno::vector<Vertex>                     vertices;
  // Indicies are used to reduce the memory footprint required to
  // represent a 3D model. They are indices in the vertex array, there
  // can be multiple indices for the same vertex.
  tenno::vector<unsigned int>               indices;
  tenno::vector<tenno::shared_ptr<Texture>> textures;

  Mesh(const Config& conf);
  
  constexpr Mesh(const Mesh&) = delete;
  constexpr Mesh& operator=(const Mesh&) = delete;

  constexpr Mesh(Mesh&& other) noexcept
  {
    this->vao = tenno::move(other.vao);
    this->vbo = tenno::move(other.vbo);
    this->ebo = tenno::move(other.ebo);
    this->vertices = tenno::move(other.vertices);
    this->indices  = tenno::move(other.indices);
    this->textures = tenno::move(other.textures);
    this->id = other.id;
    other.id = 0;
  }
  constexpr Mesh& operator=(Mesh&& other) noexcept
  {
    this->vao = tenno::move(other.vao);
    this->vbo = tenno::move(other.vbo);
    this->ebo = tenno::move(other.ebo);
    this->vertices = tenno::move(other.vertices);
    this->indices  = tenno::move(other.indices);
    this->textures = tenno::move(other.textures);
    this->id = other.id;
    other.id = 0;

    return *this;
  }
  ~Mesh();

  void draw() const;

private:

  Vao    vao;
  Vbo    vbo;
  Ebo    ebo;

  unsigned int id = 0;
  
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
  
  tenno::vector<Vertex>                     vertices;
  tenno::vector<unsigned int>               indices;
  tenno::vector<tenno::shared_ptr<Texture>> textures;
  
};

class Mesh::Builder
{
private:

  Mesh::Config conf = {};
  
public:
  
  Builder &vertices(tenno::vector<Vertex> &&vertices);
  Builder &indices(tenno::vector<unsigned int> &&indices);
  Builder &texture(Texture &&texture);
  Builder &texture(tenno::shared_ptr<Texture> &&texture);
  Builder &textures(tenno::vector<tenno::shared_ptr<Texture>> &&textures);
  
  Mesh build();
};

} // namespace brenta
