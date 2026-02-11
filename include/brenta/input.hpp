// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/subsystem.hpp>

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <expected>

namespace brenta
{

/**
 * @brief Input subsystem
 *
 * This subsystem is used to add and remove callbacks for keyboard and
 * mouse events. You can define your own callbacks and register them
 * with the input subsystem.
 */
class Input : public Subsystem
{
public:

  using MouseCallbackId  = std::string;
  using MouseCallback    = std::function<void(double, double)>;
  using KeyId            = int;
  using KeyboardCallback = std::function<void()>;
  
  class Builder;

  // Subsystem interface
  static const std::string subsystem_name;
  std::expected<void, Subsystem::Error> initialize() override;
  std::expected<void, Subsystem::Error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  Input() = default;
  ~Input() = default;

  // Member functions
  
  static Input &instance();
  
  static void add_keyboard_callback(KeyId key, KeyboardCallback callback);
  static void remove_keyboard_callback(KeyId key);
  
  static void
  add_mouse_callback(MouseCallbackId name,
                         std::function<void(double, double)> callback);
  static void
  remove_mouse_callback(MouseCallbackId name);

private:
  
  static std::unordered_map<KeyId, KeyboardCallback> keyboard_callbacks;
  static std::unordered_map<MouseCallbackId, MouseCallback> mouse_callbacks;
  static bool initialized;

  /**
   * @brief Keyboard callback
   *
   * This function is called when a key is pressed. It calls the
   * callback function associated with the key.
   *
   * @param window The window that received the event
   * @param key The key that was pressed or released
   * @param scancode The system-specific scancode of the key
   * @param action The GLFW key action (GLFW_PRESS...)
   * @param mods Bit field describing which modifier keys were held down
   */
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  /**
   * @brief Mouse position callback
   *
   * This function is called when the mouse is moved. It calls the
   * callback function associated with the mouse position.
   *
   * @param window The window that received the event
   * @param xpos The new x-coordinate, in screen coordinates, of the cursor
   * @param ypos The new y-coordinate, in screen coordinates, of the cursor
   */
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  
};

class Input::Builder : public Subsystem::Builder
{
public:

  Builder() = default;
  ~Builder() = default;
  
  brenta::Subsystem &build() override;
};
  
} // namespace brenta
