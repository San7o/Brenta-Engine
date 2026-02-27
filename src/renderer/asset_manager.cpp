// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/asset_manager.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

// Static members

std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Model>>    AssetManager::models;
std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Texture>>  AssetManager::textures;
std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Material>> AssetManager::materials;
std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Scene>>    AssetManager::scenes;
std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Shader>>   AssetManager::shaders;
std::unordered_map<AssetManager::AssetId, tenno::weak_ptr<Font>>     AssetManager::fonts;

//
// Member functions
//

template<>
tenno::shared_ptr<Model>
AssetManager::new_asset<Model>(const AssetId& id,
                               Model::Builder &builder)
{
  auto ptr = tenno::make_shared<Model>(builder.build());
  AssetManager::models[id] = ptr;
  return ptr;
}

template<>
tenno::shared_ptr<Texture>
AssetManager::new_asset<Texture>(const AssetId& id,
                                 Texture::Builder &builder)
{
  auto ptr = tenno::make_shared<Texture>(builder.build());
  AssetManager::textures[id] = ptr;
  return ptr;
}

template<>
tenno::shared_ptr<Material>
AssetManager::new_asset<Material>(const AssetId& id,
                                  Material::Builder &builder)
{
  auto ptr = tenno::make_shared<Material>(builder.build());
  AssetManager::materials[id] = ptr;
  return ptr;
}

template<>
tenno::shared_ptr<Font>
AssetManager::new_asset<Font>(const AssetId& id,
                              Font::Builder &builder)
{
  auto ptr = tenno::make_shared<Font>(builder.build());
  AssetManager::fonts[id] = ptr;
  return ptr;
}

template<>
tenno::shared_ptr<Scene>
AssetManager::new_asset<Scene>(const AssetId& id,
                               Scene::Builder &builder)
{
  auto ptr = tenno::make_shared<Scene>(builder.build());
  AssetManager::scenes[id] = ptr;
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

  AssetManager::shaders[id] = shader;
  return shader;
}

template<>
tenno::shared_ptr<Texture>  AssetManager::get<Texture>(const AssetId& id)
{
  if (!AssetManager::textures.contains(id)) return nullptr;

  if (tenno::shared_ptr<Texture> ptr = AssetManager::textures[id].lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Model> AssetManager::get<Model>(const AssetId& id)
{
  if (!AssetManager::models.contains(id)) return nullptr;

  if (tenno::shared_ptr<Model> ptr = AssetManager::models[id].lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Shader> AssetManager::get<Shader>(const AssetId& id)
{
  if (!AssetManager::shaders.contains(id)) return nullptr;

  if (tenno::shared_ptr<Shader> ptr = AssetManager::shaders[id].lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Material> AssetManager::get<Material>(const AssetId& id)
{
  if (!AssetManager::materials.contains(id)) return nullptr;

  if (tenno::shared_ptr<Material> ptr = AssetManager::materials[id].lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Scene> AssetManager::get<Scene>(const AssetId& id)
{
  if (!AssetManager::scenes.contains(id)) return nullptr;

  if (tenno::shared_ptr<Scene> ptr = AssetManager::scenes[id].lock())
    return ptr;

  return nullptr;
}

template<>
tenno::shared_ptr<Font> AssetManager::get<Font>(const AssetId& id)
{
  if (!AssetManager::fonts.contains(id)) return nullptr;

  if (tenno::shared_ptr<Font> ptr = AssetManager::fonts[id].lock())
    return ptr;

  return nullptr;
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
