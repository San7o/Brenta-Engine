// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/material.hpp>

using namespace brenta;

void Material::apply()
{
  this->shader->use();

  for (auto const& [name, val] : this->floats)
    shader->set_float(name.c_str(), val);
  for (auto const& [name, val] : this->vectors)
    shader->set_vec3(name.c_str(), val);

  for (auto const& [name, index_tex] : this->textures)
  {
    auto tex = std::get<1>(index_tex);
    tex->active_texture(std::get<0>(index_tex));
    tex->bind();
    shader->set_int(name.c_str(), std::get<0>(index_tex));
  }
  return;
}

Material &Material::set_float(std::string name, float val)
{
  this->floats[name] = val;
  return *this;
}

Material &Material::set_vector(std::string name, glm::vec3 val)
{
  this->vectors[name] = val;
  return *this;
}

Material &Material::set_texture(std::string name, std::shared_ptr<Texture> val,
                                int index)
{
  this->textures[name] = std::make_pair(index, val);
  return *this;
}
