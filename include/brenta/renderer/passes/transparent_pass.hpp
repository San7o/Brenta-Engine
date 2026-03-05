// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

namespace brenta
{

class TransparentPass : public RenderPass
{
public:

  TransparentPass() = delete;
  TransparentPass(tenno::shared_ptr<FrameBuffer> fb);

  TransparentPass(const TransparentPass& other)            = delete;
  TransparentPass &operator=(const TransparentPass& other) = delete;

  void begin() override;
  void end()   override;

  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> fb;
  
};

} // namespace brenta
