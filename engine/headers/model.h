#pragma once

#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef __glad_h_
#include <glad/glad.h>
#endif

#include <string>
#include <vector>

#ifndef SHADER_H
#include "shader.h"
#endif

#ifndef MESH_H
#include "mesh.h"
#endif

namespace ECS {

class Model
{
public:
    Model(std::string const& path);
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
