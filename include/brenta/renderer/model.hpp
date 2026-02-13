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

  struct Config;
  class  Builder;

  Model() {}
  Model(Config &&conf);
  ~Model();

  Model(const Model&)            = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) noexcept            = default;
  Model& operator=(Model&&) noexcept = default;
  
  Transform &get_transform();
  void draw() const;

private:
  
  Transform                             transform;
  std::string                           path;
  std::string                           directory;
  std::vector<Mesh>                     meshes;
  std::vector<std::shared_ptr<Texture>> textures_loaded;
  
  void load(const Texture::Properties &props);
  void process_node(aiNode *node, const aiScene *scene,
                    const Texture::Properties &props);
  void process_mesh(aiMesh *mesh, const aiScene *scene,
                    const Texture::Properties &props);
  std::vector<std::shared_ptr<Texture>>
  load_material_textures(aiMaterial *mat,
                         aiTextureType type,
                         Texture::Type type_brenta,
                         const Texture::Properties &props);
  
};

struct Model::Config
{
  Transform                 transform     = {};
  std::string               model_path    = "";
  Texture::Properties       texture_props = {};
  std::vector<Mesh>         meshes        = {};
};

class Model::Builder
{
public:

  Builder &transform(const Transform& transform);
  Builder &path(const std::filesystem::path &path);
  Builder &texture_props(const Texture::Properties &props);
  Builder &mesh(Mesh &&mesh);
  Builder &meshes(std::vector<Mesh> &&meshes);

  Model build();

private:

  Model::Config conf = {};
  
};

} // namespace brenta
