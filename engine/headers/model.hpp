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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad/glad.h>

#include <string>
#include <vector>

#include "shader.hpp"
#include "mesh.hpp"

namespace ECS
{

/**
 * @brief Model class
 *
 * This class is used to load a model from a file and draw it
 */
class Model
{
public:
    /**
     * @brief Texture wrapping mode
     *
     * Check Mesh for more information
     */
    GLint wrapping;
    /**
     * @brief Texture filtering mode
     *
     * Check Mesh for more information
     */
    GLint filtering_min;
    /**
     * @brief Texture filtering mode
     *
     * Check Mesh for more information
     */
    GLint filtering_mag;
    /**
     * @brief If the texture has a mipmap
     *
     * Check Mesh for more information
     */
    GLboolean hasMipmap;
    /**
     * @brief Mipmap filtering mode
     *
     * Check Mesh for more information
     */
    GLint mipmap_min;
    /**
     * @brief Mipmap filtering mode
     *
     * Check Mesh for more information
     */
    GLint mipmap_mag;
    /**
     * @brief If the texture should be flipped
     */
    bool flip;

    /**
     * @brief Empty constructor
     *
     * Does nothing
     */
    Model() {}
    /**
     * @brief Construct a new Model object
     *
     * @param path Path to the model
     * @param wrapping Texture wrapping mode
     * @param filtering_min Texture filtering mode
     * @param filtering_mag Texture filtering mode
     * @param hasMipmap If the texture has a mipmap
     * @param mipmap_min Mipmap filtering mode
     * @param mipmap_mag Mipmap filtering mode
     * @param flip If the texture should be flipped
     */
    Model(std::string const& path, GLint wrapping = GL_REPEAT,
                    GLint filtering_min = GL_NEAREST,
                    GLint filtering_mag = GL_LINEAR,
                    GLboolean hasMipmap = GL_TRUE,
                    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
                    GLint mipmap_mag = GL_LINEAR,
                    bool flip = true);

    /**
     * @brief Builder class for Model
     */
    class Builder;

    /**
     * @brief Draw the model
     *
     * @param shader Shader to use
     */
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

/**
 * @brief Builder class for Model
 */
class Model::Builder
{
private:
    std::string path;
    GLint wrapping = GL_REPEAT;
    GLint filtering_min = GL_NEAREST;
    GLint filtering_mag = GL_LINEAR;
    GLboolean hasMipmap = GL_TRUE;
    GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR;
    GLint mipmap_mag = GL_LINEAR;
    bool flip = true;

public:
    Builder& set_path(std::string path);
    Builder& set_wrapping(GLint wrapping);
    Builder& set_filtering_min(GLint filtering_min);
    Builder& set_filtering_mag(GLint filtering_mag);
    Builder& set_hasMipmap(GLboolean hasMipmap);
    Builder& set_mipmap_min(GLint mipmap_min);
    Builder& set_mipmap_mag(GLint mipmap_mag);
    Builder& set_flip(bool flip);

    Model build();
};

} // namespace ECS
