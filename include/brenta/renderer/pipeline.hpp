// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/renderer.hpp>

#include <tenno/vector.hpp>
#include <tenno/memory.hpp>

#include <optional>

namespace brenta
{

// Virtual render pass class  
class RenderPass
{
public:

  virtual ~RenderPass() = default;

  // Bind a framebuffer
  virtual void begin() = 0;
  virtual void end()   = 0;
  
  // Draw to the framebuffer
  virtual void execute(const Renderer::RenderData& data) = 0;
  
};

//
// Render Pipeline
// ---------------
//
// A pipeline is a series of passes in order. Each pass has a
// reference to all data, and it draws to its framebuffer.
class RenderPipeline
{
public:

  tenno::vector<tenno::shared_ptr<RenderPass>> passes;
  
  RenderPipeline() = default;

  void add_pass(tenno::shared_ptr<RenderPass> pass);
  template<typename P, typename ...Args>
  void add_pass(Args... args)
  {
    passes.push_back(tenno::make_shared<P>(args...));
  }
  
  void execute(const Renderer::RenderData& data);

  // Get a standard pipeline that draws to the default framebuffer.
  static tenno::shared_ptr<RenderPipeline> create_default();
  
};

} // namespace brenta
