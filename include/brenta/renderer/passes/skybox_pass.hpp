// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>

namespace brenta
{

class FrameBuffer;
  
class SkyboxPass : public RenderPass
{
public:
  
  SkyboxPass() = delete;
  SkyboxPass(tenno::shared_ptr<FrameBuffer> fb,
             bool clear = false, bool set_viewport = false)
    : fb(fb), clear(clear), set_viewport(set_viewport) {}

  void begin() override;
  void end() override;
  
  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> fb;
  bool clear        = false;
  bool set_viewport = false;
  
};

} // namespace brenta
