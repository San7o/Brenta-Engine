// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/logger.hpp>
#include <iostream>

using namespace brenta;

const Model::Config Model::default_config = {
  "",
  GL_REPEAT,
  GL_NEAREST,
  GL_LINEAR,
  GL_TRUE,
  GL_LINEAR_MIPMAP_LINEAR,
  GL_LINEAR,
  true,
};

Model::Model(Config conf)
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
  return;
}

Model::~Model()
{
  DEBUG("model: destroyed");
  return;
}

void Model::init()
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
  this->directory = this->path.parent_path();

  process_node(scene->mRootNode, scene);
  return;
}

void Model::draw(Shader::Name shader) const
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw(shader);
  }
  return;
}

void Model::process_node(aiNode *node, const aiScene *scene)
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
  return;
}

void Model::process_mesh(aiMesh *m, const aiScene *scene)
{
  std::vector<Mesh::Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;
  
  for (unsigned int i = 0; i < m->mNumVertices; i++)
  {
    Mesh::Vertex vertex;
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
  std::vector<std::shared_ptr<Texture>> diffuse =
    load_material_textures(material,
                           aiTextureType_DIFFUSE,
                           Texture::Type::Diffuse);
  textures.insert(textures.end(),
                  diffuse.begin(),
                  diffuse.end());
  
  std::vector<std::shared_ptr<Texture>> specular =
    load_material_textures(material,
                           aiTextureType_SPECULAR,
                           Texture::Type::Specular);
  textures.insert(textures.end(),
                  specular.begin(),
                  specular.end());

  Mesh mesh = Mesh({vertices, indices, textures, this->wrapping,
      this->filtering_min, this->filtering_mag, this->has_mipmap,
      this->mipmap_min, this->mipmap_mag});
  meshes.push_back(std::move(mesh));
}

std::vector<std::shared_ptr<Texture>>
Model::load_material_textures(aiMaterial *mat,
                              aiTextureType type,
                              Texture::Type type_brenta)
{
  std::vector<std::shared_ptr<Texture>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    std::string path =
      std::string(this->directory.c_str()) + "/" + std::string(str.C_Str());
    
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
      Texture t = Texture(path, this->flip, type_brenta);
      std::shared_ptr<Texture> t_ptr = std::make_shared<Texture>(std::move(t));
      textures_loaded.push_back(t_ptr);
      textures.push_back(t_ptr);
    }
  }
  return textures;
}

//
// Builder functions
//

Model::Builder &Model::Builder::path(const std::filesystem::path &path)
{
  this->conf.path = path;
  return *this;
}

Model::Builder &Model::Builder::wrapping(GLint wrapping)
{
  this->conf.wrapping = wrapping;
  return *this;
}

Model::Builder &Model::Builder::filtering_min(GLint filtering_min)
{
  this->conf.filtering_min = filtering_min;
  return *this;
}

Model::Builder &Model::Builder::filtering_mag(GLint filtering_mag)
{
  this->conf.filtering_mag = filtering_mag;
  return *this;
}

Model::Builder &Model::Builder::has_mipmap(GLboolean has_mipmap)
{
  this->conf.has_mipmap = has_mipmap;
  return *this;
}

Model::Builder &Model::Builder::mipmap_min(GLint mipmap_min)
{
  this->conf.mipmap_min = mipmap_min;
  return *this;
}

Model::Builder &Model::Builder::mipmap_mag(GLint mipmap_mag)
{
  this->conf.mipmap_mag = mipmap_mag;
  return *this;
}

Model::Builder &Model::Builder::flip(bool flip)
{
  this->conf.flip = flip;
  return *this;
}

Model Model::Builder::build()
{
  return Model(this->conf);
}
