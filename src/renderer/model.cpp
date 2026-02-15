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
  this->path          = conf.model_path.string();
  this->transform     = conf.transform;
  this->material      = std::move(conf.material);
  
  if (conf.model_path != "")
    this->load(conf.texture_props);

  for (size_t i = 0; i < conf.meshes.size(); ++i)
  {
    this->meshes.push_back(std::move(conf.meshes[i]));

    for (auto& t : conf.meshes[i].textures)
      this->textures_loaded.push_back(t);
  }
  
  EVENT(Logger::Event::Lifetime, "model: initialized");
  return;
}

Model::~Model()
{
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

Material &Model::get_material()
{
  return this->material;
}

// TODO: pass props down
void Model::load(const Texture::Properties &props)
{
  tinyobj::ObjReaderConfig reader_config;
  std::string path_str = path.string();
  this->directory = path_str.substr(0, path_str.find_last_of('/'));
  reader_config.mtl_search_path = this->directory; // Path to material files

  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(this->path, reader_config))
  {
    if (!reader.Error().empty())
    {
      std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
  }

  auto& attrib = reader.GetAttrib();
  auto& shapes = reader.GetShapes();
  auto& materials = reader.GetMaterials();

  // Loop over shapes (equivalent to Assimp meshes/nodes)
  for (size_t s = 0; s < shapes.size(); s++)
  {
    process_shape(attrib, shapes[s], materials, props);
  }
}

void Model::process_shape(const tinyobj::attrib_t& attrib, 
                          const tinyobj::shape_t& shape,
                          const std::vector<tinyobj::material_t>& materials,
                          const Texture::Properties& props)
{
  // Map material_id -> Mesh Data (vertices and indices)
  std::map<int, std::vector<Mesh::Vertex>> per_mat_vertices;
  std::map<int, std::vector<unsigned int>> per_mat_indices;

  size_t index_offset = 0;

  for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++)
  {
    size_t fv = size_t(shape.mesh.num_face_vertices[f]);
    int mat_id = shape.mesh.material_ids[f]; // Get material for THIS face

    for (size_t v = 0; v < fv; v++)
    {
      tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
      Mesh::Vertex vertex;

      // Positions
      vertex.position = {
        attrib.vertices[3 * idx.vertex_index + 0],
        attrib.vertices[3 * idx.vertex_index + 1],
        attrib.vertices[3 * idx.vertex_index + 2]
      };

      // Normals
      if (idx.normal_index >= 0)
      {
        vertex.normal = {
          attrib.normals[3 * idx.normal_index + 0],
          attrib.normals[3 * idx.normal_index + 1],
          attrib.normals[3 * idx.normal_index + 2]
        };
      }

      // UVs with Flip
      if (idx.texcoord_index >= 0)
      {
        vertex.tex_coords = {
          attrib.texcoords[2 * idx.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
        };
      }

      per_mat_vertices[mat_id].push_back(vertex);
      per_mat_indices[mat_id].push_back(per_mat_indices[mat_id].size());
    }
    index_offset += fv;
  }

  // Now create a Mesh for each material group found in this shape
  for (auto const& [mat_id, verts] : per_mat_vertices)
  {
    std::vector<std::shared_ptr<Texture>> textures;
    if (mat_id >= 0)
    {
      textures = load_tiny_material(materials[mat_id], props);
    }
        
    // Push the mesh to your model's mesh list
    meshes.push_back(Mesh({verts, per_mat_indices[mat_id], textures}));
  }
}

std::vector<std::shared_ptr<Texture>>
Model::load_tiny_material(const tinyobj::material_t& mat,
                          const Texture::Properties &props)
{
  std::vector<std::shared_ptr<Texture>> textures;

  auto load_tex = [&](std::string tex_name, Texture::Type type)
  {
    if (tex_name.empty()) return;

    std::string full_path = this->directory + "/" + tex_name;
        
    // Check cache (textures_loaded)
    for (auto& loaded : textures_loaded)
    {
      if (loaded->get_path() == full_path)
      {
        textures.push_back(loaded);
        return;
      }
    }

    // Use your Builder pattern
    auto t =
      std::make_shared<Texture>(Texture::Builder()
                                .type(type)
                                .target(Texture::Target::Texture2D)
                                .path(full_path)
                                .properties(props)
                                .build());

    textures_loaded.push_back(t);
    textures.push_back(t);
  };
  
  load_tex(mat.diffuse_texname, Texture::Type::Diffuse);
  load_tex(mat.specular_texname, Texture::Type::Specular);

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

Model::Builder &Model::Builder::material(Material &&material)
{
  this->conf.material = std::move(material);
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
