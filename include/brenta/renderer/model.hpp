// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/transform.hpp>

#include <glad/glad.h>

#include <string>
#include <vector>
#include <memory>
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
  Model(Config &&conf);
  ~Model();

  Model(const Model&)            = delete;
  Model& operator=(const Model&) = delete;

  Model(Model&&) noexcept            = default;
  Model& operator=(Model&&) noexcept = default;
  
  Transform &get_transform();
  std::shared_ptr<Material> get_material();
  
  void draw() const;

private:
  
  std::filesystem::path                 path;
  std::string                           directory;
  
  Transform                             transform;
  std::shared_ptr<Material>             material;
  std::vector<Mesh>                     meshes;
  std::vector<std::shared_ptr<Texture>> textures_loaded;
  
  void load(const Texture::Properties &props);

  void process_shape(const tinyobj::attrib_t& attrib, 
                     const tinyobj::shape_t& shape,
                     const std::vector<tinyobj::material_t>& materials,
                     const Texture::Properties &props);
  std::vector<std::shared_ptr<Texture>>
  load_tiny_material(const tinyobj::material_t& mat,
                     const Texture::Properties &props);
  
};

struct Model::Config
{
  Transform                 transform     = {};
  std::shared_ptr<Material> material      = {};
  std::filesystem::path     model_path    = "";
  Texture::Properties       texture_props = {};
  std::vector<Mesh>         meshes        = {};
};

class Model::Builder
{
public:

  Builder &transform(const Transform& transform);
  Builder &material(std::shared_ptr<Material> material);
  Builder &path(const std::filesystem::path &path);
  Builder &texture_props(const Texture::Properties &props);
  Builder &mesh(Mesh &&mesh);
  Builder &meshes(std::vector<Mesh> &&meshes);

  Model build();

private:

  Model::Config conf = {};
  
};

} // namespace brenta
