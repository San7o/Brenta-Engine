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

  std::shared_ptr<Shader> shader;

  Material() = default;
  Material(std::shared_ptr<Shader> shader) : shader(shader) {}
  Material(Material&& other) = default;
  Material &operator=(Material&& other) = default;
  
  void apply();

  Material &set_float(std::string name, float val);
  Material &set_vector(std::string name, glm::vec3 val);
  Material &set_texture(std::string name, std::shared_ptr<Texture> val, int index);

private:

  std::unordered_map<std::string, float>     floats;
  std::unordered_map<std::string, glm::vec3> vectors;
  std::unordered_map<std::string,
                     std::pair<int, std::shared_ptr<Texture>>>  textures;

};
  
} // namespace brenta
