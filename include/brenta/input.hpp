// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>
#include <brenta/key.hpp>

#include <GLFW/glfw3.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <expected>

namespace brenta
{

//
// Input subsystem
// ----------------
//
// This subsystem is used to add and remove callbacks for keyboard and
// mouse events. You can define your own callbacks and register them
// using `add_keyboard_callback(..)` and `add_mouse_callback(..)`.
//
class Input : public Subsystem
{
public:

  using MouseCallbackId  = std::string;
  using MouseCallback    = std::function<void(double, double)>;
  using KeyboardCallback = std::function<void()>;

  class Builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Member functions
  
  static Input &instance();
  
  static void add_keyboard_callback(Key key, KeyboardCallback callback);
  static void remove_keyboard_callback(Key key);
  
  static void add_mouse_callback(MouseCallbackId name, MouseCallback callback);
  static void remove_mouse_callback(MouseCallbackId name);

  // These functions will be called when a key or mouse event is
  // received. They are used to call the other registered callbacks.
  static void key_callback(Key key, KeyAction action, KeyMods mods);
  static void mouse_callback(double xpos, double ypos);
  
protected:

  static bool initialized;
  static std::unordered_map<brenta::Key, KeyboardCallback>  keyboard_callbacks;
  static std::unordered_map<MouseCallbackId, MouseCallback> mouse_callbacks;

  // Private constructors / destructors for singleton  
  Input()  = default;
  ~Input() = default;

};

class Input::Builder : public Subsystem::Builder
{
public:

  Builder()  = default;
  ~Builder() = default;
  
  brenta::Subsystem &build() override;
};
  
} // namespace brenta
