// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/skybox_pass.hpp>

using namespace brenta;

void SkyboxPass::begin()
{
  fb->bind();
}

void SkyboxPass::end()
{
  fb->unbind();
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
