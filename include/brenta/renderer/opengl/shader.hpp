// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <brenta/logger.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>

namespace brenta
{

/**
 * @brief Shader class
 *
 * This class is used to create and manage shaders. The shaders are
 * created using the new method, which takes the name of the shader,
 * the type of the shader, and the path to the file that contains the
 * shader code. Multile shaders can be compiled and linked together by
 * providing any number of types and paths paired.  The shader can be
 * used with the Use method, and the uniforms can be set using the
 * set_bool, set_int, set_float, set_mat4, set_vec3 methods.
 */
class Shader
{
public:

  using Name = std::string;
  using Id   = unsigned int;

  enum Type
  {
    Vertex,
    Fragment,
    Geometry,
    Compute,
  };

  Shader()  = delete;
  ~Shader() = delete;
  
  template <typename... Args>
  static bool create(const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args);
  template <typename... Args>
  static bool create(const GLchar **feedback_varyings, int num_varyings,
                     const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args);
  static bool
  compile_shaders([[maybe_unused]] std::vector<Shader::Id> &compiled);
  template <typename... Args>
  static bool compile_shaders(std::vector<Shader::Id> &compiled,
                              Shader::Type type, const std::filesystem::path &path,
                              Args... args);


  static Shader::Id get_id(Shader::Name shader_name);
  static bool use(Shader::Name shader_name);

  // Utility uniform functions

  static bool set_bool(Shader::Name shader_name,
                       const GLchar *name, bool value);
  static bool set_int(Shader::Name shader_name, const GLchar *name,
                      int value);
  static bool set_float(Shader::Name shader_name,
                        const GLchar *name, float value);
  static bool set_mat4(Shader::Name shader_name, const GLchar *name,
                       glm::mat4 value);
  static bool set_vec3(Shader::Name shader_name, const GLchar *name,
                       float x, float y, float z);
  static bool set_vec3(Shader::Name shader_name, const GLchar *name,
                       glm::vec3 value);

private:
  
  static bool check_compile_errors(Shader::Id shader, std::string type);
  
  /**
   * @brief Map of shaders
   *
   * This map is used to store the shaders that are created during the
   * execution of the program. The key is the name of the shader and
   * the value is the ID of the shader.
   */
  static std::unordered_map<Shader::Name, Shader::Id> shaders;
  
};

#include "shader.impl"
  
} // namespace brenta
