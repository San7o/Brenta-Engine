// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/skybox.hpp>
#include <brenta/renderer/opengl/cubemap.hpp>

#include "../src/renderer/shaders/c/skybox_vs.c"
#include "../src/renderer/shaders/c/skybox_fs.c"

using namespace brenta;

Skybox::Skybox(const tenno::vector<std::filesystem::path>& faces)
{
  auto cubemap = tenno::make_shared<Cubemap>(faces);
  auto maybe_skybox_shader = Shader::create({
      { Shader::Type::Vertex,   skybox_vs },
      { Shader::Type::Fragment, skybox_fs }
    });
  if (!maybe_skybox_shader)
  {
    ERROR("Error creating shader");
    return;
  }
  this->material =
    tenno::make_shared<Material>(tenno::move(maybe_skybox_shader.value()));
  this->material->set_texture("skybox", cubemap, 0);
  auto skybox_mesh_builder =
    Mesh::Builder()
    .vertices({
        { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
        { glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f), glm::vec2(0.0f) },
      })
    .indices({
        2, 0, 1,
        1, 3, 2,
        4, 0, 2,
        2, 6, 4,
        1, 5, 7,
        7, 3, 1,
        4, 6, 7,
        7, 5, 4,
        2, 3, 7,
        7, 6, 2,
        0, 4, 1,
        1, 4, 5,
      })
    .texture(cubemap);
  this->mesh = tenno::make_shared<Mesh>(skybox_mesh_builder.build());
}

void Skybox::draw()
{
  if (!this->material || !this->mesh)
    return;

  glDepthFunc(GL_LEQUAL);  // Allow drawing at the far plane (1.0)
  glDepthMask(GL_FALSE);   // Don't overwrite depth with skybox pixels

  this->material->apply();
  this->mesh->draw();
  
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}
