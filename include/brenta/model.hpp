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
  
  GLint wrapping;
  GLint filtering_min;
  GLint filtering_mag;
  GLboolean has_mipmap;
  GLint mipmap_min;
  GLint mipmap_mag;
  bool flip;
  std::string path;

  struct config;
  class builder;

  static const config default_config;
  
  /**
   * @brief Empty constructor
   *
   * Does nothing
   */
  model()
  {
  }
  
  model(config conf);
  ~model() = default;

  constexpr model(model &&m) noexcept = default;

  void draw(types::shader_name_t shader);

private:

  std::vector<mesh> meshes;
  std::vector<types::texture> textures_loaded;
  std::string directory;

  void process_node(aiNode *node, const aiScene *scene);
  mesh process_mesh(aiMesh *mesh, const aiScene *scene);
  std::vector<types::texture> load_material_textures(aiMaterial *mat,
                                                     aiTextureType type,
                                                     std::string type_name);
  void init();
};

struct model::config
{
  std::string path;
  GLint wrapping;
  GLint filtering_min;
  GLint filtering_mag;
  GLboolean has_mipmap;
  GLint mipmap_min;
  GLint mipmap_mag;
  bool flip;
};
  
/**
 * @brief Builder class for Model
 */
class model::builder
{
private:

  model::config conf = model::default_config;
  
public:
  builder &path(std::string path);
  builder &wrapping(GLint wrapping);
  builder &filtering_min(GLint filtering_min);
  builder &filtering_mag(GLint filtering_mag);
  builder &has_mipmap(GLboolean has_mipmap);
  builder &mipmap_min(GLint mipmap_min);
  builder &mipmap_mag(GLint mipmap_mag);
  builder &flip(bool flip);

  model build();
};

} // namespace brenta
