// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>

namespace brenta
{

class Material
{
public:

  Shader::Name shader;

  Material() = default;
  Material(Shader::Name name) : shader(name) {}
  
  // TODO
  
};
  
} // namespace brenta
