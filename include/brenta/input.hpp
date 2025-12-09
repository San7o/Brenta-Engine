// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace types
{

typedef std::string mouse_callback_name_t;

} // namespace types

/**
 * @brief Input subsystem
 *
 * This subsystem is used to add and remove callbacks for keyboard and
 * mouse events. You can define your own callbacks and register them
 * with the input subsystem.
 */
class input : public subsystem
{
public:


  class builder;

  std::string subsystem_name = "input";
  
  input() = default;
  ~input() = default;
  
  /**
   * @brief Initialize the input system
   *
   * This function initializes the input subsystem. It should be
   * called before any other input function is called.
   */
  std::expected<void, std::string> initialize() override;

  /**
   * @brief Cleans up resources
   */
  std::expected<void, std::string> terminate() override;

  static input &instance();
  
  /**
   * @brief Add a keyboard callback
   *
   * This function adds a callback to a key press event. The callback is a
   * function that takes no arguments and returns void. The key enum is
   * defined in GLFW. When the key is pressed, the callback is called.
   *
   * @param key The key to add the callback to
   * @param callback The callback function
   */
  static void add_keyboard_callback(int key, std::function<void()> callback);
  /**
   * @brief Remove a keyboard callback
   *
   * This function removes a callback from a key press event. The key enum is
   * defined in GLFW. When the key is pressed, the callback is removed.
   *
   * @param key The key to remove the callback from
   */
  static void remove_keyboard_callback(int key);
  /**
   * @brief Keyboard callback
   *
   * This function is called when a key is pressed. It calls the callback
   * function associated with the key.
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
   * @brief Add a mouse position callback
   *
   * This function adds a callback to a mouse position event. The callback is
   * a function that takes two doubles and returns void. The two doubles are
   * the x and y position of the mouse. When the mouse is moved, the callback
   * is called.
   *
   * @param name The name of the callback
   * @param callback The callback function
   */
  static void
  add_mouse_pos_callback(types::mouse_callback_name_t name,
                         std::function<void(double, double)> callback);
  /**
   * @brief Remove a mouse position callback
   *
   * This function removes a callback from a mouse position event. When the
   * mouse is moved, the callback is removed.
   *
   * @param callback_name The name of the callback
   */
  static void
  remove_mouse_pos_callback(types::mouse_callback_name_t callback_name);
  /**
   * @brief Mouse position callback
   *
   * This function is called when the mouse is moved. It calls the callback
   * function associated with the mouse position.
   *
   * @param window The window that received the event
   * @param xpos The new x-coordinate, in screen coordinates, of the cursor
   * @param ypos The new y-coordinate, in screen coordinates, of the cursor
   */
  static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);

private:
  static std::unordered_map<int, std::function<void()>> keyboard_callbacks;
  static std::unordered_map<std::string, std::function<void(double, double)>>
    mouse_callbacks;
};

class input::builder : public subsystem::builder
{
public:

  builder() = default;
  ~builder() = default;
  
  brenta::subsystem &build() override;
};
  
} // namespace brenta
