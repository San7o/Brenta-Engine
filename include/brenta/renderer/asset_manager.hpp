// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/scene.hpp>
#include <brenta/font.hpp>

namespace brenta
{

class AssetManager
{
public:

  using AssetId = std::string;
  
  static tenno::shared_ptr<Texture>  new_texture(const AssetId& id,
                                                 const Texture::Config &conf);
  static tenno::shared_ptr<Model>    new_model(const AssetId& id,
                                               Model::Builder &&builder);
  static tenno::shared_ptr<Shader>   new_shader(const AssetId& id,
                                                const tenno::vector<Shader::Object> &objects);
  static tenno::shared_ptr<Shader>   new_shader(const AssetId& id,
                                                const GLchar **feedback_varyings,
                                                int num_varyings,
                                                const tenno::vector<Shader::Object> &objects);
  static tenno::shared_ptr<Material> new_material(const AssetId& id,
                                                tenno::shared_ptr<Shader> shader);
  static tenno::shared_ptr<Scene>    new_scene(const AssetId& id,
                                               tenno::shared_ptr<Camera> camera);
  static tenno::shared_ptr<Font>     new_font(const AssetId& id,
                                              const std::filesystem::path &path,
                                              int size);
  
  static tenno::shared_ptr<Texture>  get_texture(const AssetId& id);
  static tenno::shared_ptr<Model>    get_model(const AssetId& id);
  static tenno::shared_ptr<Shader>   get_shader(const AssetId& id);
  static tenno::shared_ptr<Material> get_material(const AssetId& id);
  static tenno::shared_ptr<Scene>    get_scene(const AssetId& id);
  static tenno::shared_ptr<Font>     get_font(const AssetId& id);

  static void clear();
  
private:

  static std::unordered_map<AssetId, tenno::weak_ptr<Model>>    models;
  static std::unordered_map<AssetId, tenno::weak_ptr<Texture>>  textures;
  static std::unordered_map<AssetId, tenno::weak_ptr<Material>> materials;
  static std::unordered_map<AssetId, tenno::weak_ptr<Scene>>    scenes;
  static std::unordered_map<AssetId, tenno::weak_ptr<Shader>>   shaders;
  static std::unordered_map<AssetId, tenno::weak_ptr<Font>>     fonts;

  // Private constructor for singleton
  AssetManager() = default;
  
};

} // namespace brenta
