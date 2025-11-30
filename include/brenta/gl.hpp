// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

//
// Helper functions to interact with OpenGL.
//

#pragma once

#include <glad/glad.h> // OpenGL driver

namespace brenta
{

/**
 * @brief OpenGL helper functions
 *
 * This class contains helper functions to interact with OpenGL.
 */
class gl
{
public:
  /**
   * @brief Load OpenGL
   *
   * This function loads OpenGL and sets some default values.
   *
   * @param gl_blending    Enable blending
   * @param gl_cull_face   Enable face culling
   * @param gl_multisample Enable multisampling
   * @param gl_depth_test  Enable depth testing
   */
  static void load_opengl(bool gl_blending = true, bool gl_cull_face = true,
                          bool gl_multisample = true,
                          bool gl_depth_test = true);
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
   * @param SCR_WIDTH  Width of the viewport
   * @param SCR_HEIGHT Height of the viewport
   */
  static void set_viewport(int x, int y, int SCR_WIDTH, int SCR_HEIGHT);
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
  /**
   * @brief Enable Depth Test
   *
   * This function enables the depth test.
   */
  static void bind_vertex_array(unsigned int n);
  /**
   * @brief Check OpenGL error
   *
   * This function checks if there is an OpenGL error.
   *
   * @return The error code
   */
  static GLenum check_error_(const char *file, int line);
#define check_error() gl::check_error_(__FILE__, __LINE__)
};

} // namespace brenta
