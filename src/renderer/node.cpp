// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/node.hpp>
#include <brenta/renderer/renderer.hpp>

using namespace brenta;

void Node::add_model(tenno::shared_ptr<Model> model)
{
  this->models.push_back(model);
}

void Node::add_point_light(tenno::shared_ptr<PointLight> point_light)
{
  this->point_lights.push_back(point_light);
}

void Node::set_dir_light(tenno::shared_ptr<DirLight> dir_light)
{
  this->dir_light = dir_light;
}

void Node::set_local(Transform local)
{
  this->local = local;
}

Transform &Node::get_local()
{
  return this->local;
}

void Node::update_world_matrix()
{
  glm::mat4 parent_matrix = glm::mat4(1.0f);

  if (auto parent_ptr = parent->lock())
  {
    parent_matrix = parent_ptr->world_matrix;
  }

  this->world_matrix = parent_matrix * this->local.get_model_matrix();

  for (auto& child : this->children)
    child->update_world_matrix();
}

void Node::update(float delta_time)
{
  for (auto& child : this->children)
    child->update(delta_time);
  return;
}

void Node::draw()
{
  if (this->point_lights.size() > 0)
    Renderer::submit_point_lights(this->point_lights);
  if (this->dir_light)
    Renderer::submit_dir_light(this->dir_light.value());

  if (this->models.size() > 0)
    for (auto& model : this->models)
      Renderer::submit({this->world_matrix, model});

  for (auto& child : this->children)
    child->draw();
  return;
}

