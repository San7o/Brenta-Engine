// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/mesh.hpp>
#include <iostream>

using namespace brenta;

const mesh::config mesh::default_config = {
  {},
  {},
  {},
  GL_REPEAT,
  GL_NEAREST,
  GL_LINEAR,
  GL_TRUE,
  GL_LINEAR_MIPMAP_LINEAR,
  GL_LINEAR,
};

mesh::mesh(config conf)
{
  this->vao.init();
  this->vao.bind();
  this->vertices = conf.vertices;
  this->indices = conf.indices;
  this->textures = conf.textures;
  this->vbo = types::buffer(GL_ARRAY_BUFFER);
  this->ebo = types::buffer(GL_ELEMENT_ARRAY_BUFFER);
  this->wrapping = conf.wrapping;
  this->filtering_min = conf.filtering_min;
  this->filtering_mag = conf.filtering_mag;
  this->has_mipmap = conf.has_mipmap;
  this->mipmap_min = conf.mipmap_min;
  this->mipmap_mag = conf.mipmap_max;

  setup_mesh();
}

void mesh::draw(types::shader_name_t shader_name)
{
  if (this->vao.get_vao() == 0)
  {
    ERROR("mesh::draw: not initialized");
    return;
  }

  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < this->textures.size(); i++)
  {
    texture::active_texture(GL_TEXTURE0 + i);
    std::string number;
    std::string name = textures[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++);
    shader::set_int(shader_name, ("material." + name + number).c_str(), i);
    texture::bind_texture(GL_TEXTURE_2D, textures[i].id, this->wrapping,
                          this->filtering_min, this->filtering_mag,
                          this->has_mipmap, this->mipmap_min, this->mipmap_mag);
  }
  texture::active_texture(GL_TEXTURE0);

  // draw mesh
  this->vao.bind();
  gl::draw_elements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  this->vao.unbind();

  texture::active_texture(GL_TEXTURE0);
}

void mesh::setup_mesh()
{
  this->vbo.copy_vertices(this->vertices.size() * sizeof(types::vertex),
                          &this->vertices[0], GL_STATIC_DRAW);
  this->ebo.copy_indices(this->indices.size() * sizeof(unsigned int),
                         &this->indices[0], GL_STATIC_DRAW);
  this->vao.set_vertex_data(this->vbo, 0, 3, GL_FLOAT, GL_FALSE,
                            sizeof(types::vertex), (void *) 0);
  this->vao.set_vertex_data(this->vbo, 1, 3, GL_FLOAT, GL_FALSE,
                            sizeof(types::vertex),
                            (void *) offsetof(types::vertex, normal));
  this->vao.set_vertex_data(this->vbo, 2, 2, GL_FLOAT, GL_FALSE,
                            sizeof(types::vertex),
                            (void *) offsetof(types::vertex, tex_coords));

  gl::bind_vertex_array(0);
}

//
// Builder functions
//

mesh::builder &mesh::builder::vertices(std::vector<types::vertex> vertices)
{
  this->conf.vertices = vertices;
  return *this;
}

mesh::builder &mesh::builder::indices(std::vector<unsigned int> indices)
{
  this->conf.indices = indices;
  return *this;
}

mesh::builder &mesh::builder::textures(std::vector<types::texture> textures)
{
  this->conf.textures = textures;
  return *this;
}

mesh::builder &mesh::builder::wrapping(GLint wrapping)
{
  this->conf.wrapping = wrapping;
  return *this;
}

mesh::builder &mesh::builder::filtering_min(GLint filtering_min)
{
  this->conf.filtering_min = filtering_min;
  return *this;
}

mesh::builder &mesh::builder::filtering_mag(GLint filtering_mag)
{
  this->conf.filtering_mag = filtering_mag;
  return *this;
}

mesh::builder &mesh::builder::has_mipmap(GLboolean has_mipmap)
{
  this->conf.has_mipmap = has_mipmap;
  return *this;
}

mesh::builder &mesh::builder::mipmap_min(GLint mipmap_min)
{
  this->conf.mipmap_min = mipmap_min;
  return *this;
}

mesh mesh::builder::build()
{
  return mesh(this->conf);
}
