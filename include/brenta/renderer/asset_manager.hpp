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

  using  AssetId = std::string;
  
  template<typename T>
  struct Asset;

  template<typename T>
  static tenno::shared_ptr<T> new_asset(const AssetId& id,
                                        typename T::Builder& builder);
  
  template<typename T>
  static tenno::shared_ptr<T> get(const AssetId& id);
  
  template<typename T>
  static bool reload(const AssetId& id);
  
  // Wipe out everything
  static void clear();
  
private:

  static std::unordered_map<AssetId, Asset<Model>>    models;
  static std::unordered_map<AssetId, Asset<Texture>>  textures;
  static std::unordered_map<AssetId, Asset<Material>> materials;
  static std::unordered_map<AssetId, Asset<Scene>>    scenes;
  static std::unordered_map<AssetId, Asset<Shader>>   shaders;
  static std::unordered_map<AssetId, Asset<Font>>     fonts;

  // Private constructor for singleton
  AssetManager() = default;
  
};

template<typename T>
struct AssetManager::Asset
{
  T::Builder         builder;
  tenno::weak_ptr<T> ptr;
};
  
} // namespace brenta
