// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/skybox.hpp>
#include <brenta/ecs/ecs.hpp>

class SkyboxResource : public viotecs::Resource
{
public:
  
  tenno::shared_ptr<brenta::Skybox> skybox;

  SkyboxResource() = default;
  SkyboxResource(tenno::shared_ptr<brenta::Skybox> s)
    : skybox(s) {}
};
