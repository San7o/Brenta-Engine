#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "gl.h"
#include "texture.h"
#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct MyTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<MyTexture>    textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
                        std::vector<MyTexture> textures);
        void Draw(Shader &shader);
    private:
        // render data
        VAO vao;
        VBO vbo;
        EBO ebo;

        void setupMesh();
};
#endif
