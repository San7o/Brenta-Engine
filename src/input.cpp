// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <brenta/window.hpp>
#include <brenta/input.hpp>
#include <brenta/logger.hpp>

using namespace brenta;

//
// Static variables
//

std::unordered_map<int, std::function<void()>> input::keyboard_callbacks;
std::unordered_map<std::string, std::function<void(double, double)>>
  input::mouse_callbacks;
const std::string input::subsystem_name = "input";

//
// Subsystem interface
//

std::expected<void, subsystem::error> input::initialize()
{
  window::set_key_callback(input::key_callback);
  window::set_mouse_pos_callback(input::mouse_pos_callback);

  INFO("{} initialized", input::subsystem_name);
  return {};
}

std::expected<void, subsystem::error> input::terminate()
{
  INFO("{}: terminated", input::subsystem_name);
  return {};
}

std::string input::name()
{
  return input::subsystem_name;
}

//
// Member functions
//

input &input::instance()
{
  static input _input;
  return _input;
}

void input::add_keyboard_callback(int key, std::function<void()> callback)
{
  input::keyboard_callbacks[key] = callback;
  DEBUG("{}: aded callback for key: {}",
       input::subsystem_name, std::to_string(key));
}

void input::remove_keyboard_callback(int key)
{
  if (input::keyboard_callbacks.find(key) == input::keyboard_callbacks.end())
  {
    ERROR("{}: no callback found for key: {}", input::subsystem_name, key);
    return;
  }

  input::keyboard_callbacks.erase(key);
  DEBUG("{}: removed callback for key: {}", input::subsystem_name, key);
}

void input::key_callback([[maybe_unused]] GLFWwindow *window,
                         [[maybe_unused]] int key,
                         [[maybe_unused]] int scancode, int action,
                         [[maybe_unused]] int mods)
{
  if (action == GLFW_PRESS)
  {
    if (input::keyboard_callbacks.find(key) != input::keyboard_callbacks.end())
    {
      input::keyboard_callbacks.at(key)();
    }
  }
}

void input::add_mouse_pos_callback(types::mouse_callback_name_t callback_name,
                                   std::function<void(double, double)> callback)
{
  input::mouse_callbacks[callback_name] = callback;
  DEBUG("{}: added callback for mouse: {}",
       input::subsystem_name, callback_name);
}

void input::remove_mouse_pos_callback(types::mouse_callback_name_t callback_name)
{
  if (input::mouse_callbacks.find(callback_name)
      == input::mouse_callbacks.end())
  {
    ERROR("{}: no callback found for mouse: {}",
          input::subsystem_name, callback_name);
    return;
  }

  input::mouse_callbacks.erase(callback_name);
  DEBUG("{}: removed callback for mouse: {}",
        input::subsystem_name, callback_name);
}

void input::mouse_pos_callback([[maybe_unused]] GLFWwindow *window,
                               double xpos,
                               double ypos)
{
  for (auto &callback : input::mouse_callbacks)
  {
    callback.second(xpos, ypos);
  }
}

//
// Builder
//

subsystem &input::builder::build()
{
  return input::instance();
}
