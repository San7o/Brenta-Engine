// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/renderer/opengl/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace brenta
{

/**
 * @brief Translation util class
 *
 * This class is used to store the view, projection, and model
 * matrices of an object. The matrices can be set using the
 * set_view(), set_projection(), and set_model() methods.
 */
class Translation
{
public:

  glm::mat4 view        = glm::mat4(1.0f);
  glm::mat4 projection  = glm::mat4(1.0f);
  glm::mat4 model       = glm::mat4(1.0f);

  Translation() = default;
  Translation(glm::mat4 view, glm::mat4 projection, glm::mat4 model)
    : view(view), projection(projection), model(model) {}

  void set_view(glm::mat4 view);
  void set_projection(glm::mat4 projection);
  void set_projection(int window_width, int window_height,
                      float fov, float near, float far);
  void set_model(glm::mat4 model);
  bool set_shader(Shader::Name shader_name);
  
  void translate(glm::vec3 translation);
  void rotate(glm::vec3 rotation);
  void scale(float scale);
};

} // namespace brenta
