/*
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */ 

#include "mesh.h"
#include "engine_logger.h"

#include <iostream>

using namespace ECS;

Mesh::Mesh(std::vector<Types::Vertex> vertices,
                std::vector<unsigned int> indices,
                std::vector<Types::Texture> textures,
                GLint wrapping, GLint filtering_min, GLint filtering_mag,
                GLboolean hasMipmap, GLint mipmap_min, GLint mipmap_max)
{
    this->vao.Init();
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;
    this->vbo = Types::Buffer(GL_ARRAY_BUFFER);
    this->ebo = Types::Buffer(GL_ELEMENT_ARRAY_BUFFER);
    this->wrapping = wrapping;
    this->filtering_min = filtering_min;
    this->filtering_mag = filtering_mag;
    this->hasMipmap = hasMipmap;
    this->mipmap_min = mipmap_min;
    this->mipmap_mag = mipmap_max;

    setupMesh();
}

void Mesh::Draw(Types::ShaderName shader_name)
{
    if (this->vao.GetVAO() == 0) {
        Logger::Log(Types::LogLevel::ERROR, "Mesh not initialized");
        return;
    }

    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < this->textures.size(); i++)
    {
        Texture::ActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        Shader::SetInt(shader_name, ("material." + name + number).c_str(), i);
        Texture::BindTexture(GL_TEXTURE_2D, textures[i].id, this->wrapping,
                        this->filtering_min, this->filtering_mag,
                        this->hasMipmap, this->mipmap_min, this->mipmap_mag);
    }
    Texture::ActiveTexture(GL_TEXTURE0);

    // draw mesh
    this->vao.Bind();
    GL::DrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    this->vao.Unbind();

    Texture::ActiveTexture(GL_TEXTURE0);
    
}

void Mesh::setupMesh()
{
    this->vbo.CopyVertices(this->vertices.size() * sizeof(Types::Vertex),
                    &this->vertices[0], GL_STATIC_DRAW);
    this->ebo.CopyIndices(this->indices.size() * sizeof(unsigned int),
                    &this->indices[0], GL_STATIC_DRAW);
    this->vao.SetVertexData(this->vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex),
                    (void*)0);
    this->vao.SetVertexData(this->vbo, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex),
                    (void*)offsetof(Types::Vertex, Normal));
    this->vao.SetVertexData(this->vbo, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Types::Vertex),
                    (void*)offsetof(Types::Vertex, TexCoords));

    GL::BindVertexArray(0);
}

Mesh::Builder& Mesh::Builder::set_vertices(std::vector<Types::Vertex> vertices)
{
    this->vertices = vertices;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_indices(std::vector<unsigned int> indices)
{
    this->indices = indices;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_textures(std::vector<Types::Texture> textures)
{
    this->textures = textures;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_wrapping(GLint wrapping)
{
    this->wrapping = wrapping;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_filtering_min(GLint filtering_min)
{
    this->filtering_min = filtering_min;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_filtering_mag(GLint filtering_mag)
{
    this->filtering_mag = filtering_mag;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_hasMipmap(GLboolean hasMipmap)
{
    this->hasMipmap = hasMipmap;
    return *this;
}

Mesh::Builder& Mesh::Builder::set_mipmap_min(GLint mipmap_min)
{
    this->mipmap_min = mipmap_min;
    return *this;
}

Mesh Mesh::Builder::build()
{
    return Mesh(this->vertices, this->indices, this->textures,
                this->wrapping, this->filtering_min, this->filtering_mag,
                this->hasMipmap, this->mipmap_min, this->mipmap_mag);
}
