#pragma once

#ifndef MESH_H
#define MESH_H

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

namespace ECS {

namespace Types {

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

} // namespace Types

class Mesh {
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
    void Draw(Types::ShaderName shader_name);
private:
    // render data
    Types::VAO vao;
    Types::Buffer vbo;
    Types::Buffer ebo;
    void setupMesh();
};

} // namespace ECS

#endif
