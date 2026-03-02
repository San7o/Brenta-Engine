// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/transform.hpp>

#include <glad/glad.h>

#include <tenno/vector.hpp>
#include <tenno/memory.hpp>
#include <string>
#include <filesystem>

#include <tiny_obj_loader.h>

namespace brenta
{

class Model
{
public:

  struct Config;
  class  Builder;

  Model() = default;
  Model(Config &conf);
  ~Model();

  Model(const Model&)            = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) noexcept            = default;
  Model& operator=(Model&&) noexcept = default;
  
  Transform &get_transform();
  tenno::shared_ptr<Material> get_material();
  
  void draw() const;

private:
  
  std::filesystem::path                 path;
  std::string                           directory;
  
  Transform                                 transform;
  tenno::shared_ptr<Material>               material;
  tenno::vector<Mesh>                       meshes;
  tenno::vector<tenno::shared_ptr<Texture>> textures_loaded;
  
  void load(const Texture::Properties &props);

  void process_shape(const tinyobj::attrib_t& attrib, 
                     const tinyobj::shape_t& shape,
                     const tenno::vector<tinyobj::material_t>& materials,
                     const Texture::Properties &props);
  tenno::vector<tenno::shared_ptr<Texture>>
  load_tiny_material(const tinyobj::material_t& mat,
                     const Texture::Properties &props);
  
};

struct Model::Config
{
  Transform                     transform     = {};
  tenno::shared_ptr<Material>   material      = {};
  std::filesystem::path         model_path    = "";
  Texture::Properties           texture_props = {};
  tenno::vector<Mesh::Builder>  meshes        = {};
};

class Model::Builder
{
public:

  Builder &transform(const Transform& transform);
  Builder &material(tenno::shared_ptr<Material> material);
  Builder &path(const std::filesystem::path &path);
  Builder &texture_props(const Texture::Properties &props);
  Builder &mesh(const Mesh::Builder &mesh);
  Builder &mesh(Mesh::Builder &&mesh);
  Builder &meshes(const tenno::vector<Mesh::Builder> &meshes);
  Builder &meshes(tenno::vector<Mesh::Builder> &&meshes);

  // Add path to be watched for hot-reloading
  Builder &watch(const std::filesystem::path &path);

  Model build();
  tenno::vector<std::filesystem::path> get_watch_paths() const;

private:

  Model::Config conf = {};
  tenno::vector<std::filesystem::path> watch_paths = {};
  
};

} // namespace brenta
