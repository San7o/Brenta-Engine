#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#ifndef GL_HELPER_h
#include "gl_helper.h"
#endif

#ifndef TEXTURE_H
#include "texture.h"
#endif

#ifndef SHADER_H
#include "shader.h"
#endif

#ifndef VAO_H
#include "vao.h"
#endif

#ifndef VBO_H
#include "vbo.h"
#endif

#ifndef EBO_H
#include "ebo.h"
#endif

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

    Mesh(std::vector<Types::Vertex> vertices, std::vector<unsigned int> indices,
                    std::vector<Types::Texture> textures);
    void Draw(Shader &shader);
private:
    // render data
    VAO vao;
    VBO vbo;
    EBO ebo;
    void setupMesh();
};

} // namespace ECS

#endif
