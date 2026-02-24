// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/scene.hpp>
#include <brenta/gui/font.hpp>

namespace brenta
{

class AssetManager
{
public:

  using AssetId = std::string;
  
  static std::shared_ptr<Texture>  new_texture(const AssetId& id,
                                               const Texture::Config &conf);
  static std::shared_ptr<Model>    new_model(const AssetId& id,
                                             Model::Builder &&builder);
  static std::shared_ptr<Shader>   new_shader(const AssetId& id,
                                              const std::vector<Shader::Object> &objects);
  static std::shared_ptr<Shader>   new_shader(const AssetId& id,
                                              const GLchar **feedback_varyings,
                                              int num_varyings,
                                              const std::vector<Shader::Object> &objects);
  static std::shared_ptr<Material> new_material(const AssetId& id,
                                                std::shared_ptr<Shader> shader);
  static std::shared_ptr<Scene>    new_scene(const AssetId& id,
                                             std::shared_ptr<Camera> camera);
  static std::shared_ptr<Font>     new_font(const AssetId& id,
                                            const std::filesystem::path &path, int size);
  
  static std::shared_ptr<Texture>  get_texture(const AssetId& id);
  static std::shared_ptr<Model>    get_model(const AssetId& id);
  static std::shared_ptr<Shader>   get_shader(const AssetId& id);
  static std::shared_ptr<Material> get_material(const AssetId& id);
  static std::shared_ptr<Scene>    get_scene(const AssetId& id);
  static std::shared_ptr<Font>     get_font(const AssetId& id);

private:

  static std::unordered_map<AssetId, std::weak_ptr<Model>>    models;
  static std::unordered_map<AssetId, std::weak_ptr<Texture>>  textures;
  static std::unordered_map<AssetId, std::weak_ptr<Material>> materials;
  static std::unordered_map<AssetId, std::weak_ptr<Scene>>    scenes;
  static std::unordered_map<AssetId, std::weak_ptr<Shader>>   shaders;
  static std::unordered_map<AssetId, std::weak_ptr<Font>>     fonts;

  // Private constructor for singleton
  AssetManager() = default;
  
};

} // namespace brenta
