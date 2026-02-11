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
  
  /**
   * @brief Create a new shader
   *
   * This method is used to create a new shader with the given name,
   * type, and path. The path is the path to the file that contains
   * the shader code. The type is the type of the shader
   * (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER).
   *
   * @param shader_name Name of the shader
   * @param type Type of the shader
   * @param path Path to the file that contains the shader code
   * @return true on success, or false on error
   *
   * You can provide any number of types and paths, those will be all
   * compiled and linked in the same program.
   */
  template <typename... Args>
  static bool create(const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args)
  {
    std::vector<Shader::Id> compiled_shaders = {};
    if (!compile_shaders(compiled_shaders, type, path, args...))
    {
      ERROR("shader: error compiling shader {}", path.c_str());
      return false;
    }

    // shader Program
    Shader::Id id = glCreateProgram();
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [&id](auto shader) { glAttachShader(id, shader); });

    glLinkProgram(id);
    if (!Shader::check_compile_errors(id, "PROGRAM"))
    {
      return false;
    }

    Shader::shaders.insert({shader_name, id});
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [](auto shader) { glDeleteShader(shader); });
    return true;
  }

  /**
   * @brief Create a new shader with feedback varyings
   *
   * @param feedback_varyings Array of feedback varyings
   * @param num_varyings Number of feedback varyings
   * @param shader_name Name of the shader
   * @param type Type of the shader
   * @param path Path to the file that contains the shader code
   * @return true on success, or false on error
   *
   * Same as the New method, but adds feedback varyings to the shader,
   * so that the output of the shader can be saved in a buffer object.
   *
   * Example feedback_varyings:
   * const GLchar* feedback_varyings[] = {"outValue"};
   */
  template <typename... Args>
  static bool create(const GLchar **feedback_varyings, int num_varyings,
                     const Shader::Name &shader_name,
                     Shader::Type type, const std::filesystem::path &path,
                     Args... args)
  {
    std::vector<Shader::Id> compiled_shaders = {};
    if (!compile_shaders(compiled_shaders, type, path, args...))
    {
      ERROR("shader: error compiling shader {}", path.c_str())
      return false;
    }

    // shader Program
    Shader::Id id = glCreateProgram();
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [&id](auto shader) { glAttachShader(id, shader); });

    if (feedback_varyings != nullptr)
    {
      glTransformFeedbackVaryings(id, num_varyings, feedback_varyings,
                                  GL_INTERLEAVED_ATTRIBS);
    }

    glLinkProgram(id);
    if (!Shader::check_compile_errors(id, "PROGRAM"))
    {
      return false;
    }

    Shader::shaders.insert({shader_name, id});
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [](auto shader) { glDeleteShader(shader); });

    return true;
  }

  static bool
  compile_shaders([[maybe_unused]] std::vector<Shader::Id> &compiled)
  {
    return true;
  }

  template <typename... Args>
  static bool compile_shaders(std::vector<Shader::Id> &compiled,
                              Shader::Type type, const std::filesystem::path &path,
                              Args... args)
  {
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
      file.open(path);
      if (!file.is_open()) throw "Cannot open file";
      std::stringstream stream;
      stream << file.rdbuf();
      file.close();
      code = stream.str();
    }
    catch (std::ifstream::failure &e)
    {
      ERROR("shader: error reading shader file: {}", path.c_str());
      return false;
    }

    if (code.empty())
    {
      ERROR("shader: file is empty: {}", path.c_str());
      return false;
    }

    const char *shader_code = code.c_str();
    
    GLenum shader_type_gl;
    switch(type)
    {
    case Fragment: shader_type_gl = GL_FRAGMENT_SHADER; break;
    case Vertex: shader_type_gl   = GL_VERTEX_SHADER; break;
    case Geometry: shader_type_gl = GL_GEOMETRY_SHADER; break;
    case Compute: shader_type_gl  = GL_COMPUTE_SHADER; break;
    default: shader_type_gl       = 0; break;
    }
    
    unsigned int shader = glCreateShader(shader_type_gl);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    if (!Shader::check_compile_errors(shader, "SHADER"))
    {
      return false;
    }

    compiled.push_back(shader);
    return compile_shaders(compiled, args...);
  }

  /**
   * @brief Get the ID of a shader
   *
   * @param shader_name Name of the shader
   * @return ID of the shader
   */
  static Shader::Id get_id(Shader::Name shader_name);

  /**
   * @brief Use the shader
   *
   * @param shader_name Name of the shader
   
   * You need to call this method before rendering anything with the
   * shader.
   
   * @return true on success, or false on error
   */
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

} // namespace brenta
