// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

Mesh::Mesh(Config&& conf)
{
  this->vertices      = conf.vertices;
  this->indices       = conf.indices;
  this->textures      = std::move(conf.textures);
  
  this->init();
  EVENT(Logger::Event::Lifetime, "mesh: created");
  return;
}

Mesh::~Mesh()
{
  EVENT(Logger::Event::Lifetime, "mesh: deleted");
  return;
}

void Mesh::init()
{
  this->vao.init();
  this->vao.bind();
  
  this->vbo.init(Buffer::Target::Array);
  this->vbo.bind();
  this->vbo.copy_data(&this->vertices[0],
                      this->vertices.size() * sizeof(Vertex),
                      Buffer::DataUsage::StaticDraw);

  this->vao.link_buffer(this->vbo, 0, 3, Gl::Type::Float, Gl::False,
                        sizeof(Vertex), (void *) 0);
  this->vao.link_buffer(this->vbo, 1, 3, Gl::Type::Float, Gl::False,
                        sizeof(Vertex),
                        (void *) offsetof(Vertex, normal));
  this->vao.link_buffer(this->vbo, 2, 2, Gl::Type::Float, Gl::False,
                        sizeof(Vertex),
                        (void *) offsetof(Vertex, tex_coords));
  
  this->ebo.init(Buffer::Target::ElementArray);
  this->ebo.bind();
  this->ebo.copy_data(&this->indices[0],
                      this->indices.size() * sizeof(unsigned int),
                      Buffer::DataUsage::StaticDraw);

  this->vao.unbind();
  this->vbo.unbind();
  this->ebo.unbind();
  return;
}

void Mesh::draw() const
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
    Texture::active_texture(i);

    std::string number;
    std::string name;
    auto type = textures[i]->get_type();
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

    // Get the current shader
    GLint prog = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prog);

    // Set uniforms
    Shader::set_int(prog, ("material." + name + number).c_str(), i);
    textures[i]->bind();
  }
  
  Texture::active_texture(0);

  // draw mesh
  this->vao.bind();
  Gl::draw_elements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
  this->vao.unbind();

  Texture::active_texture(0);
  return;
}

//
// Builder functions
//

Mesh::Builder &Mesh::Builder::vertices(std::vector<Vertex> &&vertices)
{
  this->conf.vertices = vertices;
  return *this;
}

Mesh::Builder &Mesh::Builder::indices(std::vector<unsigned int> &&indices)
{
  this->conf.indices = indices;
  return *this;
}

Mesh::Builder &Mesh::Builder::texture(std::shared_ptr<Texture> &&texture)
{
  this->conf.textures.push_back(std::move(texture));
  return *this;
}

Mesh::Builder &Mesh::Builder::texture(Texture &&texture)
{
  std::shared_ptr<Texture> shared = std::make_shared<Texture>(std::move(texture));
  this->conf.textures.push_back(std::move(shared));
  return *this;
}

Mesh::Builder &Mesh::Builder::textures(std::vector<std::shared_ptr<Texture>> &&textures)
{
  for (auto t : textures)
    this->conf.textures.push_back(std::move(t));
  return *this;
}

Mesh Mesh::Builder::build()
{
  return Mesh(std::move(this->conf));
}
