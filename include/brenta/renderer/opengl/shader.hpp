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
#include <optional>

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
// Therefore shaders can be compiled into a program, loaded and
// configured via special variables called "uniforms". They are
// special variables you can create in the shader that can be set
// from "outside" the shader, usually to pass som kind of state or
// configuration.
//
// This class provides a simple API to create shaders, load them and
// set uniforms.
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

  // Static API
  
  // Creates and compiles several shaders (into a shader "program")
  template <typename... Args>
  static std::optional<Shader>
  create(const Shader::Name &shader_name,
         Shader::Type type, const std::filesystem::path &path,
         Args... args);
  
  template <typename... Args>
  static std::optional<Shader>
  create(const GLchar **feedback_varyings, int num_varyings,
         const Shader::Name &shader_name,
         Shader::Type type, const std::filesystem::path &path,
         Args... args);
  
  static std::optional<Shader> get_shader(Shader::Name shader_name);

  // Non static API
  
  Shader()  = delete;
  Shader(Shader::Id id, Shader::Name name) : id(id), name(name) {}
  
  Shader::Id        get_id();
  Shader::Name      get_name();
  bool              use();
  
  bool set_bool(const GLchar *unif_name, bool value);
  bool set_int(const GLchar *unif_name, int value);
  bool set_float(const GLchar *unif_name, float value);
  bool set_float2(const GLchar *unif_name, float v1, float v2);
  bool set_float3(const GLchar *unif_name, float v1, float v2, float v3);
  bool set_mat4(const GLchar *unif_name, glm::mat4 value);
  bool set_vec3(const GLchar *name, float x, float y, float z);
  bool set_vec3(const GLchar *unif_name, glm::vec3 value);

private:

  Shader::Id   id;
  Shader::Name name;
  
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
