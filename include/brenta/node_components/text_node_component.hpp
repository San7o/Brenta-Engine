// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/node.hpp>
#include <brenta/text.hpp>

#include <tenno/memory.hpp>

namespace brenta
{

class TextNodeComponent : public NodeComponent
{
public:

  Text text;

  TextNodeComponent() = default;
  TextNodeComponent(const Text& text)
    : text(text) {}
  
  void update(float delta_time) override;
  void draw(const glm::mat4& world_matrix) override;
  
};

} // namespace brenta
