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

Material &Material::set_float(const std::string &name, float val)
{
  this->floats[name] = val;
  return *this;
}

Material &Material::set_vector(const std::string &name, glm::vec3 val)
{
  this->vectors[name] = val;
  return *this;
}

Material &Material::set_texture(const std::string &name,
                                         tenno::shared_ptr<Texture> val,
                                         int index)
{
  this->textures[name] = std::make_pair(index, val);
  return *this;
}

Material::Builder&
Material::Builder::shader(tenno::shared_ptr<Shader> shader)
{
  this->_shader = shader;
  return *this;
}

Material::Builder&
Material::Builder::floating(const std::string &name, float val)
{
  this->floats.push_back(std::make_tuple(name, val));
  return *this;
}

Material::Builder&
Material::Builder::vector(const std::string &name, glm::vec3 val)
{
  this->vectors.push_back(std::make_tuple(name, val));
  return *this;
}

Material::Builder&
Material::Builder::texture(const std::string &name,
                           tenno::shared_ptr<Texture> val, int index)
{
  this->textures.push_back(std::make_tuple(name,
                                           std::make_tuple(index, val)));
  return *this;
}

Material Material::Builder::build()
{
  Material material = Material(this->_shader);

  for (auto& f : this->floats)
  {
    material.set_float(std::get<0>(f), std::get<1>(f));
  }
  for (auto& v : this->vectors)
  {
    material.set_vector(std::get<0>(v), std::get<1>(v));
  }
  for (auto& t : this->textures)
  {
    material.set_texture(std::get<0>(t),
                         std::get<1>(std::get<1>(t)),
                         std::get<0>(std::get<1>(t)));
  }

  return material; // copy elision
}
