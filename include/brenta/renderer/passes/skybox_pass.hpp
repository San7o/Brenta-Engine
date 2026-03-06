// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

namespace brenta
{

class SkyboxPass : public RenderPass
{
public:
  
  SkyboxPass() = delete;
  SkyboxPass(tenno::shared_ptr<FrameBuffer> fb)
    : fb(fb) {}

  void begin() override;
  void end() override;
  
  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> fb;
  
};

} // namespace brenta
