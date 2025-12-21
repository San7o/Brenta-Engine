// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/logger.hpp>
#include <brenta/model.hpp>
#include <iostream>

using namespace brenta;

const model::config model::default_config = {
  "",
  GL_REPEAT,
  GL_NEAREST,
  GL_LINEAR,
  GL_TRUE,
  GL_LINEAR_MIPMAP_LINEAR,
  GL_LINEAR,
  true,
};

model::model(config conf)
{
  this->wrapping = conf.wrapping;
  this->filtering_min = conf.filtering_min;
  this->filtering_mag = conf.filtering_mag;
  this->has_mipmap = conf.has_mipmap;
  this->mipmap_min = conf.mipmap_min;
  this->mipmap_mag = conf.mipmap_mag;
  this->flip = conf.flip;
  this->path = conf.path;

  this->init();
  DEBUG("model: initialized");
}

model::~model()
{
  DEBUG("model: destroyed");
}

void model::init()
{
  // Load with assimp
  Assimp::Importer importer;
  const aiScene *scene =
    importer.ReadFile(this->path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    ERROR("model::load_model: Could not load model with assimp: {}",
          importer.GetErrorString());
    return;
  }
  directory = this->path.substr(0, this->path.find_last_of('/'));

  process_node(scene->mRootNode, scene);
}

void model::draw(shader::name_t shader) const
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
}

void model::process_node(aiNode *node, const aiScene *scene)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *m = scene->mMeshes[node->mMeshes[i]];
    process_mesh(m, scene);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    process_node(node->mChildren[i], scene);
  }
}

void model::process_mesh(aiMesh *m, const aiScene *scene)
{
  std::vector<types::vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<texture>> textures;
  
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

  aiMaterial *material = scene->mMaterials[m->mMaterialIndex];
  std::vector<std::shared_ptr<texture>> diffuse =
    load_material_textures(material,
                           aiTextureType_DIFFUSE,
                           "texture_diffuse");
  textures.insert(textures.end(),
                  diffuse.begin(),
                  diffuse.end());
  
  std::vector<std::shared_ptr<texture>> specular =
    load_material_textures(material,
                           aiTextureType_SPECULAR,
                           "texture_specular");
  textures.insert(textures.end(),
                  specular.begin(),
                  specular.end());

  mesh me = mesh({vertices, indices, textures, this->wrapping,
      this->filtering_min, this->filtering_mag, this->has_mipmap,
      this->mipmap_min, this->mipmap_mag});
  meshes.push_back(std::move(me));
}

std::vector<std::shared_ptr<texture>> model::load_material_textures(aiMaterial *mat,
                                                                    aiTextureType type,
                                                                    const std::string &typeName)
{
  std::vector<std::shared_ptr<texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    std::string path = directory + "/" + std::string(str.C_Str());
    
    for (unsigned int j = 0; j < textures_loaded.size(); j++)
    {
      // Do not load the same texture again
      if (textures_loaded[j]->path == path)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip)
    {
      texture t = texture(path, this->flip, typeName);
      std::shared_ptr<texture> t_ptr = std::make_shared<texture>(std::move(t));
      textures_loaded.push_back(t_ptr);
      textures.push_back(t_ptr);
    }
  }
  return textures;
}

//
// Builder functions
//

model::builder &model::builder::path(std::string path)
{
  this->conf.path = path;
  return *this;
}

model::builder &model::builder::wrapping(GLint wrapping)
{
  this->conf.wrapping = wrapping;
  return *this;
}

model::builder &model::builder::filtering_min(GLint filtering_min)
{
  this->conf.filtering_min = filtering_min;
  return *this;
}

model::builder &model::builder::filtering_mag(GLint filtering_mag)
{
  this->conf.filtering_mag = filtering_mag;
  return *this;
}

model::builder &model::builder::has_mipmap(GLboolean has_mipmap)
{
  this->conf.has_mipmap = has_mipmap;
  return *this;
}

model::builder &model::builder::mipmap_min(GLint mipmap_min)
{
  this->conf.mipmap_min = mipmap_min;
  return *this;
}

model::builder &model::builder::mipmap_mag(GLint mipmap_mag)
{
  this->conf.mipmap_mag = mipmap_mag;
  return *this;
}

model::builder &model::builder::flip(bool flip)
{
  this->conf.flip = flip;
  return *this;
}

model model::builder::build()
{
  return model(this->conf);
}
