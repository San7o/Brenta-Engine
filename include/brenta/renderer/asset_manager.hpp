// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/texture.hpp>
#include <brenta/renderer/model.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/scene.hpp>

namespace brenta
{

class AssetManager
{
public:

  static std::shared_ptr<Texture>  get_texture(const Texture::Config &conf);
  static std::shared_ptr<Model>    get_model(Config &&conf);
  // TODO: make shader not static and make it cleanup in the destructor
  static std::shared_ptr<Material> get_material(std::shared_ptr<Shader> shader);
  static std::shared_ptr<Scene>    get_scene(std::shared_ptr<Camera> camera);

private:

  static std::unordered_map<std::string, std::weak_ptr<Model>>    models;
  static std::unordered_map<std::string, std::weak_ptr<Texture>>  textures;
  static std::unordered_map<std::string, std::weak_ptr<Material>> materials;
  static std::unordered_map<std::string, std::weak_ptr<Scene>>    scenes;

  // Private constructor for singleton
  AssetManager() = default;
  
};

} // namespace brenta
