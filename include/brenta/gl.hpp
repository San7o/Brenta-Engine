// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Helper functions to interact with OpenGL.
//

#pragma once

#include <brenta/subsystem.hpp>

#include <glad/glad.h> // OpenGL driver

#include <expected>
#include <string>

#define check_error() gl::_check_error(__FILE__, __LINE__)

namespace brenta
{

/**
 * @brief OpenGL helper functions
 *
 * This class contains helper functions to interact with OpenGL.
 */
class gl : public subsystem
{
protected:

  static bool enable_blending;
  static bool enable_cull_face;
  static bool enable_multisample;
  static bool enable_depth_test;
  
public:

  class builder;
  
  std::string subsystem_name = "gl";

  gl() = default;
  ~gl() = default;

  static gl &instance();

  std::expected<void, std::string> initialize() override;
  std::expected<void, std::string> terminate() override;
  
  /**
   * @brief Set Poligon Mode
   * @param enable Enable or disable poligon mode
   */
  static void set_poligon_mode(GLboolean enable);
  
  /**
   * @brief Set Viewport
   *
   * This function sets the viewport of the window.
   *
   * @param x          X coordinate of the viewport
   * @param y          Y coordinate of the viewport
   * @param width      Width of the viewport
   * @param height     Height of the viewport
   */
  static void set_viewport(int x, int y, int width, int height);
  
  /**
   * @brief Set Clear Color
   *
   * This function sets the clear color of the window.
   *
   * @param r Red component of the clear color
   * @param g Green component of the clear color
   * @param b Blue component of the clear color
   * @param a Alpha component of the clear color
   */
  static void set_color(float r, float g, float b, float a);
  
  /**
   * @brief Draw Arrays
   *
   * This function draws primitives from the array data.
   *
   * @param mode  Specifies what kind of primitives to render
   * @param first Specifies the starting index in the enabled arrays
   * @param count Specifies the number of indices to be rendered
   */
  static void draw_arrays(GLenum mode, int first, int count);
  
  /**
   * @brief Draw Elements
   *
   * This function draws primitives from the array data.
   *
   * @param mode    Specifies what kind of primitives to render
   * @param count   Specifies the number of elements to be rendered
   * @param type    Specifies the type of the values in indices
   * @param indices Specifies a pointer to the location where the indices are
   * stored
   */
  static void draw_elements(GLenum mode, int count, GLenum type,
                            const void *indices);
  
  /**
   * @brief Clear
   *
   * This function clears the color and depth buffer.
   */
  static void clear();
  
  static void bind_vertex_array(unsigned int n);
  
  /**
   * @brief Check OpenGL error
   *
   * This function checks if there is an OpenGL error.
   *
   * @return The error code
   */
  static GLenum _check_error(const char *file, int line);
};

class gl::builder : public subsystem::builder
{
private:

  bool enable_blending = false;
  bool enable_cull_face = false;
  bool enable_multisample = false;
  bool enable_depth_test = false;

public:

  builder() = default;
  ~builder() = default;

  builder &blending();
  builder &cull_face();
  builder &multisample();
  builder &depth_test();

  subsystem &build() override;
  
};
  
} // namespace brenta
