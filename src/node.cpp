// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/renderer.hpp>
#include <brenta/node.hpp>

using namespace brenta;

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
  Script::current_node = this; // This is a big non-thread safe hack for now
  if (this->script)
    this->script->update(delta_time);

  for (auto& component : this->components)
    component->update(delta_time);
  
  for (auto& child : this->children)
    child->update(delta_time);
  return;
}

void Node::draw()
{
  for (auto& component : this->components)
    component->draw(this->world_matrix);
  
  for (auto& child : this->children)
    child->draw();
  return;
}
