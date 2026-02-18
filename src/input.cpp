// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/input.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

std::unordered_map<brenta::Key, Input::KeyboardCallback> Input::keyboard_callbacks;
std::unordered_map<Input::MouseCallbackId, Input::MouseCallback>
  Input::mouse_callbacks;
const std::string Input::subsystem_name = "input";
bool Input::initialized = false;

//
// Subsystem interface
//

std::expected<void, Subsystem::Error> Input::initialize()
{
  if (this->is_initialized()) return {};
  
  Input::initialized = true;
  INFO("{} initialized", Input::subsystem_name);
  return {};
}

std::expected<void, Subsystem::Error> Input::terminate()
{
  if (!this->is_initialized()) return {};

  Input::initialized = false;
  INFO("{}: terminated", Input::subsystem_name);
  return {};
}

std::string Input::name()
{
  return Input::subsystem_name;
}

bool Input::is_initialized()
{
  return Input::initialized;
}

//
// Member functions
//

Input &Input::instance()
{
  static Input _input;
  return _input;
}

void Input::add_keyboard_callback(Key key, KeyboardCallback callback)
{
  Input::keyboard_callbacks[key] = callback;
  DEBUG("{}: aded callback for key: {}",
        Input::subsystem_name, (int) key);
  return;
}

void Input::remove_keyboard_callback(Key key)
{
  if (Input::keyboard_callbacks.find(key) == Input::keyboard_callbacks.end())
  {
    ERROR("{}: no callback found for key: {}", Input::subsystem_name, (int) key);
    return;
  }

  Input::keyboard_callbacks.erase(key);
  DEBUG("{}: removed callback for key: {}", Input::subsystem_name, (int) key);
  return;
}

void Input::key_callback(Key key, KeyAction action,
                         [[maybe_unused]] KeyMods mods)
{
  DEBUG("{}: received key callback", Input::subsystem_name);

  if (action == KeyAction::Press)
  {
    if (Input::keyboard_callbacks.find(key) != Input::keyboard_callbacks.end())
    {
      Input::keyboard_callbacks.at(key)();
    }
  }
  return;
}

void Input::add_mouse_callback(MouseCallbackId callback_name,
                               MouseCallback callback)
{
  Input::mouse_callbacks[callback_name] = callback;
  DEBUG("{}: added callback for mouse: {}",
       Input::subsystem_name, callback_name);
  return;
}

void Input::remove_mouse_callback(MouseCallbackId callback_name)
{
  if (Input::mouse_callbacks.find(callback_name)
      == Input::mouse_callbacks.end())
  {
    ERROR("{}: no callback found for mouse: {}",
          Input::subsystem_name, callback_name);
    return;
  }

  Input::mouse_callbacks.erase(callback_name);
  DEBUG("{}: removed callback for mouse: {}",
        Input::subsystem_name, callback_name);
  return;
}

void Input::mouse_callback(double xpos,
                           double ypos)
{
  for (auto &callback : Input::mouse_callbacks)
  {
    callback.second(xpos, ypos);
  }
  return;
}

//
// Builder
//

Subsystem &Input::Builder::build()
{
  return Input::instance();
}
