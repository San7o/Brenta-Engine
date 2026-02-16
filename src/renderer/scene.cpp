// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/renderer/scene.hpp>
#include <brenta/renderer/renderer.hpp>

#ifdef BRENTA_USE_ECS
#include <viotecs/viotecs.hpp>
using namespace viotecs;
#endif

using namespace brenta;

Scene& Scene::add_model(std::shared_ptr<Model> model)
{
  this->models.push_back(model);
  return *this;
}

Scene& Scene::set_active_camera(std::shared_ptr<Camera> camera)
{
  this->active_camera = camera;
  return *this;
}

std::shared_ptr<Camera> Scene::get_active_camera() const
{
  return this->active_camera;
}

void Scene::update([[maybe_unused]] float delta_time)
{
  #ifdef BRENTA_USE_ECS
  World::tick();
  #endif
  return;
}

void Scene::draw()
{
  Renderer::begin_frame({this->active_camera});
  for (auto& model : this->models)
    Renderer::submit(model);
  Renderer::end_frame();
  return;
}
