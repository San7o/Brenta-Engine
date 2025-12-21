// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brenta
{

namespace types
{

/**
 * @brief Translation util class
 *
 * This class is used to store the view, projection, and model
 * matrices of an object. The matrices can be set using the
 * set_view(), set_projection(), and set_model() methods.
 */
class translation
{
public:
  /**
   * @brief Camera view matrix
   */
  glm::mat4 view;
  /**
   * @brief Camera projection matrix
   */
  glm::mat4 projection;
  /**
   * @brief Object position matrix
   */
  glm::mat4 model;

  /**
   * @brief Translation constructor
   *
   * This constructor initializes the view, projection, and model
   * matrices to the identity matrix.
   */
  translation();
  /**
   * @brief Translation constructor
   *
   * This constructor initializes the view, projection, and model
   * matrices to the given matrices.
   * @param view The view matrix
   * @param projection The projection matrix
   * @param model The model matrix
   */
  translation(glm::mat4 view, glm::mat4 projection, glm::mat4 model);

  /**
   * @brief Set the view matrix
   * @param view The view matrix
   */
  void set_view(glm::mat4 view);
  /**
   * @brief Set the projection matrix
   * @param projection The projection matrix
   */
  void set_projection(glm::mat4 projection);
  /**
   * @brief Set the model matrix
   * @param model The model matrix
   */
  void set_projection(int window_width, int window_height,
                      float fov, float near, float far);
  /**
   * @brief Set the model matrix
   * @param model The model matrix
   */
  void set_model(glm::mat4 model);
  /**
   * @brief Translate the object
   * @param translation The translation vector
   */
  void translate(glm::vec3 translation);
  /**
   * @brief Rotate the object
   * @param rotation The rotation vector
   */
  void rotate(glm::vec3 rotation);
  /**
   * @brief Scale the object
   * @param scale The scale factor
   */
  void scale(float scale);
  /**
   * @brief Set the shader
   * @param shader_name The shader name
   */
  bool set_shader(shader::name_t shader_name);
};

} // namespace types

} // namespace brenta
