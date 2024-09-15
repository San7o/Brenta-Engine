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

using namespace brenta;

model::model(std::string const &path, GLint wrapping, GLint filtering_min,
             GLint filtering_mag, GLboolean has_mipmap, GLint mipmap_min,
             GLint mipmap_mag, bool flip)
{
    this->wrapping = wrapping;
    this->filtering_min = filtering_min;
    this->filtering_mag = filtering_mag;
    this->has_mipmap = has_mipmap;
    this->mipmap_min = mipmap_min;
    this->mipmap_mag = mipmap_mag;
    this->flip = flip;
    load_model(path);
}

void model::draw(types::shader_name_t shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}

void model::load_model(std::string path)
{
    /* Load with assimp */
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
        || !scene->mRootNode)
    {
        ERROR("Could not load model with assimp: {}", importer.GetErrorString());
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

void model::process_node(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *m = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(m, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

mesh model::process_mesh(aiMesh *m, const aiScene *scene)
{
    std::vector<types::vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<types::texture> textures;

    for (unsigned int i = 0; i < m->mNumVertices; i++)
    {
        types::vertex vertex;
        glm::vec3 vector;
        vector.x = m->mVertices[i].x;
        vector.y = m->mVertices[i].y;
        vector.z = m->mVertices[i].z;
        vertex.position = vector;

        vector.x = m->mNormals[i].x;
        vector.y = m->mNormals[i].y;
        vector.z = m->mNormals[i].z;
        vertex.normal = vector;

        if (m->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = m->mTextureCoords[0][i].x;
            vec.y = m->mTextureCoords[0][i].y;
            vertex.tex_coords = vec;
        }
        else
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < m->mNumFaces; i++)
    {
        aiFace face = m->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (m->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[m->mMaterialIndex];
        std::vector<types::texture> diffuseMaps = load_material_textures(
            material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<types::texture> specularMaps = load_material_textures(
            material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(),
                        specularMaps.end());
    }

    return mesh(vertices, indices, textures, this->wrapping,
                this->filtering_min, this->filtering_mag, this->has_mipmap,
                this->mipmap_min, this->mipmap_mag);
}

std::vector<types::texture> model::load_material_textures(aiMaterial *mat,
                                                          aiTextureType type,
                                                          std::string typeName)
{
    std::vector<types::texture> textures;
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
            types::texture texture;
            std::string path = directory + "/" + std::string(str.C_Str());
            texture.id = texture::load_texture(
                path, this->wrapping, this->filtering_min, this->filtering_mag,
                this->has_mipmap, this->mipmap_min, this->mipmap_mag,
                this->flip);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

model::builder &model::builder::set_path(std::string path)
{
    this->path = path;
    return *this;
}

model::builder &model::builder::set_wrapping(GLint wrapping)
{
    this->wrapping = wrapping;
    return *this;
}

model::builder &model::builder::set_filtering_min(GLint filtering_min)
{
    this->filtering_min = filtering_min;
    return *this;
}

model::builder &model::builder::set_filtering_mag(GLint filtering_mag)
{
    this->filtering_mag = filtering_mag;
    return *this;
}

model::builder &model::builder::set_has_mipmap(GLboolean has_hipmap)
{
    this->has_mipmap = has_mipmap;
    return *this;
}

model::builder &model::builder::set_mipmap_min(GLint mipmap_min)
{
    this->mipmap_min = mipmap_min;
    return *this;
}

model::builder &model::builder::set_mipmap_mag(GLint mipmap_mag)
{
    this->mipmap_mag = mipmap_mag;
    return *this;
}

model::builder &model::builder::set_flip(bool flip)
{
    this->flip = flip;
    return *this;
}

model model::builder::build()
{
    return model(this->path, this->wrapping, this->filtering_min,
                 this->filtering_mag, this->has_mipmap, this->mipmap_min,
                 this->mipmap_mag, this->flip);
}
