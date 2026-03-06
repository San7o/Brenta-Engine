// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/material.hpp>

namespace brenta
{

class Skybox
{
public:

  tenno::shared_ptr<Mesh>       mesh;
  tenno::shared_ptr<Material>   material;
  
  Skybox() = delete;
  Skybox(const tenno::vector<std::filesystem::path>& faces);

  void draw();

};

} // namespace brenta
