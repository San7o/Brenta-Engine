// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/script.hpp>
#include <brenta/node.hpp>
#include <brenta/logger.hpp>

extern "C"
{
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

using namespace brenta;

Node* Script::current_node = nullptr;

Script::~Script()
{
  if (!this->state) return;

  lua_close(this->state);
  this->state = nullptr;
}

int Script::lua_get_position(lua_State* L)
{
  auto* node = Script::current_node;
  if (!node) goto error;

  lua_pushnumber(L, node->get_local().get_x());
  lua_pushnumber(L, node->get_local().get_y());
  lua_pushnumber(L, node->get_local().get_z());
    return 3; 
  
 error:
  lua_pushnumber(L, 0);
  lua_pushnumber(L, 0);
  lua_pushnumber(L, 0);
  return 3; 
}

int Script::lua_set_position(lua_State* L)
{
  auto* node = Script::current_node;
  if (!node) return 0;
    
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);

  node->get_local().set_x(x);
  node->get_local().set_y(y);
  node->get_local().set_z(z);
  return 0;
}

void Script::init()
{
  if (this->state != nullptr) return;

  this->state = luaL_newstate();
  luaL_openlibs(this->state);

  if (this->path)
  {
    if (!luaL_dofile(this->state, this->path->c_str()) == LUA_OK)
    {
      ERROR("Script: error running script {}: {}",
            path->string(), lua_tostring(this->state, -1));
    }
  }
  else if (this->source)
  {
    if (!luaL_dostring(this->state, this->source->c_str()) == LUA_OK)
    {
      ERROR("Script: error running script: {}",
            lua_tostring(this->state, -1));
    }
  }
  else
  {
    ERROR("Script: tried to run script with no source");
  }

  // Functions
  
  lua_pushcfunction(this->state, lua_get_position);
  lua_setglobal(this->state, "get_position");
  lua_pushcfunction(this->state, lua_set_position);
  lua_setglobal(this->state, "set_position");
  
  DEBUG("Script: initialized");
}

void Script::reload()
{
  if (this->state != nullptr)
  {
    lua_close(this->state);
    this->state = nullptr;
  }

  this->init();
  DEBUG("Script: reloaded");
}

void Script::update(float delta_time)
{
  lua_getglobal(this->state, "update");
  if (!lua_isfunction(this->state, -1))
  {
    lua_pop(this->state, 1);
    return;
  }

  lua_pushnumber(this->state, delta_time);

  if (lua_pcall(this->state, 1, 0, 0) != LUA_OK) 
  {
    ERROR("Lua: Error: {}", lua_tostring(this->state, -1));
    lua_pop(this->state, 1);
  }
}
