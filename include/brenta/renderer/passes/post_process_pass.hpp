// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/pipeline.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

namespace brenta
{

class PostProcessPass : public RenderPass
{
public:

  PostProcessPass() = delete;
  PostProcessPass(tenno::shared_ptr<FrameBuffer> input_fb,
                  tenno::shared_ptr<FrameBuffer> output_fb,
                  tenno::shared_ptr<Material>    pp_material,
                  bool clear = false, bool set_viewport = false);

  PostProcessPass(const PostProcessPass& other)            = delete;
  PostProcessPass &operator=(const PostProcessPass& other) = delete;

  void begin() override;
  void end()   override;

  void execute(const Renderer::RenderData& data) override;

private:

  tenno::shared_ptr<FrameBuffer> input_fb;
  tenno::shared_ptr<FrameBuffer> output_fb;
  tenno::shared_ptr<Model>       screen_quad;
  bool clear        = false;
  bool set_viewport = false;
  
};

} // namespace brenta
