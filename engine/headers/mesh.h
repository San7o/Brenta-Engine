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

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "gl_helper.h"
#include "texture.h"
#include "shader.h"
#include "buffer.h"
#include "vao.h"

namespace ECS
{

namespace Types
{

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

} // namespace Types

class Mesh
{
public:
    // mesh data
    std::vector<Types::Vertex>  vertices;
    std::vector<unsigned int>   indices;
    std::vector<Types::Texture> textures;
    GLint wrapping;
    GLint filtering_min;
    GLint filtering_mag;
    GLboolean hasMipmap;
    GLint mipmap_min;
    GLint mipmap_mag;

    Mesh(std::vector<Types::Vertex> vertices, std::vector<unsigned int> indices,
                    std::vector<Types::Texture> textures,
                    GLint wrapping = GL_REPEAT,
                    GLint filtering_min = GL_NEAREST,
                    GLint filtering_mag = GL_LINEAR,
                    GLboolean hasMipmap = GL_TRUE,
                    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                    GLint mipmap_max = GL_LINEAR);
    class Builder;

    void Draw(Types::ShaderName shader_name);
private:
    // render data
    Types::VAO vao;
    Types::Buffer vbo;
    Types::Buffer ebo;
    void setupMesh();
};

class Mesh::Builder
{
private:
    std::vector<Types::Vertex>  vertices = {};
    std::vector<unsigned int>   indices  = {};
    std::vector<Types::Texture> textures = {};
    GLint wrapping = GL_REPEAT;
    GLint filtering_min = GL_NEAREST;
    GLint filtering_mag = GL_LINEAR;
    GLboolean hasMipmap = GL_TRUE;
    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR;
    GLint mipmap_mag = GL_LINEAR;

public:
    Builder& set_vertices(std::vector<Types::Vertex> vertices);
    Builder& set_indices(std::vector<unsigned int> indices);
    Builder& set_textures(std::vector<Types::Texture> textures);
    Builder& set_wrapping(GLint wrapping);
    Builder& set_filtering_min(GLint filtering_min);
    Builder& set_filtering_mag(GLint filtering_mag);
    Builder& set_hasMipmap(GLboolean hasMipmap);
    Builder& set_mipmap_min(GLint mipmap_min);
    Builder& set_mipmap_mag(GLint mipmap_mag);

    Mesh build();
};

} // namespace ECS
