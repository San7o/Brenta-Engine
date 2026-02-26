// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/opengl/ubo.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

void Ubo::init(Shader& shader,
               std::string uniform_name,
               unsigned int binding_point,
               size_t size)
{
  GLuint block_index =
    glGetUniformBlockIndex(shader.get_id(), uniform_name.c_str());
  if (block_index == GL_INVALID_INDEX)
  {
    ERROR("Particles::init: error settings uniform block index");
    return;
  }
  
  this->target = Buffer::Target::Uniform;
  glGenBuffers(1, &id);
  
  this->bind();
  glUniformBlockBinding(shader.get_id(), block_index, 3);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, id);
  glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, id, 0,
                    size);

  EVENT(Logger::Event::Lifetime, "ubo: initialized");
  this->unbind();
  return;
}
