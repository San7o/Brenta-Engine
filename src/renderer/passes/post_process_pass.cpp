// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/passes/post_process_pass.hpp>
#include <brenta/renderer/opengl/gl.hpp>

using namespace brenta;

PostProcessPass::PostProcessPass(tenno::shared_ptr<FrameBuffer> input_fb,
                                 tenno::shared_ptr<FrameBuffer> output_fb,
                                 tenno::shared_ptr<Material>    pp_material,
                                 bool clear, bool set_viewport)
{
  this->input_fb     = input_fb;
  this->output_fb    = output_fb;
  this->clear        = clear;
  this->set_viewport = set_viewport;

  auto screen_quad_builder =
    Model::Builder()
    .mesh(Mesh::Builder()
          .shape(Mesh::Shape::Square))
    .material(pp_material);
  this->screen_quad =
    tenno::make_shared<Model>(screen_quad_builder);
}

void PostProcessPass::begin()
{
  this->output_fb->bind();

  if (this->set_viewport)
    Gl::set_viewport(0, 0, this->output_fb->width, this->output_fb->height);
  
  if (this->clear)
    Gl::clear();
}

void PostProcessPass::end()
{
  this->output_fb->unbind();
}

void PostProcessPass::execute(const Renderer::RenderData& data)
{
  (void) data;
  
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, input_fb->texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  this->screen_quad->draw();
}
