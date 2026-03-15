// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/skybox_pass.hpp>
#include <brenta/renderer/opengl/gl.hpp>
#include <brenta/renderer/material.hpp>
#include <brenta/renderer/skybox.hpp>
#include <brenta/renderer/opengl/framebuffer.hpp>

using namespace brenta;

void SkyboxPass::begin()
{
  this->fb->bind();

  if (set_viewport)
    Gl::set_viewport(0, 0, this->fb->width, this->fb->height);

  if (this->clear)
    Gl::clear();
}

void SkyboxPass::end()
{
  this->fb->unbind();
}

void SkyboxPass::execute(const Renderer::RenderData& data)
{
  if (!data.skybox) return;

  // remove the position, only keep rotation
  auto view = glm::mat4(glm::mat3(data.view));
  
  data.skybox.value()->material->shader->use();
  data.skybox.value()->material->shader->set_mat4("view",       view);
  data.skybox.value()->material->shader->set_mat4("projection", data.projection);
    
  data.skybox.value()->draw();
}
