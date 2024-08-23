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
 * The above copyright notice and this permission notice shall be included in
 all
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

#include "model.hpp"

#include "engine_logger.hpp"

#include <iostream>

using namespace Brenta;

Model::Model(std::string const &path, GLint wrapping, GLint filtering_min,
             GLint filtering_mag, GLboolean hasMipmap, GLint mipmap_min,
             GLint mipmap_mag, bool flip)
{
    this->wrapping = wrapping;
    this->filtering_min = filtering_min;
    this->filtering_mag = filtering_mag;
    this->hasMipmap = hasMipmap;
    this->mipmap_min = mipmap_min;
    this->mipmap_mag = mipmap_mag;
    this->flip = flip;
    loadModel(path);
}

void Model::Draw(Types::ShaderName shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    /* Load with assimp */
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !scene->mRootNode)
    {
        ERROR("Could not load model with assimp: ", importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Types::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Types::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Types::Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Types::Texture> diffuseMaps = loadMaterialTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Types::Texture> specularMaps = loadMaterialTextures(
            material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());
    }

    return Mesh(vertices, indices, textures, this->wrapping,
                this->filtering_min, this->filtering_mag, this->hasMipmap,
                this->mipmap_min, this->mipmap_mag);
}

std::vector<Types::Texture> Model::loadMaterialTextures(aiMaterial *mat,
                                                        aiTextureType type,
                                                        std::string typeName)
{
    std::vector<Types::Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Types::Texture texture;
            std::string path = directory + "/" + std::string(str.C_Str());
            texture.id = Texture::LoadTexture(
                path, this->wrapping, this->filtering_min, this->filtering_mag,
                this->hasMipmap, this->mipmap_min, this->mipmap_mag,
                this->flip);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Model::Builder &Model::Builder::set_path(std::string path)
{
    this->path = path;
    return *this;
}

Model::Builder &Model::Builder::set_wrapping(GLint wrapping)
{
    this->wrapping = wrapping;
    return *this;
}

Model::Builder &Model::Builder::set_filtering_min(GLint filtering_min)
{
    this->filtering_min = filtering_min;
    return *this;
}

Model::Builder &Model::Builder::set_filtering_mag(GLint filtering_mag)
{
    this->filtering_mag = filtering_mag;
    return *this;
}

Model::Builder &Model::Builder::set_hasMipmap(GLboolean hasMipmap)
{
    this->hasMipmap = hasMipmap;
    return *this;
}

Model::Builder &Model::Builder::set_mipmap_min(GLint mipmap_min)
{
    this->mipmap_min = mipmap_min;
    return *this;
}

Model::Builder &Model::Builder::set_mipmap_mag(GLint mipmap_mag)
{
    this->mipmap_mag = mipmap_mag;
    return *this;
}

Model::Builder &Model::Builder::set_flip(bool flip)
{
    this->flip = flip;
    return *this;
}

Model Model::Builder::build()
{
    return Model(this->path, this->wrapping, this->filtering_min,
                 this->filtering_mag, this->hasMipmap, this->mipmap_min,
                 this->mipmap_mag, this->flip);
}
