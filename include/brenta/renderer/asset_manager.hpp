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
#include <brenta/fswatcher.hpp>

#include <tenno/memory.hpp>
#include <tenno/thread.hpp>

namespace brenta
{

class AssetManager
{
public:

  using  AssetId = std::string;
  
  template<typename T>
  struct Asset;
  struct HotReloadItem;

  enum class AssetType
  {
    Model,
    Texture,
    Material,
    Scene,
    Shader,
    Font,
  };
  
  template<typename T>
  static tenno::shared_ptr<T> new_asset(const AssetId& id,
                                        typename T::Builder& builder);
  
  template<typename T>
  static tenno::shared_ptr<T> get(const AssetId& id);
  
  template<typename T>
  static bool reload(const AssetId& id);

  // Wipe out everything
  static void clear();

  //
  // Hotreload API
  //
  
  static void hotreload_activate();
  static void hotreload_deactivate();
  // Call this function to reload the assets that needs update
  static void hotreload_update();
  
private:

  static std::unordered_map<AssetId, Asset<Model>>    models;
  static std::unordered_map<AssetId, Asset<Texture>>  textures;
  static std::unordered_map<AssetId, Asset<Material>> materials;
  static std::unordered_map<AssetId, Asset<Scene>>    scenes;
  static std::unordered_map<AssetId, Asset<Shader>>   shaders;
  static std::unordered_map<AssetId, Asset<Font>>     fonts;

  // Private constructor for singleton
  AssetManager() = default;
  
  // Hotreloading
  
  static bool                         hotreload_active;
  static FilesystemWatcher            fswatcher;
  // This thread uses fswathcer to watch for events, and writes them
  // to reload_pending. You need to call hotrealod_update to consume
  // the pending assets.  The thread is created with
  // hotreload_activate and destroyed with hotreload_deactivate. So we
  // don't have any performance penalty if we don't want to use this.
  static tenno::jthread               hotreload_thread;
  static tenno::mutex                 hotreload_pending_mutex;
  static tenno::vector<HotReloadItem> hotreload_pending;
  static std::unordered_map<std::filesystem::path, HotReloadItem> hotreload_entries;
  
};

template<typename T>
struct AssetManager::Asset
{
  T::Builder         builder;
  tenno::weak_ptr<T> ptr;
};

struct AssetManager::HotReloadItem
{
  AssetType type;
  AssetId   id;
};
  
} // namespace brenta
