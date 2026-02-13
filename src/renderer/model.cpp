// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/model.hpp>
#include <brenta/logger.hpp>
#include <iostream>

using namespace brenta;

Model::Model(Config &&conf)
{
  this->path          = conf.model_path;
  this->transform     = conf.transform;
  
  if (conf.model_path != "")
    this->load(conf.texture_props);

  for (size_t i = 0; i < conf.meshes.size(); ++i)
  {
    this->meshes.push_back(std::move(conf.meshes[i]));

    for (auto& t : conf.meshes[i].textures)
      this->textures_loaded.push_back(t);
  }
  
  DEBUG("model: initialized");
  return;
}

Model::~Model()
{
  return;
}

void Model::load(const Texture::Properties &props)
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
  this->directory = this->path.substr(0, this->path.find_last_of('/'));

  process_node(scene->mRootNode, scene, props);
  return;
}

void Model::draw() const
{
  for (unsigned int i = 0; i < meshes.size(); i++)
  {
    meshes[i].draw();
  }
  return;
}

Transform &Model::get_transform()
{
  return this->transform;
}

void Model::process_node(aiNode *node, const aiScene *scene,
                         const Texture::Properties &props)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *m = scene->mMeshes[node->mMeshes[i]];
    process_mesh(m, scene, props);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    process_node(node->mChildren[i], scene, props);
  }
  return;
}

void Model::process_mesh(aiMesh *m, const aiScene *scene,
                         const Texture::Properties &props)
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
                           Texture::Type::Diffuse,
                           props);
  textures.insert(textures.end(),
                  diffuse.begin(),
                  diffuse.end());
  
  std::vector<std::shared_ptr<Texture>> specular =
    load_material_textures(material,
                           aiTextureType_SPECULAR,
                           Texture::Type::Specular,
                           props);
  textures.insert(textures.end(),
                  specular.begin(),
                  specular.end());

  Mesh mesh = Mesh({vertices, indices, textures});
  meshes.push_back(std::move(mesh));
}

std::vector<std::shared_ptr<Texture>>
Model::load_material_textures(aiMaterial *mat,
                              aiTextureType type,
                              Texture::Type type_brenta,
                              const Texture::Properties &props)
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
      if (textures_loaded[j]->get_path() == path)
      {
        textures.push_back(textures_loaded[j]);
        skip = true;
        break;
      }
    }
    if (!skip)
    {
      Texture t = Texture::Builder()
        .type(type_brenta)
        .target(Texture::Target::Texture2D)
        .path(path)
        .properties(props)
        .build();
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

Model::Builder &Model::Builder::transform(const Transform& transform)
{
  this->conf.transform = transform;
  return *this;
}

Model::Builder &Model::Builder::path(const std::filesystem::path &path)
{
  this->conf.model_path = path;
  return *this;
}

Model::Builder &Model::Builder::texture_props(const Texture::Properties &props)
{
  this->conf.texture_props = props;
  return *this;
}

Model::Builder &Model::Builder::mesh(Mesh &&mesh)
{
  this->conf.meshes.push_back(std::move(mesh));
  return *this;
}

Model::Builder &Model::Builder::meshes(std::vector<Mesh> &&meshes)
{
  for (size_t i = 0; i < meshes.size(); ++i)
    this->conf.meshes.push_back(std::move(meshes[i]));
  return *this;
}

Model Model::Builder::build()
{
  return Model(std::move(this->conf));
}
