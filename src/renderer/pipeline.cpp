// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/window.hpp>
#include <brenta/renderer/pipeline.hpp>

#include <brenta/renderer/passes/opaque_pass.hpp>
#include <brenta/renderer/passes/transparent_pass.hpp>
#include <brenta/renderer/passes/ui_pass.hpp>
#include <brenta/renderer/passes/skybox_pass.hpp>

using namespace brenta;

void RenderPipeline::add_pass(tenno::shared_ptr<RenderPass> pass)
{
  this->passes.push_back(pass);
}

void RenderPipeline::execute(const Renderer::RenderData& data)
{
  for (auto& pass : this->passes)
  {
    pass->begin();
    pass->execute(data);
    pass->end();
  }
}

tenno::shared_ptr<RenderPipeline> RenderPipeline::create_default()
{
  auto fb       = Window::framebuffer;
  auto pipeline = tenno::make_shared<RenderPipeline>();

  pipeline->add_pass<OpaquePass>(fb, true);
  pipeline->add_pass<TransparentPass>(fb);
  pipeline->add_pass<SkyboxPass>(fb);
  pipeline->add_pass<UiPass>(fb);

  return pipeline;
}
