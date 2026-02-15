// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/renderer/opengl/buffer.hpp>

namespace brenta
{
  
//
// Uniform buffer objects
// ----------------------
//
// These are used to set multiple uniforms with a single command
//
// You bind / unbind dem and write data as usual, where the data is
// a struct with the same format as the one in the shader, which
// can access the uniforms at the [binding_point] layout.
class Ubo : public Buffer
{
public:

  Ubo() : Buffer(Buffer::Target::Uniform) {}
  
  // Initialized the UBO
  void init(Shader& shader,
            std::string uniform_name,
            unsigned int binding_point,
            size_t size);  
};
  
} // namespace brenta
