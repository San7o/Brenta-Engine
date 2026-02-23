// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/model.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <viotecs/viotecs.hpp>

using namespace brenta;
using namespace viotecs;

struct ModelComponent : Component
{
  Model        mod;
  float        shininess;
  std::shared_ptr<Shader> shader;
  bool         hasAtlas;
  int          atlasSize;
  int          atlasIndex;
  int          elapsedFrames = 0;

  ModelComponent()
      : mod(Model()), shininess(0.0f), shader(),
        hasAtlas(false), atlasSize(0), atlasIndex(0)
  {
  }
  ModelComponent(Model &&mod, float shininess,
                 std::shared_ptr<Shader> shader, bool hasAtlas = false,
                 int atlasSize = 0, int atlasIndex = 0)
    : mod(std::move(mod)), shininess(shininess), shader(shader),
      hasAtlas(hasAtlas), atlasSize(atlasSize), atlasIndex(atlasIndex)
  {
  }
};
