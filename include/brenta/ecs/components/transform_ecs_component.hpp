// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/transform.hpp>
#include <brenta/ecs/ecs.hpp>

namespace brenta
{

class TransformEcsComponent : public viotecs::Component
{
public:

  Transform transform   = {};

  TransformEcsComponent() = default;
  TransformEcsComponent(Transform& t)
    : transform(t) {}
  
};

} // namespace brenta
