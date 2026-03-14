// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

namespace brenta
{

class UiPass : public RenderPass
{
public:

  UiPass() = delete;
  UiPass(tenno::shared_ptr<FrameBuffer> fb,
         bool clear = false, bool set_viewport = false);

  UiPass(const UiPass& other)            = delete;
  UiPass &operator=(const UiPass& other) = delete;

  void begin() override;
  void end()   override;

  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> fb;
  bool clear        = false;
  bool set_viewport = false;
  
};

} // namespace brenta
