// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/renderer.hpp>
#include <brenta/renderer/scene.hpp>

#ifdef BRENTA_USE_ECS
#include <viotecs/viotecs.hpp>
using namespace viotecs;
#endif

using namespace brenta;

tenno::shared_ptr<Node> Scene::get_root() const
{
  return this->root;
}

tenno::shared_ptr<Camera> Scene::get_active_camera() const
{
  return this->active_camera;
}

tenno::shared_ptr<Node> Scene::create_child(tenno::shared_ptr<Node> parent)
{
  auto child = tenno::make_shared<Node>();
  if (parent)
  {
    parent->children.push_back(child);
    child->parent = parent;
  }
  return child;
}

void Scene::update(float delta_time)
{
  #ifdef BRENTA_USE_ECS
  World::tick();
  #endif
  this->root->update(delta_time);
}

void Scene::draw()
{
  this->root->update_world_matrix();
  
  Renderer::begin_frame(this->active_camera);
  this->root->draw();
  Renderer::end_frame();
}
