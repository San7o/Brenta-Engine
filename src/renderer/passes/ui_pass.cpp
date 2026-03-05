// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/ui_pass.hpp>

using namespace brenta;

UiPass::UiPass(tenno::shared_ptr<FrameBuffer> fb)
{
  this->fb = fb;
}

void UiPass::begin()
{
  this->fb->bind();
}

void UiPass::end()
{
  this->fb->unbind();
}

void UiPass::execute(const Renderer::RenderData& data)
{
  for (auto& text : data.ui_queue)
    text.render();
}
