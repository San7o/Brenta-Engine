// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/mesh.hpp>
#include <brenta/renderer/opengl/shader.hpp>
#include <brenta/logger.hpp>

#include <cmath>

using namespace brenta;

Mesh::Mesh(const Config& conf)
{
  static int _id = 1;
  this->id            = _id;
  this->vertices      = conf.vertices;
  this->indices       = conf.indices;
  this->textures      = tenno::move(conf.textures);

  _id++;
  
  this->init();
  return;
}

Mesh::~Mesh()
{
  if (id == 0) return;
  
  EVENT(Logger::Event::Lifetime, "mesh: deleted {}", this->id);
  return;
}

void Mesh::init()
{
  if (this->vertices.size() == 0 || this->indices.size() == 0)
    return;
  
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

  EVENT(Logger::Event::Lifetime, "mesh: created {}", this->id);
  return;
}

void Mesh::draw() const
{
  if (this->vao.get_id() == 0)
  {
    ERROR("Mesh::draw: not initialized");
    return;
  }

  unsigned int diffuseNr  = 1;
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
    case Texture::Type::None:
      continue;
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

Mesh::Builder &Mesh::Builder::vertices(tenno::vector<Vertex> &&vertices)
{
  this->conf.vertices = vertices;
  return *this;
}

Mesh::Builder &Mesh::Builder::indices(tenno::vector<unsigned int> &&indices)
{
  this->conf.indices = indices;
  return *this;
}

Mesh::Builder &Mesh::Builder::texture(tenno::shared_ptr<Texture> texture)
{
  this->conf.textures.push_back(texture);
  return *this;
}

Mesh::Builder &Mesh::Builder::texture(Texture &&texture)
{
  tenno::shared_ptr<Texture> shared = tenno::make_shared<Texture>(tenno::move(texture));
  this->conf.textures.push_back(tenno::move(shared));
  return *this;
}

Mesh::Builder &Mesh::Builder::textures(tenno::vector<tenno::shared_ptr<Texture>> textures)
{
  for (auto t : textures)
    this->conf.textures.push_back(t);
  return *this;
}

#ifndef PI
  #define PI 3.14159265358979323846f
#endif

Mesh::Builder &Mesh::Builder::shape(Mesh::Shape shape)
{
  switch (shape)
  {
  case Mesh::Shape::Triangle:
    this->conf.vertices = {
      { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
      { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
      { { 0.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 1.0f} }};
    this->conf.indices = {0, 1 , 2};
    break;
  case Mesh::Shape::Square:
    this->conf.vertices = {
      { {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
      { { 1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
      { { 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
      { {-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} }};
    this->conf.indices = {0, 1 , 2, 0, 2, 3};
    break;
  case Mesh::Shape::Circle: {
    const int num_vertices = 50;
    const float delta = PI * 2 / num_vertices;

    // Center vertex
    this->conf.vertices = {{
        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}
      }};

    for (int i = 0; i <= num_vertices; ++i)
    {
      float angle = delta * i;
      float x = std::cos(angle);
      float y = std::sin(angle);
      float tex_x = (x + 1) * 0.5f;
      float tex_y = (y + 1) * 0.5f;
      this->conf.vertices.push_back({
          {x, y, 0.0f}, {1.0f, 1.0f, 1.0f}, {tex_x, tex_y}
        });
    }

    for (int i = 1; i <= num_vertices; ++i)
    {
      this->conf.indices.push_back(0);
      this->conf.indices.push_back(i-1);
      this->conf.indices.push_back(i);
    }
    this->conf.indices.push_back(0);
    this->conf.indices.push_back(num_vertices);
    this->conf.indices.push_back(1);
    
    break;
  }

  case Mesh::Shape::Piramid:
    
    this->conf.vertices = {
      // Base
      { {-1,-1,-1}, {0,-1,0}, {0,0} }, //0
      { { 1,-1,-1}, {0,-1,0}, {1,0} }, //1
      { { 1,-1, 1}, {0,-1,0}, {1,1} }, //2
      { {-1,-1, 1}, {0,-1,0}, {0,1} }, //3
      // Side 1 (0,4,1)
      { {-1,-1,-1}, {0,0,0}, {0,0} },   //4
      { { 0, 1, 0}, {0,0,0}, {0.5,1} }, //5
      { { 1,-1,-1}, {0,0,0}, {1,0} },   //6
      // Side 2 (1,4,2)
      { { 1,-1,-1}, {0,0,0}, {0,0} },   //7
      { { 0, 1, 0}, {0,0,0}, {0.5,1} }, //8
      { { 1,-1, 1}, {0,0,0}, {1,0} },   //9
      // Side 3 (2,4,3)
      { { 1,-1, 1}, {0,0,0}, {0,0} },   //10
      { { 0, 1, 0}, {0,0,0}, {0.5,1} }, //11
      { {-1,-1, 1}, {0,0,0}, {1,0} },   //12
      // Side 4 (3,4,0)
      { {-1,-1, 1}, {0,0,0}, {0,0} },   //13
      { { 0, 1, 0}, {0,0,0}, {0.5,1} }, //14
      { {-1,-1,-1}, {0,0,0}, {1,0} }    //15
    };

    this->conf.indices = {
      // Base
      0,1,2,
      0,2,3,

      // Sides
      4,5,6,
      7,8,9,
      10,11,12,
      13,14,15
    };

    break;
  case Mesh::Shape::Cube:

    this->conf.vertices = {
      // Front
      { {-1,-1, 1}, {0, 0, 1}, {0, 0} },
      { { 1,-1, 1}, {0, 0, 1}, {1, 0} },
      { { 1, 1, 1}, {0, 0, 1}, {1, 1} },
      { {-1, 1, 1}, {0, 0, 1}, {0, 1} },
      // Back
      { { 1,-1,-1}, {0, 0, -1}, {0, 0} },
      { {-1,-1,-1}, {0, 0, -1}, {1, 0} },
      { {-1, 1,-1}, {0, 0, -1}, {1, 1} },
      { { 1, 1,-1}, {0, 0, -1}, {0, 1} },
      // Left
      { {-1,-1,-1}, {-1, 0, 0}, {0, 0} },
      { {-1,-1, 1}, {-1, 0, 0}, {1, 0} },
      { {-1, 1, 1}, {-1, 0, 0}, {1, 1} },
      { {-1, 1,-1}, {-1, 0, 0}, {0, 1} },
      // Right
      { {1,-1, 1}, {1, 0, 0}, {0, 0} },
      { {1,-1,-1}, {1, 0, 0}, {1, 0} },
      { {1, 1,-1}, {1, 0, 0}, {1, 1} },
      { {1, 1, 1}, {1, 0, 0}, {0, 1} },
      // Top
      { {-1,1, 1}, {0, 1, 0}, {0, 0} },
      { { 1,1, 1}, {0, 1, 0}, {1, 0} },
      { { 1,1,-1}, {0, 1, 0}, {1, 1} },
      { {-1,1,-1}, {0, 1, 0}, {0, 1} },
      // Bottom
      { {-1,-1,-1}, {0, -1, 0}, {0, 0} },
      { { 1,-1,-1}, {0, -1, 0}, {1, 0} },
      { { 1,-1, 1}, {0, -1, 0}, {1, 1} },
      { {-1,-1, 1}, {0, -1, 0}, {0, 1} }
    };

    this->conf.indices = {
      0,1,2,    0,2,3,
      4,5,6,    4,6,7,
      8,9,10,   8,10,11,
      12,13,14, 12,14,15,
      16,17,18, 16,18,19,
      20,21,22, 20,22,23
    };

    break;
    
  case Mesh::Shape::Sphere:
  {
    const int stacks = 20;
    const int sectors = 20;

    for (int i = 0; i <= stacks; ++i)
    {
      float stack_angle = PI/2 - i * PI / stacks;
      float xy = cos(stack_angle);
      float z = sin(stack_angle);

      for (int j = 0; j <= sectors; ++j)
      {
        float sector_angle = j * 2 * PI / sectors;

        float x = xy * cos(sector_angle);
        float y = xy * sin(sector_angle);

        float u = (float)j / sectors;
        float v = (float)i / stacks;

        this->conf.vertices.push_back({
            glm::vec3(x,y,z),
            glm::vec3(x,y,z),
            glm::vec2(u,v)
          });
      }
    }

    for (int i = 0; i < stacks; ++i)
    {
      int k1 = i * (sectors + 1);
      int k2 = k1 + sectors + 1;

      for (int j = 0; j < sectors; ++j, ++k1, ++k2)
      {
        this->conf.indices.push_back(k1);
        this->conf.indices.push_back(k2);
        this->conf.indices.push_back(k1 + 1);

        this->conf.indices.push_back(k1 + 1);
        this->conf.indices.push_back(k2);
        this->conf.indices.push_back(k2 + 1);
      }
    }

    break;
  }
  default:
    break;
  }
  
  return *this;
}

Mesh Mesh::Builder::build()
{
  return Mesh(this->conf);
}
