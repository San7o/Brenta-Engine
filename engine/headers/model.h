#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>

#include <string>
#include <vector>

#include "shader.h"
#include "mesh.h"

namespace ECS {

class Model
{
public:
    GLint wrapping;
    GLint filtering_min;
    GLint filtering_mag;
    GLboolean hasMipmap;
    GLint mipmap_min;
    GLint mipmap_mag;
    bool flip;

    Model(std::string const& path, GLint wrapping = GL_REPEAT,
                    GLint filtering_min = GL_NEAREST,
                    GLint filtering_mag = GL_LINEAR,
                    GLboolean hasMipmap = GL_TRUE,
                    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                    GLint mipmap_mag = GL_LINEAR,
                    bool flip = true);

    void Draw(Types::ShaderName shader);
private:
    // model data
    std::vector<Mesh> meshes;
    std::vector<Types::Texture> textures_loaded;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Types::Texture> loadMaterialTextures
            (aiMaterial* mat, aiTextureType type, std::string typeName);
};

} // namespace ECS

#endif
