// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/fswatcher.hpp>

#include <tenno/memory.hpp>
#include <tenno/thread.hpp>

#include <unordered_map>

namespace brenta
{

class Gui;
class Texture;
class Model;
class Material;
class Scene;
class Font;
class Sound;
class SoundAsset;
class Shader;
  
//
// The central Asset Manager
// -------------------------
//
// The asset manager is an interface to create and hold certain
// onjects. The api is the same for all types of objects.
//
// All objects have an identifier which you provide when you
// create a new object with the `new_asset<T>(...)` function. You
// need to pass the identifier (a string) and a builder for `T`.
//
// After creation, you can get the object with `get<T>(id)` and reload
// it with `reload<T>(id)`. If you enable hot reloading, this can be
// done automatically when a change is detected in the source files in
// the filesystem.
//
// The asset manager holds either weak references, or it owns the
// data. Weak references are kept in memory as long as their owner
// exists, the asset manager does not partecipate in its ownership but
// it provides a way to create more references to that object.  This
// can be useful for loading levels, as all resources are deallocated
// automatically when you change scene.
//
class AssetManager
{
public:

  using  AssetId = std::string;
  
  template<typename T>
  struct Asset;
  template<typename T>
  struct AssetOwned;
  struct HotReloadItem;

  friend class Gui;

  enum class AssetType
  {
    Model,
    Texture,
    Material,
    Scene,
    Shader,
    Font,
    Sound,
  };

  //
  // AssetManager API
  // ----------------
  //
  // You can use the following functions with their specialization,
  // for example:
  //
  //  tenno::shared_ptr<Model> new_asset<Model>("my_model", builder);
  //  reload<Model>("my_model");
  //
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

  static std::unordered_map<AssetId, Asset<Model>>           models;
  static std::unordered_map<AssetId, Asset<Texture>>         textures;
  static std::unordered_map<AssetId, Asset<Material>>        materials;
  static std::unordered_map<AssetId, Asset<Shader>>          shaders;
  static std::unordered_map<AssetId, Asset<Font>>            fonts;
  static std::unordered_map<AssetId, AssetOwned<Scene>>      scenes;
  static std::unordered_map<AssetId, AssetOwned<SoundAsset>> sound_assets;


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
  static std::unordered_map<std::filesystem::path,
                            HotReloadItem> hotreload_entries;
  
};

template<typename T>
struct AssetManager::Asset
{
  T::Builder         builder;
  tenno::weak_ptr<T> ptr;
};

template<typename T>
struct AssetManager::AssetOwned
{
  T::Builder           builder;
  tenno::shared_ptr<T> ptr;
};

struct AssetManager::HotReloadItem
{
  AssetType type;
  AssetId   id;
};
  
} // namespace brenta
