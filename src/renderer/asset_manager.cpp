// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static storage
//

std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Model>>   AssetManager::models;
std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Texture>>  AssetManager::textures;
std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Material>> AssetManager::materials;
std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Scene>>    AssetManager::scenes;
std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Shader>>   AssetManager::shaders;
std::unordered_map<AssetManager::AssetId,
                   AssetManager::Asset<Font>>     AssetManager::fonts;

bool                                 AssetManager::hotreload_active = false;
FilesystemWatcher                    AssetManager::fswatcher;
tenno::jthread                       AssetManager::hotreload_thread;
tenno::mutex                         AssetManager::hotreload_pending_mutex;
tenno::vector<AssetManager::HotReloadItem> AssetManager::hotreload_pending;
std::unordered_map<std::filesystem::path, AssetManager::HotReloadItem>
AssetManager::hotreload_entries;

//
// Member functions
//

template<>
tenno::shared_ptr<Model>
AssetManager::new_asset<Model>(const AssetId& id,
                               Model::Builder &builder)
{
  auto ptr = tenno::make_shared<Model>(builder.build());
  ptr.set_cache(false);
  AssetManager::models[id] = {builder, ptr};

  auto watch_paths = builder.get_watch_paths();
  for (auto& watch_path : watch_paths)
  {
    AssetManager::hotreload_entries[watch_path] = {
      .type = AssetType::Model,
      .id   = id,
    };
  }
  
  return ptr;
}

template<>
tenno::shared_ptr<Texture>
AssetManager::new_asset<Texture>(const AssetId& id,
                                 Texture::Builder &builder)
{
  auto ptr = tenno::make_shared<Texture>(builder.build());
  ptr.set_cache(false);
  AssetManager::textures[id] = {builder, ptr};

  auto watch_paths = builder.get_watch_paths();
  for (auto& watch_path : watch_paths)
  {
    AssetManager::hotreload_entries[watch_path] = {
      .type = AssetType::Texture,
      .id   = id,
    };
  }
  
  return ptr;
}

template<>
tenno::shared_ptr<Material>
AssetManager::new_asset<Material>(const AssetId& id,
                                  Material::Builder &builder)
{
  auto ptr = tenno::make_shared<Material>(builder.build());
  ptr.set_cache(false);
  AssetManager::materials[id] = {builder, ptr};
  
  return ptr;
}

template<>
tenno::shared_ptr<Font>
AssetManager::new_asset<Font>(const AssetId& id,
                              Font::Builder &builder)
{
  auto ptr = tenno::make_shared<Font>(builder.build());
  ptr.set_cache(false);
  AssetManager::fonts[id] = {builder, ptr};

  auto watch_paths = builder.get_watch_paths();
  for (auto& watch_path : watch_paths)
  {
    AssetManager::hotreload_entries[watch_path] = {
      .type = AssetType::Font,
      .id   = id,
    };
  }
  
  return ptr;
}

template<>
tenno::shared_ptr<Scene>
AssetManager::new_asset<Scene>(const AssetId& id,
                               Scene::Builder &builder)
{
  auto ptr = tenno::make_shared<Scene>(builder.build());
  ptr.set_cache(false);
  AssetManager::scenes[id] = {builder, ptr};
  
  return ptr;
}

template<>
tenno::shared_ptr<Shader>
AssetManager::new_asset<Shader>(const AssetId& id,
                                Shader::Builder &builder)
{
  auto maybe_shader = builder.build();
  if (!maybe_shader) return nullptr;

  auto shader =
    tenno::make_shared<Shader>(tenno::move(maybe_shader.value()));
  shader.set_cache(false);

  AssetManager::shaders[id] = {builder, shader};
  
  // TODO: hotreloading
  
  return shader;
}

template<>
tenno::shared_ptr<Texture>  AssetManager::get<Texture>(const AssetId& id)
{
  if (!AssetManager::textures.contains(id)) return nullptr;

  if (tenno::shared_ptr<Texture> ptr =
      AssetManager::textures[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Model> AssetManager::get<Model>(const AssetId& id)
{
  if (!AssetManager::models.contains(id)) return nullptr;

  if (tenno::shared_ptr<Model> ptr =
      AssetManager::models[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Shader> AssetManager::get<Shader>(const AssetId& id)
{
  if (!AssetManager::shaders.contains(id)) return nullptr;

  if (tenno::shared_ptr<Shader> ptr =
      AssetManager::shaders[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Material> AssetManager::get<Material>(const AssetId& id)
{
  if (!AssetManager::materials.contains(id)) return nullptr;

  if (tenno::shared_ptr<Material> ptr =
      AssetManager::materials[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Scene> AssetManager::get<Scene>(const AssetId& id)
{
  if (!AssetManager::scenes.contains(id)) return nullptr;

  if (tenno::shared_ptr<Scene> ptr =
      AssetManager::scenes[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Font> AssetManager::get<Font>(const AssetId& id)
{
  if (!AssetManager::fonts.contains(id)) return nullptr;

  if (tenno::shared_ptr<Font> ptr =
      AssetManager::fonts[id].ptr.lock())
    return ptr;

  return nullptr;
}

template<>
bool AssetManager::reload<Model>(const AssetId& id)
{
  if (!AssetManager::models.contains(id)) return false;

  Asset<Model>& asset = AssetManager::models[id];
  tenno::shared_ptr<Model> new_model = tenno::make_shared<Model>(asset.builder.build());
  asset.ptr.swap_ptr(new_model);
  
  return true;
}

template<>
bool AssetManager::reload<Texture>(const AssetId& id)
{
  if (!AssetManager::textures.contains(id)) return false;

  Asset<Texture>& asset = AssetManager::textures[id];
  tenno::shared_ptr<Texture> new_texture = tenno::make_shared<Texture>(asset.builder.build());
  asset.ptr.swap_ptr(new_texture);
  
  return true;
}

template<>
bool AssetManager::reload<Material>(const AssetId& id)
{
  if (!AssetManager::materials.contains(id)) return false;

  Asset<Material>& asset = AssetManager::materials[id];
  tenno::shared_ptr<Material> new_material = tenno::make_shared<Material>(asset.builder.build());
  asset.ptr.swap_ptr(new_material);
  
  return true;
}

template<>
bool AssetManager::reload<Font>(const AssetId& id)
{
  if (!AssetManager::fonts.contains(id)) return false;

  Asset<Font>& asset = AssetManager::fonts[id];
  tenno::shared_ptr<Font> new_font = tenno::make_shared<Font>(asset.builder.build());
  asset.ptr.swap_ptr(new_font);
  
  return true;
}

template<>
bool AssetManager::reload<Scene>(const AssetId& id)
{
  if (!AssetManager::scenes.contains(id)) return false;

  Asset<Scene>& asset = AssetManager::scenes[id];
  tenno::shared_ptr<Scene> new_scene = tenno::make_shared<Scene>(asset.builder.build());
  asset.ptr.swap_ptr(new_scene);
  
  return true;
}

template<>
bool AssetManager::reload<Shader>(const AssetId& id)
{
  if (!AssetManager::shaders.contains(id)) return false;

  Asset<Shader>& asset = AssetManager::shaders[id];
  auto maybe_shader = asset.builder.build();
  if (!maybe_shader) return false;
  
  tenno::shared_ptr<Shader> new_shader =
    tenno::make_shared<Shader>(tenno::move(*new_shader));
  asset.ptr.swap_ptr(new_shader);
  
  return true;
}

void AssetManager::clear()
{
  AssetManager::models.clear();
  AssetManager::textures.clear();
  AssetManager::materials.clear();
  AssetManager::scenes.clear();
  AssetManager::shaders.clear();
  AssetManager::fonts.clear();
  
  return;
}

void AssetManager::hotreload_activate()
{
  if (AssetManager::hotreload_active) return;

  AssetManager::hotreload_active = true;

  AssetManager::hotreload_thread = tenno::jthread([](){
    DEBUG("AssetManager: created hotreloading thread");

    AssetManager::fswatcher.init();

    for (auto& [path, _] : AssetManager::hotreload_entries)
    {
      AssetManager::fswatcher.add(path, {FilesystemWatcher::Event::Modify});
      DEBUG("AssetManager: hotreloading setup {}", path.string());
    }

    while(AssetManager::hotreload_active)
    {
      auto maybe_event = AssetManager::fswatcher.watch();
      if (!maybe_event) continue;

      auto event = *maybe_event;
      if (AssetManager::hotreload_entries.contains(event))
      {
        tenno::lock_guard<tenno::mutex> lock(AssetManager::hotreload_pending_mutex);
        AssetManager::hotreload_pending.push_back(AssetManager::hotreload_entries[event]);
        DEBUG("AssetManager: hotreloading detected change in {}", event.string());
      }
    }
    
    return;
  });

  DEBUG("AssetManager: hotreloading activated");
}

void AssetManager::hotreload_deactivate()
{
  if (!AssetManager::hotreload_active) return;
  
  AssetManager::hotreload_active = false;
  
  AssetManager::hotreload_thread.request_stop();
  AssetManager::fswatcher.destroy();

  DEBUG("AssetManager: hotreloading deactivated");
}

void AssetManager::hotreload_update()
{
  if (!AssetManager::hotreload_active) return;
  
  tenno::lock_guard<tenno::mutex> lock(AssetManager::hotreload_pending_mutex);

  for (auto& item : AssetManager::hotreload_pending)
  {
    DEBUG("AssetManager: hotreloading now {}", item.id);
    switch(item.type)
    {
    case AssetType::Model:    AssetManager::reload<Model>(item.id);    break;
    case AssetType::Texture:  AssetManager::reload<Texture>(item.id);  break;
    case AssetType::Shader:   AssetManager::reload<Shader>(item.id);   break;
    case AssetType::Font:     AssetManager::reload<Font>(item.id);     break;
    default: break;
    }
  }

  AssetManager::hotreload_pending.clear();
}
