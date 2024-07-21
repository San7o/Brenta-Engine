#include "mesh.h"
#include "engine_logger.h"

#include <iostream>

using namespace ECS;

Mesh::Mesh(std::vector<Types::Vertex> vertices, std::vector<unsigned int> indices, std::vector<Types::Texture> textures)
{
    this->vao.Init();
    this->vertices = vertices;
    this->indices  = indices;
    this->textures = textures;
    this->vbo = Types::Buffer(GL_ARRAY_BUFFER);
    this->ebo = Types::Buffer(GL_ELEMENT_ARRAY_BUFFER);

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
        Shader::SetFloat(shader_name, ("material." + name + number).c_str(), i);
        Texture::BindTexture(GL_TEXTURE_2D, textures[i].id);
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

