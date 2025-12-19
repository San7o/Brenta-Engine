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
public:

  class config;
  class builder;

  static const std::string subsystem_name;
  static const gl::config default_config;
  static gl::config init_config;

  // Subsystem interface
  std::expected<void, subsystem::error> initialize() override;
  std::expected<void, subsystem::error> terminate() override;
  std::string name() override;
  bool is_initialized() override;
  
  // Constructors / destructors
  gl() = default;
  ~gl() = default;

  // Member functions
  
  static gl &instance();
  
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

private:
  
  static bool initialized;
  
};

struct gl::config
{
public:
  bool enable_blending;
  bool enable_cull_face;
  bool enable_multisample;
  bool enable_depth_test;
};
  
class gl::builder : public subsystem::builder
{
private:

  gl::config conf;
  
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
