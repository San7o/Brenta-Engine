// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>

namespace brenta
{

class Transform
{
public:
  
  Transform() = default;
  Transform(double x, double y, double z);
  Transform(glm::vec3 vec);
  Transform(Transform& other) = default;
  Transform(const Transform& other) = default;

  // Model = Transform * Rotation * Scale
  glm::mat4 get_model_matrix();
  glm::vec3 get_pos() const;
  float     get_x() const;
  float     get_y() const;
  float     get_z() const;
  glm::quat get_rotation() const;

  void     set_pos(glm::vec3 new_pos);
  void     set_x(float x);
  void     set_y(float y);
  void     set_z(float z);

  Transform& translate(const glm::vec3& translation);
  Transform& rotate(const glm::quat &rotation);
  Transform& rotate_x(float degrees);
  Transform& rotate_y(float degrees);
  Transform& rotate_z(float degrees);
  Transform& scale(const glm::vec3& scale);
  
private:

  glm::vec3 m_position  = glm::vec3(0);
  glm::quat m_rotation  = glm::identity<glm::quat>();
  glm::vec3 m_scale     = glm::vec3(1.0f);

  glm::mat4 model_matrix;
  bool      dirty = true;  // set this to true when something was
                           // changed
  
};
  
} // namespace brenta
