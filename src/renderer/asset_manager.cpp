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

tenno::shared_ptr<Texture>
AssetManager::new_texture(const AssetId& id,
                          const Texture::Config &conf)
{
  auto ptr = tenno::make_shared<Texture>(conf);
  AssetManager::textures[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Model>
AssetManager::new_model(const AssetId& id,
                        Model::Builder &&builder)
{
  auto ptr = tenno::make_shared<Model>(builder.build());
  AssetManager::models[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Shader>
AssetManager::new_shader(const AssetId& id,
                         const tenno::vector<Shader::Object> &objects)
{
  auto shader = Shader::create(objects);
  if (!shader) return nullptr;

  auto ptr = tenno::make_shared<Shader>(tenno::move(shader.value()));

  AssetManager::shaders[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Shader>
AssetManager::new_shader(const AssetId& id,
                         const GLchar **feedback_varyings,
                         int num_varyings,
                         const tenno::vector<Shader::Object> &objects)
{
  auto shader = Shader::create(feedback_varyings, num_varyings, objects);
  if (!shader) return nullptr;

  auto ptr = tenno::make_shared<Shader>(tenno::move(shader.value()));
  AssetManager::shaders[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Material>
AssetManager::new_material(const AssetId& id,
                           tenno::shared_ptr<Shader> shader)
{
  auto ptr = tenno::make_shared<Material>(shader);
  AssetManager::materials[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Scene>
AssetManager::new_scene(const AssetId& id,
                        tenno::shared_ptr<Camera> camera)
{
  auto ptr = tenno::make_shared<Scene>(camera);
  AssetManager::scenes[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Font>
AssetManager::new_font(const AssetId& id,
                       const std::filesystem::path &path, int size)
{
  auto ptr = tenno::make_shared<Font>(path, size);
  AssetManager::fonts[id] = ptr;
  return ptr;
}

tenno::shared_ptr<Texture>  AssetManager::get_texture(const AssetId& id)
{
  if (!AssetManager::textures.contains(id)) return nullptr;

  if (tenno::shared_ptr<Texture> ptr = AssetManager::textures[id].lock())
    return ptr;

  return nullptr;
}

tenno::shared_ptr<Model>    AssetManager::get_model(const AssetId& id)
{
  if (!AssetManager::models.contains(id)) return nullptr;

  if (tenno::shared_ptr<Model> ptr = AssetManager::models[id].lock())
    return ptr;

  return nullptr;
}
  
tenno::shared_ptr<Shader>   AssetManager::get_shader(const AssetId& id)
{
  if (!AssetManager::shaders.contains(id)) return nullptr;

  if (tenno::shared_ptr<Shader> ptr = AssetManager::shaders[id].lock())
    return ptr;

  return nullptr;
}

tenno::shared_ptr<Material> AssetManager::get_material(const AssetId& id)
{
  if (!AssetManager::materials.contains(id)) return nullptr;

  if (tenno::shared_ptr<Material> ptr = AssetManager::materials[id].lock())
    return ptr;

  return nullptr;
}

tenno::shared_ptr<Scene> AssetManager::get_scene(const AssetId& id)
{
  if (!AssetManager::scenes.contains(id)) return nullptr;

  if (tenno::shared_ptr<Scene> ptr = AssetManager::scenes[id].lock())
    return ptr;

  return nullptr;
}

tenno::shared_ptr<Font> AssetManager::get_font(const AssetId& id)
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
