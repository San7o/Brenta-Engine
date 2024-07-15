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

class Model
{
    public:
        Model(std::string const& path);
        void Draw(Shader &shader);
    private:
        // model data
        std::vector<Mesh> meshes;
        std::vector<MyTexture> textures_loaded;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<MyTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, 
                                             std::string typeName);
};
#endif
