// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/framebuffer.hpp>
#include <brenta/ecs/ecs.hpp>

class FrameBufferResource : public viotecs::Resource
{
public:
  
  tenno::shared_ptr<brenta::FrameBuffer> fb;

  FrameBufferResource() = default;
  FrameBufferResource(tenno::shared_ptr<brenta::FrameBuffer> fb)
    : fb(fb) {}
};
