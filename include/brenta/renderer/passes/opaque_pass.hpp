// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

namespace brenta
{

class OpaquePass : public RenderPass
{
public:

  OpaquePass() = delete;
  OpaquePass(tenno::shared_ptr<FrameBuffer> fb);

  OpaquePass(const OpaquePass& other)            = delete;
  OpaquePass &operator=(const OpaquePass& other) = delete;

  void begin() override;
  void end()   override;

  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> fb;
  
};

} // namespace brenta
