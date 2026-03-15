// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/ui_pass.hpp>
#include <brenta/renderer/opengl/gl.hpp>

using namespace brenta;

UiPass::UiPass(tenno::shared_ptr<FrameBuffer> fb,
               bool clear, bool set_viewport)
{
  this->fb    = fb;
  this->clear = clear;
  this->set_viewport = set_viewport;
}

void UiPass::begin()
{
  this->fb->bind();

  if (this->set_viewport)
    Gl::set_viewport(0, 0, this->fb->width, this->fb->height);

  if (this->clear)
    Gl::clear();
}

void UiPass::end()
{
  this->fb->unbind();
}

void UiPass::execute(const Renderer::RenderData& data)
{
  for (auto& text : data.ui_queue)
    text.render(this->fb->width, this->fb->height);
}
