// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <brenta/mesh.hpp>
#include <brenta/shader.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>

namespace brenta
{

/**
 * @brief Model class
 *
 * This class is used to load a model from a file and draw it
 */
class model
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
  GLboolean has_mipmap;
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
  model()
  {
  }
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
  model(std::string const &path, GLint wrapping = GL_REPEAT,
        GLint filtering_min = GL_NEAREST, GLint filtering_mag = GL_LINEAR,
        GLboolean has_mipmap = GL_TRUE,
        GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR,
        GLint mipmap_mag = GL_LINEAR, bool flip = true);

  /**
   * @brief Builder class for Model
   */
  class builder;

  /**
   * @brief Draw the model
   *
   * @param shader Shader to use
   */
  void draw(types::shader_name_t shader);

private:
  // model data
  std::vector<mesh> meshes;
  std::vector<types::texture> textures_loaded;
  std::string directory;

  void load_model(std::string path);
  void process_node(aiNode *node, const aiScene *scene);
  mesh process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<types::texture> load_material_textures(aiMaterial *mat,
                                                     aiTextureType type,
                                                     std::string type_name);
};

/**
 * @brief Builder class for Model
 */
class model::builder
{
private:
  std::string path;
  GLint wrapping = GL_REPEAT;
  GLint filtering_min = GL_NEAREST;
  GLint filtering_mag = GL_LINEAR;
  GLboolean has_mipmap = GL_TRUE;
  GLint mipmap_min = GL_LINEAR_MIPMAP_LINEAR;
  GLint mipmap_mag = GL_LINEAR;
  bool flip = true;

public:
  builder &set_path(std::string path);
  builder &set_wrapping(GLint wrapping);
  builder &set_filtering_min(GLint filtering_min);
  builder &set_filtering_mag(GLint filtering_mag);
  builder &set_has_mipmap(GLboolean has_mipmap);
  builder &set_mipmap_min(GLint mipmap_min);
  builder &set_mipmap_mag(GLint mipmap_mag);
  builder &set_flip(bool flip);

  model build();
};

} // namespace brenta
