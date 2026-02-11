// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/mesh.hpp>
#include <brenta/logger.hpp>
#include <iostream>

using namespace brenta;

const Mesh::Config Mesh::default_config = {
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

Mesh::Mesh(Config&& conf)
{
  this->vertices = conf.vertices;
  this->indices = conf.indices;
  this->textures = std::move(conf.textures);
  this->wrapping = conf.wrapping;
  this->filtering_min = conf.filtering_min;
  this->filtering_mag = conf.filtering_mag;
  this->has_mipmap = conf.has_mipmap;
  this->mipmap_min = conf.mipmap_min;
  this->mipmap_mag = conf.mipmap_max;

  this->init();
  DEBUG("mesh: created");
  return;
}

Mesh::~Mesh()
{
  DEBUG("mesh: deleted");
  return;
}

void Mesh::init()
{
  this->vao.init();
  this->vao.bind();
  this->vbo.init(GL_ARRAY_BUFFER);
  this->ebo.init(GL_ELEMENT_ARRAY_BUFFER);
  this->vbo.copy_vertices(this->vertices.size() * sizeof(Vertex),
                          &this->vertices[0], GL_STATIC_DRAW);
  this->ebo.copy_indices(this->indices.size() * sizeof(unsigned int),
                         &this->indices[0], GL_STATIC_DRAW);
  this->vao.set_vertex_data(this->vbo, 0, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex), (void *) 0);
  this->vao.set_vertex_data(this->vbo, 1, 3, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex),
                            (void *) offsetof(Vertex, normal));
  this->vao.set_vertex_data(this->vbo, 2, 2, GL_FLOAT, GL_FALSE,
                            sizeof(Vertex),
                            (void *) offsetof(Vertex, tex_coords));

  Gl::bind_vertex_array(0);
  return;
}

void Mesh::draw(Shader::Name shader_name) const
{
  if (this->vao.get_id() == 0)
  {
    ERROR("Mesh::draw: not initialized");
    return;
  }

  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for (unsigned int i = 0; i < this->textures.size(); i++)
  {
    Texture::active_texture(GL_TEXTURE0 + i);

    std::string number;
    std::string name;
    auto type = textures[i]->type;
    switch (type)
    {
    case Texture::Type::Diffuse:
      name   = "texture_diffuse";
      number = std::to_string(diffuseNr++);
      break;
    case Texture::Type::Specular:
      name   = "texture_specular";
      number = std::to_string(specularNr++);
      break;
    default:
      number = "0";
      break;
    }
    
    Shader::set_int(shader_name, ("material." + name + number).c_str(), i);
    textures[i]->bind(GL_TEXTURE_2D, this->wrapping,
                     this->filtering_min, this->filtering_mag,
                     this->has_mipmap, this->mipmap_min, this->mipmap_mag);
  }
  
  Texture::active_texture(GL_TEXTURE0);

  // draw mesh
  this->vao.bind();
  Gl::draw_elements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  this->vao.unbind();

  Texture::active_texture(GL_TEXTURE0);
  return;
}

//
// Builder functions
//

Mesh::Builder &Mesh::Builder::vertices(std::vector<Vertex> vertices)
{
  this->conf.vertices = vertices;
  return *this;
}

Mesh::Builder &Mesh::Builder::indices(std::vector<unsigned int> indices)
{
  this->conf.indices = indices;
  return *this;
}

Mesh::Builder &Mesh::Builder::textures(std::vector<std::shared_ptr<Texture>> textures)
{
  this->conf.textures = std::move(textures);
  return *this;
}

Mesh::Builder &Mesh::Builder::wrapping(GLint wrapping)
{
  this->conf.wrapping = wrapping;
  return *this;
}

Mesh::Builder &Mesh::Builder::filtering_min(GLint filtering_min)
{
  this->conf.filtering_min = filtering_min;
  return *this;
}

Mesh::Builder &Mesh::Builder::filtering_mag(GLint filtering_mag)
{
  this->conf.filtering_mag = filtering_mag;
  return *this;
}

Mesh::Builder &Mesh::Builder::has_mipmap(GLboolean has_mipmap)
{
  this->conf.has_mipmap = has_mipmap;
  return *this;
}

Mesh::Builder &Mesh::Builder::mipmap_min(GLint mipmap_min)
{
  this->conf.mipmap_min = mipmap_min;
  return *this;
}

Mesh Mesh::Builder::build()
{
  return Mesh(std::move(this->conf));
}
