// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <tenno/utility.hpp>
#include <tenno/memory.hpp>

#include <string>
#include <filesystem>
#include <optional>

class lua_State;

namespace brenta
{

class Node;
  
//
// Script
// ------
//
// Scripts are an iterface for storing and running lua programs.
//
class Script
{
public:

  friend class Node;
  
  Script()  = default;
  Script(tenno::weak_ptr<Node> node, const std::string &source)
    : source(source), node(node)
  {
    this->init();
  }
  Script(tenno::weak_ptr<Node> node, const std::filesystem::path &path)
    : path(path), node(node)
  {
    this->init();
  }
  Script(Script&& other)
  {
    this->source = tenno::move(other.source);
    this->path   = tenno::move(other.path);
    this->node   = tenno::move(other.node);
    this->state  = other.state;
    other.state  = nullptr;
  }
  ~Script();

  Script& operator=(Script&& other)
  {
    this->source = tenno::move(other.source);
    this->path   = tenno::move(other.path);
    this->node   = tenno::move(other.node);
    this->state  = other.state;
    other.state  = nullptr;
    
    return *this;
  }
  
  void reload();
  void update(float delta_time);
  
private:

  std::optional<std::string>           source;
  std::optional<std::filesystem::path> path;
  lua_State                           *state = nullptr;
  tenno::weak_ptr<Node>                node;

  void init();

  static int lua_get_position(lua_State* L);
  static int lua_set_position(lua_State* L);
  static Node* current_node; // big non-thread safe hack for now
};

} // namespace brenta
