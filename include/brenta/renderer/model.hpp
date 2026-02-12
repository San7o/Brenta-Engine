// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/transform.hpp>
#include <brenta/renderer/opengl/shader.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace brenta
{

class Model
{
public:
  
  GLint     wrapping;
  GLint     filtering_min;
  GLint     filtering_mag;
  GLboolean has_mipmap;
  GLint     mipmap_min;
  GLint     mipmap_mag;
  bool      flip;

  struct Config;
  class  Builder;

  Model() {}
  Model(Config conf);
  ~Model();

  /*
  constexpr Model(const Model&)            = delete;
  constexpr Model& operator=(const Model&) = delete;

  constexpr Model(Model&&) noexcept            = default;
  constexpr Model& operator=(Model&&) noexcept = default;
  */

  Model(const Model&)            = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) noexcept            = default;
  Model& operator=(Model&&) noexcept = default;
  
  Transform &get_transform();
  void draw(Shader::Name shader) const;

private:
  
  Transform                             transform;
  std::string                           path;
  std::vector<Mesh>                     meshes;
  std::vector<std::shared_ptr<Texture>> textures_loaded;
  std::string                           directory;

  void process_node(aiNode *node, const aiScene *scene);
  void process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<std::shared_ptr<Texture>>
  load_material_textures(aiMaterial *mat,
                         aiTextureType type,
                         Texture::Type type_brenta);
  
  static const Config default_config;
  
  void init();
};

struct Model::Config
{
  Transform   transform;
  std::string path;
  GLint       wrapping;
  GLint       filtering_min;
  GLint       filtering_mag;
  GLboolean   has_mipmap;
  GLint       mipmap_min;
  GLint       mipmap_mag;
  bool        flip;
};

class Model::Builder
{
private:

  Model::Config conf = Model::default_config;
  
public:

  Builder &transform(const Transform& transform);
  Builder &path(const std::filesystem::path &path);
  Builder &wrapping(GLint wrapping);
  Builder &filtering_min(GLint filtering_min);
  Builder &filtering_mag(GLint filtering_mag);
  Builder &has_mipmap(GLboolean has_mipmap);
  Builder &mipmap_min(GLint mipmap_min);
  Builder &mipmap_mag(GLint mipmap_mag);
  Builder &flip(bool flip);

  Model build();
};

} // namespace brenta
