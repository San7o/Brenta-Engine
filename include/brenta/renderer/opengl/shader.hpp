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

//
// Shader
// ------
//
// Shaders are programs that run on the GPU. They can be written in
// a high-level language and then compiled into instructions that
// the GPU understands. OpenGL uses the GLSL language which is similar
// to C in syntax.
//
// To create a program, you need to compile the varous GLSL source
// files of the shader, then link them together. This class provides
// a simple API to create shaders, load them and set uniforms.
//
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

  // Creates and compiles several shaders (into a shader "program")
  template <typename... Args>
  static bool create(const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args);
  template <typename... Args>
  static bool create(const GLchar **feedback_varyings, int num_varyings,
                     const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args);

  static Shader::Id get_id(Shader::Name shader_name);
  static bool use(Shader::Name shader_name);

  // Utility uniform functions

  static bool set_bool(Shader::Name shader_name,
                       const GLchar *unif_name, bool value);
  static bool set_int(Shader::Name shader_name,
                      const GLchar *unif_name, int value);
  static bool set_float(Shader::Name shader_name,
                        const GLchar *unif_name, float value);
  static bool set_mat4(Shader::Name shader_name,
                       const GLchar *unif_name, glm::mat4 value);
  static bool set_vec3(Shader::Name shader_name,
                       const GLchar *name, float x, float y, float z);
  static bool set_vec3(Shader::Name shader_name,
                       const GLchar *unif_name, glm::vec3 value);

private:
  
  static bool
  compile_shaders([[maybe_unused]] std::vector<Shader::Id> &compiled);
  template <typename... Args>
  static bool compile_shaders(std::vector<Shader::Id> &compiled,
                              Shader::Type type, const std::filesystem::path &path,
                              Args... args);
  static bool check_compile_errors(Shader::Id shader);  
  static bool check_link_errors(Shader::Id shader);
  
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
