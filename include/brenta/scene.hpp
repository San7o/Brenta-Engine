// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/camera.hpp>

#include <tenno/memory.hpp>

#include <filesystem>

namespace brenta
{

class Node;
class NodeComponent;
class Skybox;
  
class RenderPipeline;
  
//
// Scene
// -----
//
// The scene contains the root node of the node-graph and it is used
// to manage the rest of the graph, like creating new nodes or adding
// scripts.
//
// When the scene is updated, all the nodes are updated top to bottom,
// same with drawing.
//
class Scene
{
public:

  class Builder;

  Scene(Camera&& camera);
  Scene(tenno::shared_ptr<Camera> camera);
  Scene(Camera::Builder& camera);

  void set_skybox(tenno::shared_ptr<Skybox> skybox);
  void set_skybox(const tenno::vector<std::filesystem::path>& faces);

  tenno::shared_ptr<Node>   get_root() const;
  tenno::shared_ptr<Camera> get_camera() const;

  static void add_component(tenno::shared_ptr<Node> owner,
                            tenno::shared_ptr<NodeComponent> component);
  static tenno::shared_ptr<Node> create_child(tenno::shared_ptr<Node> parent);
  
  static void set_script(tenno::weak_ptr<Node> node, const std::filesystem::path &path);
  
  static void set_script(tenno::weak_ptr<Node> node, const std::string &source);
    
  void update(float delta_time);
  void draw(tenno::shared_ptr<RenderPipeline> pipeline,
            int width, int height);
  
private:

  tenno::shared_ptr<Node>   root;
  tenno::shared_ptr<Camera> active_camera;
  std::optional<tenno::shared_ptr<Skybox>>  skybox;
  
};

class Scene::Builder
{
public:
  
  Builder &camera(tenno::shared_ptr<Camera> camera);
  Builder &camera(Camera&& camera);
  Builder &camera(Camera::Builder& camera);

  Scene build();

private:

  tenno::shared_ptr<Camera> _camera;
  
};
  
}  // namespace brenta
