// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/texture.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <unordered_map>

namespace brenta
{

//
// Material
// --------
//
// A material stores a shader and the values of its uniforms. The
// renderer uses the material to set which shader program should be
// used, and which uniform values.
//
// Use apply() to set the shader and its uniforms.
//
class Material
{
public:

  class Builder;
  
  tenno::shared_ptr<Shader> shader;

  Material() = default;
  Material(tenno::shared_ptr<Shader> shader) : shader(shader) {}
  Material(Material&& other) = default;
  Material &operator=(Material&& other) = default;
  
  void apply();

  Material &set_float(const std::string &name, float val);
  Material &set_vector(const std::string &name, glm::vec3 val);
  Material &set_texture(const std::string &name,
                        tenno::shared_ptr<Texture> val, int index);

private:

  std::unordered_map<std::string, float>     floats;
  std::unordered_map<std::string, glm::vec3> vectors;
  std::unordered_map<std::string,
                     std::pair<int, tenno::shared_ptr<Texture>>>  textures;

};

class Material::Builder
{
public:

  Builder& shader(tenno::shared_ptr<Shader> shader);
  Builder& floating(const std::string &name, float val);
  Builder& vector(const std::string &name, glm::vec3 val);
  Builder& texture(const std::string &name,
                   tenno::shared_ptr<Texture> val, int index);

 // Add path to be watched for hot-reloading
  Builder &watch(const std::filesystem::path &path);
  
  Material build();
  tenno::vector<std::filesystem::path> get_watch_paths() const;  
  
private:
  
  tenno::shared_ptr<Shader> _shader;
  tenno::vector<std::pair<std::string, float>> floats;
  tenno::vector<std::pair<std::string, glm::vec3>> vectors;
  tenno::vector<std::pair<std::string,
                          std::pair<int, tenno::shared_ptr<Texture>>>> textures;

};  

} // namespace brenta
