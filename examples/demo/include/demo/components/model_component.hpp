// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/engine.hpp>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;

/* Model Component */
struct ModelComponent : component
{
  model mod;
  float shininess;
  brenta::types::shader_name_t shader;
  bool hasAtlas;
  int atlasSize;
  int atlasIndex;
  int elapsedFrames = 0;

  ModelComponent()
      : mod(model()), shininess(0.0f), shader("default_shader"),
        hasAtlas(false), atlasSize(0), atlasIndex(0)
  {
  }
  ModelComponent(model mod, float shininess,
                 brenta::types::shader_name_t shader, bool hasAtlas = false,
                 int atlasSize = 0, int atlasIndex = 0)
      : mod(mod), shininess(shininess), shader(shader), hasAtlas(hasAtlas),
        atlasSize(atlasSize), atlasIndex(atlasIndex)
  {
  }
};
