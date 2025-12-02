// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <algorithm>
#include <brenta/logger.hpp>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace brenta
{

namespace types
{

typedef std::string shader_name_t;

} // namespace types

/**
 * @brief Shader class
 *
 * This class is used to create and manage shaders. The shaders
 * are created using the New method, which takes the name of the
 * shader, the type of the shader, and the path to the file that
 * contains the shader code. Multile shaders can be compiled and
 * linked together by providing any number of types and paths.
 * The shader can be used with the Use method, and the uniforms
 * can be set using the SetBool, SetInt, SetFloat, SetMat4, SetVec3
 * methods.
 */
class shader
{
public:
  /**
   * @brief Map of shaders
   *
   * This map is used to store the shaders that are created
   * during the execution of the program. The key is the name
   * of the shader and the value is the ID of the shader.
   */
  static std::unordered_map<types::shader_name_t, unsigned int> shaders;

  /**
   * @brief Create a new shader
   *
   * This method is used to create a new shader with the given
   * name, type, and path. The path is the path to the file that
   * contains the shader code. The type is the type of the shader
   * (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER).
   *
   * @param shader_name Name of the shader
   * @param type Type of the shader
   * @param path Path to the file that contains the shader code
   * @return true on success, or false on error
   *
   * You can provide any number of types and paths, those will be
   * all compiled and linked in the same program.
   */
  template <typename... Args>
  static bool create(std::string shader_name, GLenum type, std::string path,
                     Args... args)
  {
    std::vector<unsigned int> compiled_shaders = {};
    if (!compile_shaders(compiled_shaders, type, path, args...))
    {
      ERROR("Error compiling shader {}", path);
      return false;
    }

    /* shader Program */
    unsigned int ID = glCreateProgram();
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [&ID](auto shader) { glAttachShader(ID, shader); });

    glLinkProgram(ID);
    if (!shader::check_compile_errors(ID, "PROGRAM"))
    {
      return false;
    }

    shader::shaders.insert({shader_name, ID});
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
                     std::string shader_name, GLenum type, std::string path,
                     Args... args)
  {
    std::vector<unsigned int> compiled_shaders = {};
    if (!compile_shaders(compiled_shaders, type, path, args...))
    {
      ERROR("Error compiling shader {}", path)
      return false;
    }

    /* shader Program */
    unsigned int ID = glCreateProgram();
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [&ID](auto shader) { glAttachShader(ID, shader); });

    if (feedback_varyings != nullptr)
    {
      glTransformFeedbackVaryings(ID, num_varyings, feedback_varyings,
                                  GL_INTERLEAVED_ATTRIBS);
    }

    glLinkProgram(ID);
    if (!shader::check_compile_errors(ID, "PROGRAM"))
    {
      return false;
    }

    shader::shaders.insert({shader_name, ID});
    std::for_each(compiled_shaders.begin(), compiled_shaders.end(),
                  [](auto shader) { glDeleteShader(shader); });

    return true;
  }

  static bool
  compile_shaders([[maybe_unused]] std::vector<unsigned int> &compiled)
  {
    return true;
  }

  template <typename... Args>
  static bool compile_shaders(std::vector<unsigned int> &compiled, GLenum type,
                              std::string path, Args... args)
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
      ERROR("Error reading shader file: {}", path);
      return false;
    }

    if (code.empty())
    {
      ERROR("Shader file is empty: {}", path);
      return false;
    }

    const char *shader_code = code.c_str();
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);
    if (!shader::check_compile_errors(shader, "SHADER"))
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
  static unsigned int get_id(types::shader_name_t shader_name);

  /**
   * @brief Use the shader
   *
   * @param shader_name Name of the shader
   * You need to call this method before rendering anything
   * with the shader.
   * @return true on success, or false on error
   */
  static bool use(types::shader_name_t shader_name);

  /* Utility uniform functions */

  /**
   * @brief Set a boolean in the shader
   *
   * @param shader_name Name of the shader
   * @param name Name of the uniform boolean
   * @param value Value of the boolean
   * @return true on success, or false on error
   */
  static bool set_bool(types::shader_name_t shader_name,
                       const GLchar *name, bool value);
  /**
   * @brief Set an integer in the shader
   *
   * @param shader_name Name of the shader
   * @param name Name of the uniform integer
   * @param value Value of the integer
   * @return true on success, or false on error
   */
  static bool set_int(types::shader_name_t shader_name, const GLchar *name,
                      int value);
  /**
   * @brief Set a float in the shader
   *
   * @param shader_name Name of the shader
   * @param name Name of the uniform float
   * @param value Value of the float
   * @return true on success, or false on error
   */
  static bool set_float(types::shader_name_t shader_name,
                        const GLchar *name, float value);
  /**
   * @brief Set a 4x4 matrix in the shader
   *
   * @param shader_name Name of the shader
   * @param name Name of the uniform matrix
   * @param value Value of the matrix
   * @return true on success, or false on error
   */
  static bool set_mat4(types::shader_name_t shader_name, const GLchar *name,
                       glm::mat4 value);
  /**
   * @brief Set a 3D vector in the shader
   *
   * @param shader_name Name of uniform the shader
   * @param name Name of the vector
   * @param x X value of the vector
   * @param y Y value of the vector
   * @param z Z value of the vector
   * @return true on success, or false on error
   */
  static bool set_vec3(types::shader_name_t shader_name, const GLchar *name,
                       float x, float y, float z);
  /**
   * @brief Set a 3D vector in the shader
   *
   * @param shader_name Name of the shader
   * @param name Name of the uniform vector
   * @param value Value of the vector
   * @return true on success, or false on error
   */
  static bool set_vec3(types::shader_name_t shader_name, const GLchar *name,
                       glm::vec3 value);

private:
  static bool check_compile_errors(unsigned int shader, std::string type);
};

} // namespace brenta
